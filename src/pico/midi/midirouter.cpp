/*
 * Copyright (c) 2023. Jonathan Bisson
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */


#include "midi/midirouter.hpp"

#include <storage.hpp>

#include "midi_Defs.h"
#include "config.hpp"
#include "MIDI.h"
#include "piomidi.hpp"
#include "serialmidi.hpp"
#include "usbmidi.hpp"


template<typename T>
void processSysex(midi::MidiInterface<T> &iface, MidiRouter &midi_router, uint8_t size, const uint8_t *sysex) {
    if (size < midi_router.device_prefix.size() + 3) return;

    uint32_t value = 0;

    // Remove first and last elements
    std::vector<uint8_t> message = std::vector<uint8_t>(sysex + 1, sysex + size - 1);

    for (size_t i = 0; i < midi_router.device_prefix.size(); i++) {
        if (message[i] != midi_router.device_prefix[i]) return;
    }

    message.erase(message.begin(), message.begin() + midi_router.device_prefix.size());

    switch (message[0]) {
        case MidiRouter::DUMP_MATRIX:
            if (message.size() != 1) goto nack;

            midi_router.send_matrix(iface);
            return;
        case MidiRouter::SET_MATRIX:
            if (message.size() != 1 + ROUTING_MATRIX_BINARY_SIZE) goto nack;
            message.erase(message.begin());
            midi_router.matrix.load_from_array(const_cast<uint8_t *>(message.data()));
            goto ack;
        case MidiRouter::GET_IJ: {
            if (message.size() != 3) goto nack;

            uint8_t i = message[1];
            uint8_t j = message[2];
            uint32_t bar = midi_router.matrix.get_element_2d(i, j);
            midi_router.send_element(iface, bar);
            return;
        }
        case MidiRouter::SET_IJ:
            if (message.size() != 7) goto nack;

            value = ((message[3] & 0x7f))
                    + ((message[4] & 0x7f) << 8)
                    + ((message[5] & 0x7f) << 16)
                    + ((message[6] & 0x7f) << 24);

            midi_router.matrix.set_element_2d(message[1],
                                              message[2],
                                              value);
            goto ack;
        case MidiRouter::SAVE:
            storage.save_routing_matrix(midi_router.matrix);
            goto ack;
        case MidiRouter::LOAD:
            storage.load_routing_matrix(midi_router.matrix);
            goto ack;
        case MidiRouter::RESET:
            midi_router.matrix.reset();
            goto ack;
    }

    // I know, and I love it

nack:
    midi_router.send_nack(iface);
    return;
ack:
    midi_router.send_ack(iface);
}

template<typename T, typename U>
void copy_midi_data(MidiRouter &router, midi::MidiInterface<T> &in, midi::MidiInterface<U> &out) {
    if (in.getType() == midi::MidiType::SystemExclusive) {
        uint8_t sysex_length = in.getSysExArrayLength();
        const byte *sysex = in.getSysExArray();
        out.sendSysEx(sysex_length, sysex);
        processSysex(in, router, sysex_length, sysex);
    } else {
        out.send(in.getType(), in.getData1(), in.getData2(), in.getChannel());
    }
}


#define FILTER_NOTE_ON 1
#define FILTER_NOTE_OFF 2
#define FILTER_CC 4

// bits for filtering later
// 7    6    5    4    3    2      1             0
//                          CC        Note OFF     Note On
// we can also add channels with higher numbers and shifting, will see

UsbMidi usb_midi;

PIO_SERIAL_MIDI(1, PIN_MIDI_1_TX, PIN_MIDI_1_RX)
PIO_SERIAL_MIDI(2, PIN_MIDI_2_TX, PIN_MIDI_2_RX)
PIO_SERIAL_MIDI(3, PIN_MIDI_3_TX, PIN_MIDI_3_RX)
PIO_SERIAL_MIDI(4, PIN_MIDI_4_TX, PIN_MIDI_4_RX)
SERIAL_MIDI(uart0, 5, PIN_MIDI_5_TX, PIN_MIDI_5_RX)

void MidiRouter::init() {
    device_prefix.reserve(4 + 1);
    device_prefix.insert(device_prefix.end(), SYSEX_COMMAND_PREFIX);
    device_prefix.push_back(SYSEX_DEVICE_ID);

    usb_midi.init();
    storage.init();

    storage.load_routing_matrix(matrix);

    usb_midi(1).begin(MIDI_CHANNEL_OMNI);
    usb_midi(2).begin(MIDI_CHANNEL_OMNI);
    usb_midi(3).begin(MIDI_CHANNEL_OMNI);
    usb_midi(4).begin(MIDI_CHANNEL_OMNI);
    usb_midi(5).begin(MIDI_CHANNEL_OMNI);

    MIDI_IF_1.begin(MIDI_CHANNEL_OMNI);
    MIDI_IF_2.begin(MIDI_CHANNEL_OMNI);
    MIDI_IF_3.begin(MIDI_CHANNEL_OMNI);
    MIDI_IF_4.begin(MIDI_CHANNEL_OMNI);
    MIDI_IF_5.begin(MIDI_CHANNEL_OMNI);
}


#define SIMPLE_ROUTER(n) if ( MIDI_IF_##n.read() ) \
    {                                              \
      if (debug) {                                 \
        debug_midi_message[0]=n;                   \
        debug_midi_message[1]=MIDI_IF_##n.getChannel(); \
        debug_midi_message[2]=MIDI_IF_##n.getType();    \
        debug_midi_message[3]=MIDI_IF_##n.getData1();   \
        debug_midi_message[4]=MIDI_IF_##n.getData2();   \
        midi_message_ready=true;                                             \
      }                                             \
      if (matrix.get_element_2d(n-1, 0) > 0) copy_midi_data(*this, MIDI_IF_##n, MIDI_IF_1);   \
      if (matrix.get_element_2d(n-1, 1) > 0) copy_midi_data(*this, MIDI_IF_##n, MIDI_IF_2);   \
      if (matrix.get_element_2d(n-1, 2) > 0) copy_midi_data(*this, MIDI_IF_##n, MIDI_IF_3);   \
      if (matrix.get_element_2d(n-1, 3) > 0) copy_midi_data(*this, MIDI_IF_##n, MIDI_IF_4);   \
      if (matrix.get_element_2d(n-1, 4) > 0) copy_midi_data(*this, MIDI_IF_##n, MIDI_IF_5);   \
      if (usb_midi.active()) copy_midi_data(*this, MIDI_IF_##n, usb_midi(n)); \
    }

void MidiRouter::loop() {
    // This is the physical routing
    SIMPLE_ROUTER(1)
    SIMPLE_ROUTER(2)
    SIMPLE_ROUTER(3)
    SIMPLE_ROUTER(4)
    SIMPLE_ROUTER(5)

    // This one has callbacks and works for all of them
    // we just need to pick up the cable number
    if (usb_midi.active()) {
        if (usb_midi(1).read()) {
            uint8_t cable = usb_midi.current_cable_limited();
            midi::MidiType type = usb_midi(1).getType();


            /*            switch (cable) {
                            case 2:
                                copy_midi_data(*this, usb_midi(cable), MIDI_IF_2);
                            break;
                            case 3:
                                copy_midi_data(*this, usb_midi(cable), MIDI_IF_3);
                            break;
                            case 4:
                                copy_midi_data(*this, usb_midi(cable), MIDI_IF_4);
                            break;
                            case 5:
                                copy_midi_data(*this, usb_midi(cable), MIDI_IF_5);
                            break;
                            default:
                                copy_midi_data(*this, usb_midi(cable), MIDI_IF_1);
                            break;
                        }
                    }
                    */
            if (type == midi::MidiType::SystemExclusive) {
                uint8_t sysex_length = usb_midi(cable).getSysExArrayLength();
                const byte *sysex = usb_midi(cable).getSysExArray();
                processSysex(usb_midi(cable), *this, sysex_length, sysex);
                switch (cable) {
                    case 1:
                        MIDI_IF_2.sendSysEx(sysex_length, sysex);
                        break;
                    case 2:
                        MIDI_IF_3.sendSysEx(sysex_length, sysex);
                        break;
                    case 3:
                        MIDI_IF_4.sendSysEx(sysex_length, sysex);
                        break;
                    case 4:
                        MIDI_IF_5.sendSysEx(sysex_length, sysex);
                        break;
                    default:
                        MIDI_IF_1.sendSysEx(sysex_length, sysex);
                        break;
                }
            }
            unsigned char data1 = usb_midi(1).getData1();
            unsigned char data2 = usb_midi(1).getData2();
            unsigned char channel = usb_midi(1).getChannel();

            switch (cable) {
                case 1:
                    MIDI_IF_2.send(type, data1, data2, channel);
                    break;
                case 2:
                    MIDI_IF_3.send(type, data1, data2, channel);
                    break;
                case 3:
                    MIDI_IF_4.send(type, data1, data2, channel);
                    break;
                case 4:
                    MIDI_IF_5.send(type, data1, data2, channel);
                    break;
                default:
                    MIDI_IF_1.send(type, data1, data2, channel);
            }
        }
    }
}

void MidiRouter::get_matrix(uint8_t *matrix_out) {
    matrix.save_to_array(matrix_out);
}

void MidiRouter::set_matrix(uint8_t *matrix_in) {
    matrix.load_from_array(matrix_in);
}

bool MidiRouter::usb_enabled() {
    return usb_midi.active();
}

void MidiRouter::set_debug(bool i) {
    debug = true;
}

bool MidiRouter::has_new_message() {
    bool m = midi_message_ready;
    midi_message_ready = false;
    return m;
}

std::vector<uint8_t> MidiRouter::create_message(std::vector<uint8_t> content) {
    std::vector<uint8_t> message;

    message.reserve(4 + 1 + content.size());
    message.insert(message.end(), SYSEX_COMMAND_PREFIX);
    message.push_back(SYSEX_DEVICE_ID);
    message.push_back(0x66);
    message.insert(message.end(), content.begin(), content.end());

    return message;
}

template<typename T>
void MidiRouter::send_matrix(midi::MidiInterface<T> iface) {
    std::vector<uint8_t> full_message = create_message(matrix.as_vector());
    iface.sendSysEx(full_message.size(), full_message.data());
}

template<typename T>
void MidiRouter::send_element(midi::MidiInterface<T> iface, uint32_t value) {
    std::vector<uint8_t> content = {
        static_cast<uint8_t>(value & 0x7F),
        static_cast<uint8_t>((value >> 8) & 0x7F),
        static_cast<uint8_t>((value >> 16) & 0x7F),
        static_cast<uint8_t>((value >> 24) & 0x7F)
    };
    std::vector<uint8_t> full_message = create_message(content);
    iface.sendSysEx(full_message.size(), full_message.data());
}

template<typename T>
void MidiRouter::send_ack(midi::MidiInterface<T> iface) {
    std::vector<uint8_t> content = {COMMANDS::ACK};
    std::vector<uint8_t> full_message = create_message(content);
    iface.sendSysEx(full_message.size(), full_message.data());
}

template<typename T>
void MidiRouter::send_nack(midi::MidiInterface<T> iface) {
    std::vector<uint8_t> content = {COMMANDS::NACK};
    std::vector<uint8_t> full_message = create_message(content);
    iface.sendSysEx(full_message.size(), full_message.data());
}

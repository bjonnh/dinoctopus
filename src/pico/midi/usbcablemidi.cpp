/**
 * Adapted from the tinyusb.org library
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 * Copyright (c) 2023. Jonathan Bisson
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "midi/usbcablemidi.hpp"

BEGIN_MIDI_NAMESPACE

    void UsbJackMIDI::begin() {
        usb_midi.begin();
    }

    void UsbJackMIDI::end() {
    }

    bool UsbJackMIDI::beginTransmission(midi::MidiType) {
        return true;
    }

    void UsbJackMIDI::write(uint8_t value) const {
        tud_midi_stream_write(cable, &value, 1);
    }

    void UsbJackMIDI::endTransmission() {
    }

    // This is modified from the tinyUSB library
    uint32_t UsbJackMIDI::tud_midi_n_stream_read_cable_aware(void *buffer, uint32_t bufsize) {
        TU_VERIFY(bufsize, 0);

        auto *buf8 = (uint8_t *) buffer;

        midid_stream_t *stream = &stream_read;

        uint32_t total_read = 0;
        while (bufsize) {
            // Get new packet from fifo, then set packet expected bytes
            if (stream->total == 0) {
                // return if there is no more data from fifo
                if (!usb_midi.readPacket(stream->buffer)) return total_read; // This is not ok

                uint8_t const code_index = stream->buffer[0] & 0x0f;
                cable = (stream->buffer[0] & 0xf0) >> 4;

                // MIDI 1.0 Table 4-1: Code Index Number Classifications
                switch (code_index) {
                    case MIDI_CIN_MISC:
                    case MIDI_CIN_CABLE_EVENT:
                        // These are reserved and unused, possibly issue somewhere, skip this packet
                        return 0;
                    case MIDI_CIN_SYSEX_END_1BYTE:
                    case MIDI_CIN_1BYTE_DATA:
                        stream->total = 1;
                        break;
                    case MIDI_CIN_SYSCOM_2BYTE     :
                    case MIDI_CIN_SYSEX_END_2BYTE  :
                    case MIDI_CIN_PROGRAM_CHANGE   :
                    case MIDI_CIN_CHANNEL_PRESSURE :
                        stream->total = 2;
                        break;
                    default:
                        stream->total = 3;
                        break;
                }
            }

            // Copy data up to bufsize
            auto const count = (uint8_t) tu_min32(stream->total - stream->index, bufsize);

            // Skip the header (1st byte) in the buffer
            memcpy(buf8, stream->buffer + 1 + stream->index, count);

            total_read += count;
            stream->index += count;
            buf8 += count;
            bufsize -= count;

            // complete current event packet, reset stream
            if (stream->total == stream->index) {
                stream->index = 0;
                stream->total = 0;
            }
        }

        return total_read;
    }

    byte UsbJackMIDI::read() {
        uint8_t ch;
        return tud_midi_n_stream_read_cable_aware(&ch, 1) ? (int) ch : (-1);
    }

    uint32_t UsbJackMIDI::tud_midi_n_available() {
        midid_stream_t const *stream = &stream_read;

        // We are monkey patching on the adafruit lib, so we need to add our own stream count
        return usb_midi.available() + (stream->total - stream->index);
    }

    unsigned UsbJackMIDI::available() {
        return tud_midi_n_available();
    }

    [[nodiscard]] uint8_t UsbJackMIDI::current_cable() const {
        return cable;
    }

END_MIDI_NAMESPACE

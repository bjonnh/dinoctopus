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

#include <config.hpp>
#include <cstdint>
#include <MIDI.h>

#include "routingmatrix.hpp"


#ifndef DINOCTOPUS_2040_MIDI_ROUTER_HPP
#define DINOCTOPUS_2040_MIDI_ROUTER_HPP

#define SERIAL_MIDI(uart, n, txpin, rxpin) midi::MySerialMIDI real_serial_##n(uart, txpin, rxpin); \
    midi::MidiInterface<midi::MySerialMIDI> MIDI_IF_##n(real_serial_##n);

class MidiRouter {
public:
    enum COMMANDS {
        IDENTITY = 0x00,
        DUMP_MATRIX = 0x01,
        SET_MATRIX = 0x02,
        GET_IJ = 0x03,
        SET_IJ = 0x04,
        SAVE = 0x05,
        LOAD = 0x06,
        RESET = 0x07,
        ACK = 0x60,
        NACK = 0x61
    };

    void init();

    void loop();

    void get_matrix(uint8_t *);

    void set_matrix(uint8_t *);

    bool usb_enabled();

    void set_debug(bool i);

    bool has_new_message();

    std::vector<uint8_t> create_message(std::vector<uint8_t> content);

    template<typename T>
    void send_matrix(midi::MidiInterface<T> iface);

    template<typename T>
    void send_element(midi::MidiInterface<T> iface, uint32_t value);

    template<typename T>
    void send_ack(midi::MidiInterface<T> iface);

    template<typename T>
    void send_nack(midi::MidiInterface<T> iface);

    uint8_t debug_midi_message[5];
    RoutingMatrix matrix;

    std::vector<uint8_t> device_prefix;
private:
    bool debug = false;
    bool midi_message_ready = true;
};


#endif //DINOCTOPUS_2040_MIDI_ROUTER_HPP

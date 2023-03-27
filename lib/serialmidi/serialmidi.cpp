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

#include "hardware/uart.h"
#include "serialmidi.hpp"

BEGIN_MIDI_NAMESPACE
    void MySerialMIDI::begin() const {
        uart_init(uart, SERIAL_BAUD);

        // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
        gpio_set_function(rxpin, GPIO_FUNC_UART);
        gpio_set_function(txpin, GPIO_FUNC_UART);
    }

    void MySerialMIDI::end() {
    }

    bool MySerialMIDI::beginTransmission(MidiType) {
        return true;
    }

    void MySerialMIDI::write(byte value) {
        uart_putc(uart, value);
    }

    void MySerialMIDI::endTransmission() {
    }

    byte MySerialMIDI::read() {
        return uart_getc(uart);
    }

    unsigned MySerialMIDI::available() {
        return uart_is_readable(uart);
    }

END_MIDI_NAMESPACE

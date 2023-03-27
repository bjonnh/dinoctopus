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

#ifndef DINOCTOPUS_2040_PIOMIDI_HPP
#define DINOCTOPUS_2040_PIOMIDI_HPP

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "midi_Defs.h"

#define PIO_SERIAL_MIDI(number, tx_pin, rx_pin) SerialPIO pio_serial##number(tx_pin, rx_pin, 32); \
    MIDI_NAMESPACE::PioMIDI pio_midi##number(pio_serial##number); \
    MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::PioMIDI> MIDI_IF_##number(pio_midi##number);

#define SERIAL_BAUD 31250
#pragma once
BEGIN_MIDI_NAMESPACE
    class PioMIDI {
    public:
        explicit PioMIDI(const SerialPIO &serialPio)
                : serialPio(serialPio) {
        };

        static const bool thruActivated = false;

        void begin();

        void end();

        static bool beginTransmission(midi::MidiType);

        void write(byte value);

        void endTransmission();

        byte read();

        unsigned available();

    private:
        SerialPIO serialPio;
    };
END_MIDI_NAMESPACE
#endif //DINOCTOPUS_2040_PIOMIDI_HPP

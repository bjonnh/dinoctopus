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

#ifndef DINOCTOPUS_2040_SERIAL_MIDI_HPP
#define DINOCTOPUS_2040_SERIAL_MIDI_HPP

#include <Arduino.h>
#include "midi_Namespace.h"
#include "hardware/uart.h"
#include "midi_Defs.h"


#define SERIAL_BAUD 31250

BEGIN_MIDI_NAMESPACE
    class MySerialMIDI {
    public:
        MySerialMIDI(uint rxpin, uint txpin)
                : rxpin(rxpin), txpin(txpin) {
        };

    public:
        static const bool thruActivated = false;

        void begin() const;

        void end();

        static bool beginTransmission(MidiType);

        static void write(byte value);

        void endTransmission();

        static byte read();

        static unsigned available();

    private:
        uint rxpin;
        uint txpin;
    };
END_MIDI_NAMESPACE


#endif //DINOCTOPUS_2040_PIOMIDI_HPP

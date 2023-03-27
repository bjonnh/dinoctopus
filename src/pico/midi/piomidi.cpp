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

#include "midi/piomidi.hpp"

BEGIN_MIDI_NAMESPACE
    void PioMIDI::begin() {
        serialPio.begin(SERIAL_BAUD);
    }

    void PioMIDI::end() {
    }

    bool PioMIDI::beginTransmission(midi::MidiType) {
        return true;
    }

    void PioMIDI::write(byte value) {
        serialPio.write(value);
    }

    void PioMIDI::endTransmission() {
    }

    byte PioMIDI::read() {
        return serialPio.read();
    }

    unsigned PioMIDI::available() {
        return serialPio.available();
    }
END_MIDI_NAMESPACE

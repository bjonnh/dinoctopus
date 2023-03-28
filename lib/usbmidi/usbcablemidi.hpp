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

#ifndef DINOCTOPUS_2040_USB_CABLE_MIDI_HPP
#define DINOCTOPUS_2040_USB_CABLE_MIDI_HPP

#include <midi/Adafruit_USBD_MIDI.h>
#include <utility>
#include "midi_Defs.h"

typedef struct {
    uint8_t buffer[4];
    uint8_t index;
    uint8_t total;
} midid_stream_t;

BEGIN_MIDI_NAMESPACE
    class UsbJackMIDI {
    public:
        explicit UsbJackMIDI(Adafruit_USBD_MIDI usb_midi, int cable)
                : usb_midi(std::move(usb_midi)), cable(cable) {  // I don't think it is legal to move it but why not
        };

    public:
        static const bool thruActivated = false;

        void begin();

        void end();

        static bool beginTransmission(midi::MidiType);

        void write(uint8_t value) const;

        void endTransmission();

        uint32_t tud_midi_n_stream_read_cable_aware(void *buffer, uint32_t bufsize);

        byte read();

        uint32_t tud_midi_n_available();

        unsigned available();

        [[nodiscard]] uint8_t current_cable() const;

    private:
        Adafruit_USBD_MIDI usb_midi;
        uint8_t cable = 0;
        midid_stream_t stream_read = {0};
    };

END_MIDI_NAMESPACE
#endif //DINOCTOPUS_2040_USB_CABLE_MIDI_HPP

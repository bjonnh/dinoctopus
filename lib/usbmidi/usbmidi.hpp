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

#ifndef DINOCTOPUS_2040_USBMIDI_HPP
#define DINOCTOPUS_2040_USBMIDI_HPP

#include <Arduino.h>
#include "Adafruit_TinyUSB.h"
#include "MIDI.h"
#include "midi_Defs.h"
#include <midi/Adafruit_USBD_MIDI.h>
#include "usbcablemidi.hpp"

#define USB_MIDI(n) midi::UsbJackMIDI jack_##n = midi::UsbJackMIDI(usb_midi, n-1); \
    midi::MidiInterface<midi::UsbJackMIDI> MIDI_USB_##n = midi::MidiInterface<midi::UsbJackMIDI>(jack_##n);

class UsbMidi {
public:
    void init() {
        TinyUSB_Device_Init(0);
    }
    void wait() {
        while (!TinyUSBDevice.mounted())
            delay(1);
    }

    uint8_t current_cable_limited() {
        uint8_t cable = jack_1.current_cable();
        if (cable<1 | cable>3)
            cable = 0;
        return cable;
    }

    midi::MidiInterface<midi::UsbJackMIDI> &operator()(uint8_t cable) {
        switch(cable) {
            case 2:
                return MIDI_USB_2;
            case 3:
                return MIDI_USB_3;
            case 4:
                return MIDI_USB_4;
            default:
                return MIDI_USB_1;
        }
    };

    Adafruit_USBD_MIDI usb_midi = Adafruit_USBD_MIDI(4);
    USB_MIDI(1);
    USB_MIDI(2);
    USB_MIDI(3);
    USB_MIDI(4);
};

#endif //DINOCTOPUS_2040_USBMIDI_HPP

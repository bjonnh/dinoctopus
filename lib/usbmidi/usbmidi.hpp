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

#include "Adafruit_TinyUSB.h"
#include "MIDI.h"
#include "midi_Defs.h"
#include <midi/Adafruit_USBD_MIDI.h>
#include "usbcablemidi.hpp"

#define USB_MIDI(n) midi::UsbJackMIDI jack_##n = midi::UsbJackMIDI(usb_midi, n-1); \
    midi::MidiInterface<midi::UsbJackMIDI> MIDI_USB_##n = midi::MidiInterface<midi::UsbJackMIDI>(jack_##n);

class UsbMidi {
public:
    void init();
    bool active();

    uint8_t current_cable_limited();

    midi::MidiInterface<midi::UsbJackMIDI> &operator()(uint8_t cable);;

    Adafruit_USBD_MIDI usb_midi = Adafruit_USBD_MIDI(4);
    USB_MIDI(1);
    USB_MIDI(2);
    USB_MIDI(3);
    USB_MIDI(4);


};

#endif //DINOCTOPUS_2040_USBMIDI_HPP

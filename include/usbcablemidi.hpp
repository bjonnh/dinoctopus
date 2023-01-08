//
// Created by bjo on 1/7/23.
//

#ifndef DINOCTOPUS_2040_USBMIDIJACK_HPP
#define DINOCTOPUS_2040_USBMIDIJACK_HPP


#include <midi_Namespace.h>
#include <midi/Adafruit_USBD_MIDI.h>
#include "hardware/pio.h"
#include "uart_rx.pio.h"
#include "uart_tx.pio.h"
#include "midi_Defs.h"


#define SERIAL_BAUD 31250

BEGIN_MIDI_NAMESPACE
class UsbJackMIDI
{
public:
    UsbJackMIDI(Adafruit_USBD_MIDI usb_midi, uint8_t jack)
            : usb_midi(usb_midi), jack(jack)
    {
    };

public:
    static const bool thruActivated = true;

    void begin()
    {
        usb_midi.begin();
    }

    void end()
    {
    }

    bool beginTransmission(MidiType)
    {
        return true;
    };

    void write(byte value)
    {
        usb_midi.write_cable(jack, value);
    };

    void endTransmission()
    {
    };

    byte read()
    {
        uint32_t val = usb_midi.read_cable(jack);
        cable = val >> 16;
        return val;
    };

    unsigned available()
    {
        return usb_midi.available();
    };
public:
    uint8_t cable;
private:
    Adafruit_USBD_MIDI usb_midi;
    uint8_t jack;
};
END_MIDI_NAMESPACE



#endif //DINOCTOPUS_2040_USBMIDIJACK_HPP

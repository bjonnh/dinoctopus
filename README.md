# Dinoctopus
[![Build documentation](https://github.com/bjonnh/dinoctopus/actions/workflows/doc.yml/badge.svg)](https://github.com/bjonnh/dinoctopus/actions/workflows/doc.yml) [![PlatformIO CI](https://github.com/bjonnh/dinoctopus/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/bjonnh/dinoctopus/actions/workflows/build_and_test.yml)

[Official documentation](https://bjonnh.github.io/dinoctopus/)


This is a RP2040 based USB-Midi interface that also allows for Midi merging and other MIDI tools
(coming, I'm porting them from an Arduino Mega).

It is currently still highly experimental (but works well for me) and comes with no support.

The noticeable things here is that:
- MIDI interfaces are using the PIO state machines. We can have up to 8 MIDI I or O (or 4 duplex ports)
- Up to 16 virtual USB interfaces (duplex) that allow connecting things together
- (not included) 2 additional physical duplex ports with the RP2040 UARTs (trivial to add)

That's 22 MIDI interfaces (6 physical) on a $4 microcontroller.

I've not stressed tested it yet, but it handles normal playing with multiple controllers so far.

It works out of the box on Linux with no driver installation.
I do not have a Windows or MacOS machine to test on them.

## Building

I'm using [Jetbrains Clion](https://www.jetbrains.com/clion/) and [PlatformIO](https://platformio.org/) to code and upload it to my board
using a [picoprobe](https://github.com/raspberrypi/picoprobe) programmer.

This should probably get you started:
```shell
pio project init
pio run -e pico -t upload
```

Most of the configuration you care about is in include/config.hpp


It used to have a UI, but I switched to a pure SysEX approach, that may have a web UI to set it or just a tiny python program.

### SysEX messages

This is the structure of the messages:
```
F0 44 49 4E 4F <command> [optional_data] F7
```

The commands are:

| Command | Description                   | Data                                                                |
|---------|-------------------------------|---------------------------------------------------------------------|   
| 0x00    | Identity (Not implemented)    |                                                                     |     
| 0x01    | Dump the matrix               | None                                                                |
| 0x02    | Set the matrix                | 100 uint32/7 numbers (see below the format)                         |   
| 0x03    | Get the matrix element at i,j | 2 uint8 between 0 and 5 for the input and output                    |  
| 0x04    | Set the matrix element at i,j | 3 uint8 between 0 and 5 for the input, output and value as uint32/7 | 
| 0x05    | Save in the EEPROM            | None                                                                |  
| 0x06    | Load from the EEPROM          | None                                                                |
| 0x07    | Reset everything to 0         | None                                                                |

All commands will return either : 0x60 (ACK) or 0x61 (NACK) except for 0x01 and 0x03 that are special.

Technically there is no EEPROM on the RP2040, but I use the flash for that. It is not supposed to be written too often
so don't hammer it thousands of times.

Any command that do not match the above will just be forwared to the port.

#### Dump the matrix (0x01)

This returns 100 uint32/7 numbers. You can safely convert them to uint32.

#### Get the element (0x03)

This returns one uint32/7 number that can be safely converted to uint32.

#### What is uint32/7:

Currently the values are stored internaly as an uint32_t, and we only use 0 and 1. The other bits will be used for filtering and other
features. I call the type uint32/7 because SysEX only allow to send 7 bits per element, so we end up with truncated values.
They are LSB.
So 1 is :
1 0 0 0 

250 cannot be represented

But 256 can:
0 1 0 0

Etc.

I will probably send the sysex differently at some point or just ignore that 1 bit, we will see when we get there.

## Testing

I made a tiny Python tool to test the SYSEX commands that could help you configure it:

```
virtualenv venv
source venv/bin/activate
pip install mido
python3  test_sysex.py
```

Careful this will erase your matrix and set it as a diagonal (so the physical ports are replicated to their matching number).

## Thanks

[DiyElectroMusic](https://diyelectromusic.wordpress.com) - About the idea of using PIOs for MIDI

[Ha Thach](https://github.com/hathach/tinyusb) - For TinyUSB (I had to extract some code from there to get virtual cable workings)

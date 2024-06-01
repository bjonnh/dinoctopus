# Dinoctopus
[![PlatformIO CI](https://github.com/bjonnh/dinoctopus/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/bjonnh/dinoctopus/actions/workflows/build_and_test.yml)

[Official documentation](https://www.bjonnh.net/project/dinoctopus/)


This is a RP2040 based USB-Midi interface that also allows for Midi merging and other MIDI tools
(coming, I'm porting them from an Arduino Mega).

It is currently still highly experimental (but works well for me) and comes with no support.

The noticeable things here is that:
- MIDI interfaces are using the PIO state machines. We can have up to 8 MIDI I or O (or 4 duplex ports)
- Up to 16 virtual USB interfaces (duplex) that allow connecting things together
- 2 additional physical duplex ports with the RP2040 UARTs (only one is setup currently)

That's 22 duplex MIDI interfaces (6 physical) on a $4 board ($1 microcontroller if you like soldering small things).

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


## Testing

I made a tiny Python tool to test the SYSEX commands that could help you configure it:

```
virtualenv venv
source venv/bin/activate
pip install -r requirements.txt
python3  test_sysex.py
```

Careful this will erase your matrix and set it as a diagonal (so the physical ports are replicated to their matching number).

## Thanks

[DiyElectroMusic](https://diyelectromusic.wordpress.com) - About the idea of using PIOs for MIDI

[Ha Thach](https://github.com/hathach/tinyusb) - For TinyUSB (I had to extract some code from there to get virtual cable workings)

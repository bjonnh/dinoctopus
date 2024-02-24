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

#ifndef DINOCTOPUS_2040_CONFIG_HPP
#define DINOCTOPUS_2040_CONFIG_HPP

#define NUMBER_OF_PHYSICAL_MIDI_PORTS 5

#define PIN_MIDI_1_RX 10
#define PIN_MIDI_1_TX 22

#define PIN_MIDI_2_RX 11
#define PIN_MIDI_2_TX 21

#define PIN_MIDI_3_RX 12
#define PIN_MIDI_3_TX 20

#define PIN_MIDI_4_RX 13
#define PIN_MIDI_4_TX 19

#define PIN_MIDI_5_RX 1
#define PIN_MIDI_5_TX 0

#define N_LOOPS_MAX_LATENCY_CORE0 10000
#define N_LOOPS_MAX_LATENCY_CORE1 100

#define SYSEX_COMMAND_PREFIX {0x44, 0x49, 0x4e, 0x4f}
#define SYSEX_DEVICE_ID 0x00
#define SYSEX_COMMAND_PREFIX_LENGTH 4

#endif //DINOCTOPUS_2040_CONFIG_HPP

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

//
// LCD
//

#define BUZZER_PIN 6

#define NEOPIXEL_PIN 9

#define LCD_CS  5    // LCD CS, sometimes called EN or SS
#define LCD_RS 7   // LCD RS, sometimes called A0 or DC
#define LCD_CLOCK 2 // Clock (Common), sometimes called SCK or SCL real pin
#define LCD_MOSI 3 // MOSI (common), sometimes called SDA or DATA
#define LCD_RESET 8  // LCD reset, sometimes called RST or RSTBb


//
// MIDI
//

#define NUMBER_OF_PHYSICAL_MIDI_PORTS 4

#define PIN_MIDI_1_RX 12
#define PIN_MIDI_1_TX 13

#define PIN_MIDI_2_RX 14
#define PIN_MIDI_2_TX 15

#define PIN_MIDI_3_RX 16
#define PIN_MIDI_3_TX 17

#define PIN_MIDI_4_RX 18
#define PIN_MIDI_4_TX 19

#define ENC_A 20
#define ENC_B 21
#define ENC_C 22

#define N_LOOPS_MAX_LATENCY_CORE0 10000
#define N_LOOPS_MAX_LATENCY_CORE1 100


#endif //DINOCTOPUS_2040_CONFIG_HPP

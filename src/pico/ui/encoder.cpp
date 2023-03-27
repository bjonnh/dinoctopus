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

#include <Arduino.h>
#include <pico/time.h>
#include "ui/manager.hpp"
#include "config.hpp"
#include "utils_rp2040.hpp"

// https://forum.arduino.cc/t/reading-rotary-encoders-as-a-state-machine/937388
void UI::Manager::readEncoder() {
    static uint8_t state = 0;
    bool CLKstate = digitalRead(ENC_C);
    bool DTstate = digitalRead(ENC_B);
    switch (state) {
        // Start state
        case 0:
            if (!CLKstate) {             // Turn clockwise and CLK goes low first
                state = 1;
            } else if (!DTstate) {      // Turn anticlockwise and DT goes low first
                state = 4;
            }
            break;
            // Clockwise rotation
        case 1:
            if (!DTstate) {             // Continue clockwise and DT will go low after CLK
                state = 2;
            }
            break;
        case 2:
            if (CLKstate) {             // Turn further and CLK will go high first
                state = 3;
            }
            break;
        case 3:
            if (CLKstate && DTstate) {  // Both CLK and DT now high as the encoder completes one step clockwise
                state = 0;
                encoder_left();
            }
            break;
            // Anticlockwise rotation
        case 4:                         // As for clockwise but with CLK and DT reversed
            if (!CLKstate) {
                state = 5;
            }
            break;
        case 5:
            if (DTstate) {
                state = 6;
            }
            break;
        case 6:
            if (CLKstate && DTstate) {
                state = 0;
                encoder_right();
            }
            break;
        default:
            state = 0;
            break;
    }
}

unsigned long time_a = to_ms_since_boot(get_absolute_time());
int delayTime_a = 200;

void UI::Manager::initEncoder() {
    pinMode(ENC_A, INPUT_PULLUP);
    pinMode(ENC_B, INPUT_PULLUP);
    pinMode(ENC_C, INPUT_PULLUP);
}


bool clicked = false;

void UI::Manager::encoderPoll() {
    bool click = digitalRead(ENC_A);
    if ((click == 0) and !clicked) {
        if ((CURRENT_TIME_MS - time_a) > delayTime_a) {
            time_a = CURRENT_TIME_MS;
            encoder_click();
            clicked = true;
        }
    } else {
        clicked = (click == 0);
    }
    readEncoder();
}

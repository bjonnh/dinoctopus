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

//
// Created by bjo on 4/2/23.
//
#include "simpleneopixel.hpp"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include <Arduino.h>

void SimpleNeoPixel::show() {
    noInterrupts();
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Set period of 4 cycles (0 to 3 inclusive)
    pwm_set_wrap(slice_num, 166);
    // Set channel A output high for one cycle before dropping
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(pin), 106);
    systick_hw->cvr = 0;
    pwm_set_enabled(slice_num, true);
    //while (systick_hw->cvr > 0x00FFFFFF-7560) {};
    delayMicroseconds(90);
    pwm_set_enabled(slice_num, false);
    gpio_put(pin, 0); // PWM doesn't always put back at 0

    /*
    systick_hw->csr = 0x5;
    systick_hw->rvr = 0x00FFFFFF;
    int time_up, time_down;
    int delay_corr = 45;
    gpio_put(pin, 0);
    delayMicroseconds(50);
    uint32_t leds[3]{
//        0xFFFFFFFF,
//        0xFFFFFFFF,
//        0xFFFFFFFF
            0x00000000,
            0x00000000,
            0x00000000
    };
    // This requires a lot of manual tuning, this would be great in pure assembly

    uint32_t t1h = 16777215 + delay_corr - 138; // roughly 0.8us at 133MHz
    uint32_t t1l = 16777215 + delay_corr - 80; // roughly 0.45us at 133MHz

    uint32_t t0h = 16777215 + delay_corr - 85;  // roughly 0.4us   ok
    uint32_t t0l = 16777215 + delay_corr - 126; // roughly 0.85us

    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 24; i++) {
            if ((leds[j] & (1 << i)) >> i) {
                gpio_put(pin, 1);
                systick_hw->cvr = 0;
                while (systick_hw->cvr > t1h) {};
                gpio_put(pin, 0);
                systick_hw->cvr = 0;
                while (systick_hw->cvr > t1l) {};
            } else {
                gpio_put(pin, 1);
                systick_hw->cvr = 0;
                while (systick_hw->cvr > t0h) {};
                gpio_put(pin, 0);
                systick_hw->cvr = 0;
                while (systick_hw->cvr > t0l) {};
            }
        }
    }
    gpio_put(pin, 0);
    delayMicroseconds(50);
     */
    interrupts();
}

void SimpleNeoPixel::begin() {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, 0);
}

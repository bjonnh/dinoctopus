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
#include "hardware/sync.h"
#include "pico/stdlib.h"

/**
 * This is EXTREMELY simple and stupid. We just send all 1s to the leds so they are fully on
 * This cannot do color or brightness. We can eventually turn some of the leds down
 */
void SimpleNeoPixel::white() {
    pwm_set_wrap(slice_num, 166);
    pwm_set_chan_level(slice_num, channel, 106); // 106 for on, 53 for off
    pwm_set_enabled(slice_num, true);
    uint32_t ints = save_and_disable_interrupts();
    busy_wait_us(90); // Doesn't matter if we go over we have only 3 leds
    restore_interrupts(ints);
    pwm_set_enabled(slice_num, false);
    gpio_put(pin, 0); // PWM doesn't always put back at 0
}

SimpleNeoPixel::SimpleNeoPixel(int pin) : pin(pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(pin);
    channel = pwm_gpio_to_channel(pin);
    gpio_put(pin, 0);
}

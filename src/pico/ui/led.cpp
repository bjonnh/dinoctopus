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

#include "ui/manager.hpp"
#include "Adafruit_NeoPixel.h"
#include "config.hpp"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);
#define C(r,g,b) Adafruit_NeoPixel::Color(r,g,b)

void UI::Manager::initStrip() {
    strip.begin();
    strip.setBrightness(255);
    strip.setPixelColor(0, C(255, 25, 0));
    strip.setPixelColor(1, C(255, 25, 0));
    strip.setPixelColor(2, C(255, 25, 0));
    strip.show();
    normalStrip();
}

[[maybe_unused]] void UI::Manager::errorStrip() {
    strip.setBrightness(255);
    strip.setPixelColor(0, C(255, 1, 85));
    strip.setPixelColor(1, C(255, 1, 145));
    strip.setPixelColor(2, C(255, 85, 45));
    strip.show();
}

void UI::Manager::normalStrip() {
    strip.setBrightness(255);
    strip.setPixelColor(0, C(1, 255, 85));
    strip.setPixelColor(1, C(1, 255, 145));
    strip.setPixelColor(2, C(15, 255, 45));
    strip.show();
}

void UI::Manager::dirtyStrip() {
    strip.setBrightness(255);
    strip.setPixelColor(0, C(151, 255, 85));
    strip.setPixelColor(1, C(151, 255, 145));
    strip.setPixelColor(2, C(155, 205, 45));
    strip.show();
}

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
#include "config.hpp"

// Turns out, the adafruit library steals a PIO, that's why one of the MIDI ports was not working!
// this is highly specific to the LEDs on the display I'm using. Use the Adafruit library if you don't know what
// you are doing, but be warned that it will take a PIO.

// All that code is adapted from the Adafruit NeoPixel library code and doesn't support much of it
// You should probably never rely on it, but I need those PIOs
#define NEO_RGB ((0 << 6) | (0 << 4) | (1 << 2) | (2)) ///< Transmit as R,G,B

class SimpleNeoPixel {  // TODO: GPL License as it has code from Adafruit NeoPixel
    // Type was NEO_RGB + NEO_KHZ800
public:
    SimpleNeoPixel(int n, int pin) : begun(false), brightness(0), pixels(nullptr), pin(pin),
                                     rOffset(1), gOffset(0), bOffset(2) {
        updateLength(n);
    };

    void updateLength(uint16_t n) {
        free(pixels); // Free existing data (if any)

        // Allocate new data -- note: ALL PIXELS ARE CLEARED
        numBytes = n * 3;
        if ((pixels = (uint8_t *)malloc(numBytes))) {
            memset(pixels, 0, numBytes);
            numLEDs = n;
        } else {
            numLEDs = numBytes = 0;
        }
    }

    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }

    void begin() {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, 0);
        begun = true;
    }

    void setBrightness(uint8_t b) {
        // Stored brightness value is different than what's passed.
        // This simplifies the actual scaling math later, allowing a fast
        // 8x8-bit multiply and taking the MSB. 'brightness' is a uint8_t,
        // adding 1 here may (intentionally) roll over...so 0 = max brightness
        // (color values are interpreted literally; no scaling), 1 = min
        // brightness (off), 255 = just below max brightness.
        uint8_t newBrightness = b + 1;
        if (newBrightness != brightness) { // Compare against prior value
            // Brightness has changed -- re-scale existing data in RAM,
            // This process is potentially "lossy," especially when increasing
            // brightness. The tight timing in the WS2811/WS2812 code means there
            // aren't enough free cycles to perform this scaling on the fly as data
            // is issued. So we make a pass through the existing color data in RAM
            // and scale it (subsequent graphics commands also work at this
            // brightness level). If there's a significant step up in brightness,
            // the limited number of steps (quantization) in the old data will be
            // quite visible in the re-scaled version. For a non-destructive
            // change, you'll need to re-render the full strip data. C'est la vie.
            uint8_t c, *ptr = pixels,
                    oldBrightness = brightness - 1; // De-wrap old brightness value
            uint16_t scale;
            if (oldBrightness == 0)
                scale = 0; // Avoid /0
            else if (b == 255)
                scale = 65535 / oldBrightness;
            else
                scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
            for (uint16_t i = 0; i < numBytes; i++) {
                c = *ptr;
                *ptr++ = (c * scale) >> 8;
            }
            brightness = newBrightness;
        }
    }

    void setPixelColor(int i, uint32_t c) {
        if (n < numLEDs) {
            uint8_t *p, r = (uint8_t)(c >> 16), g = (uint8_t)(c >> 8), b = (uint8_t)c;
            if (brightness) { // See notes in setBrightness()
                r = (r * brightness) >> 8;
                g = (g * brightness) >> 8;
                b = (b * brightness) >> 8;
            }

            p = &pixels[n * 3];
            p[rOffset] = r;
            p[gOffset] = g;
            p[bOffset] = b;
        }
    }

    void show() {
        if (!pixels)
            return;

        noInterrupts();
        systick_hw->csr = 0x5;
        systick_hw->rvr = 0x00FFFFFF;
        int time_up, time_down;
        int delay_corr=45;
        for (int i=0; i<24*3;i++) {
            if (i>8) {
                time_up = 16777215+delay_corr-106; // 104 cycles roughly 0.8us at 133MHz
                time_down = 16777215+delay_corr-59; // 59 cycles roughly 0.45us at 133MHz
            } else {
                time_up = 16777215 + delay_corr- 53;
                time_down = 16777215 +delay_corr- 113;
            }

            // lets try to send just ones first
            gpio_put(pin, 1);
            // Inspired by https://forums.raspberrypi.com/viewtopic.php?t=333928

            systick_hw->cvr=0;while(systick_hw->cvr>time_up){};
            gpio_put(pin, 0);
            systick_hw->cvr=0;while(systick_hw->cvr>time_down){};
        }
        gpio_put(pin, 0);
        // Now a roughly 50us delay
        delayMicroseconds(300);
        interrupts();
    }

private:
    int n;
    int pin;
    bool begun;
    uint8_t brightness;
    uint8_t *pixels;
    uint16_t numBytes;
    uint16_t numLEDs;
    int rOffset;
    int gOffset;
    int bOffset;
};

class Color {
private:
    int r;
    int g;
    int b;
};


SimpleNeoPixel strip = SimpleNeoPixel(3, NEOPIXEL_PIN);
#define C(r,g,b) SimpleNeoPixel::Color(r,g,b)

void UI::Manager::initStrip() {
    strip.begin();
    strip.setBrightness(255);
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

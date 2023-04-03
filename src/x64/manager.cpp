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
#include "U8g2lib.h"
#include "u8g2.h"
#include <iostream>

void UI::Manager::loop() {
    int k;
    while (true) {
        std::cout << "running a loop" << std::endl;

        display_update();
        do {
            k = u8g_sdl_get_key();
        } while (k < 0);

        if (k == 276) // left
            encoder_left();
        else if (k == 275) // right
            encoder_right();
        else if (k == 0) // enter
            encoder_click();
        else if (k == 113) // q
            return;
        else
            std::cout << "Unhandled key: " << k << std::endl;
    }
}

void UI::Manager::initHardwareLCD() {}

void UI::Manager::initEncoder() {}

// TODO Use another window for those

void UI::Manager::initStrip() {}

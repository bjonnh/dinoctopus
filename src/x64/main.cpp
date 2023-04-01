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

#include <iostream>
#include "ui/manager.hpp"


u8g2_t local_lcd;

class SDL_U8G2 : public U8G2 {
public: SDL_U8G2(): U8G2() {
        u8g2 = local_lcd;
        u8g2_SetupBuffer_SDL_128x64(&u8g2, &u8g2_cb_r0);
        u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
        u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);
    }
};

SDL_U8G2 u8g2_lcd;
UI::Manager ui(reinterpret_cast<U8G2 &>(u8g2_lcd));

int main(void)
{
    std::cout << "UI Starting" << std::endl;
    ui.init();
    std::cout << "UI Initialized" << std::endl;
    ui.loop();
    std::cout << "UI Finished" << std::endl;
}

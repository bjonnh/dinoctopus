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

#include "../../include/ui/horizontalmenu.hpp"
#include "ui/ui.hpp"
#include "U8g2lib.h"

namespace UI::Widgets {
    template<>
    void Horizontal_menu<U8G2>::draw() {
        if (!visible)
            return;

        display->setFont(u8g2_font_5x8_mf);
        display->setDrawColor(1);

        for (uint8_t i = 0; i < inserted_items; i++) {
            uint8_t shift_x = 48 * i;
            if (current_item == i) {
                if (focus)
                    display->setDrawColor(0);
                else
                    display->drawHLine(shift_x, UI_LINE_HEIGHT + 1, 50);
            }
            if (!focus & (selected_item == i))
                snprintf(buffer, 50, ">%s", items[i]);
            else
                snprintf(buffer, 50, " %s", items[i]);
            display->drawStr(shift_x, UI_LINE_HEIGHT, buffer);
            display->setDrawColor(1);
        }
    }
} // Widgets

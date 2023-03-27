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

#include "ui/matrix.hpp"
#include "ui/ui.hpp"
#include "U8g2lib.h"

namespace UI::Widgets {
    template
    class Matrix<U8G2>;

    char matrix_sign(uint32_t v) {
        return v > 0 ? 'X' : '.';
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"  // nope they are exactly what they should be

    template<>
    void Matrix<U8G2>::draw() {
        if (!visible)
            return;

        display->setFont(u8g2_font_5x8_mf);
        if (focus) {
            display->setDrawColor(state != back);
            display->drawStr(0, 2.5 * UI_LINE_HEIGHT, "<");
        }
        display->setDrawColor(1);
        display->drawStr(5, 2.5 * UI_LINE_HEIGHT, " IO|   1   2   3   4 ");
        display->drawHLine(10, 2.5 * UI_LINE_HEIGHT, 100);
        for (uint8_t i = 0; i<4; i++) {
            if (((state == input_select) or (state == route_select)) && (p0_selected_input == i + 1)) {
                display->setDrawColor(0);
            }
            else {
                display->setDrawColor(1);
            }
            snprintf(buffer, 3, "%d", i + 1);
            display->drawStr(10, (3.5 + i) * UI_LINE_HEIGHT, buffer);
            display->setDrawColor(1);
            display->drawStr(20, (3.5 + i) * UI_LINE_HEIGHT, "|");
            for (uint8_t j = 0; j < 4; j++) {
                if ((state == route_select) && (p0_selected_output == j + 1) && (p0_selected_input == i + 1))
                    display->setDrawColor(0);
                else
                    display->setDrawColor(1);
                snprintf(buffer, 50, "   %c", matrix_sign(current_matrix.get_element_2d(i,j)));
                display->drawStr(25 + 20 * j, (3.5 + i) * UI_LINE_HEIGHT, buffer);
            }
        }
        display->setDrawColor(1);
        if (focus)
            display->drawFrame(0, 9, 128, 64 - 10);
    }
}

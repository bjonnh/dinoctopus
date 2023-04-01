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

#ifndef DINOCTOPUS_2040_STATUS_BAR_HPP
#define DINOCTOPUS_2040_STATUS_BAR_HPP


#include "widget.hpp"
#include "ui.hpp"

namespace UI::Widgets {
    template<typename D>
    class StatusBar : public UI::Widget<D> {
        char buffer[UI_CHARACTERS_PER_LINE] = {0};
    public:
        explicit StatusBar(Widget<D> &parent) : Widget<D>(parent) {}

        void draw() override;

        void set_message(const char string[UI_CHARACTERS_PER_LINE]);
    };
}

#endif //DINOCTOPUS_2040_STATUS_BAR_HPP

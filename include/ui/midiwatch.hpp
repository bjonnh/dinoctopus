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

#ifndef DINOCTOPUS_2040_MIDIWATCH_HPP
#define DINOCTOPUS_2040_MIDIWATCH_HPP

#include <cstdint>
#include "ui/widget.hpp"
#include "ui/ui.hpp"

namespace UI::Widgets {
    template<typename D>
    class MidiWatch: public UI::Widget<D> {
    public:
        explicit MidiWatch(Widget<D> &parent) : Widget<D>(parent) {}

        void draw() override;

        bool clickAction() override;

        bool leftAction() override;

        bool rightAction() override;

        char buffer[UI_CHARACTERS_PER_LINE] = {0};
    };
}

#endif //DINOCTOPUS_2040_MIDIWATCH_HPP

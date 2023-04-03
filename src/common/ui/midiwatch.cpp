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

#include "ui/midiwatch.hpp"
#include "ui/manager.hpp"


namespace UI::Widgets {
    template class MidiWatch<LCD_t>;

    template<> void MidiWatch<LCD_t>::draw() {
        if (!visible)
            return;
        display->drawStr(5, 2.5 * UI_LINE_HEIGHT, buffer);
    }

    template<typename D>
    bool MidiWatch<D>::clickAction() {
        if (!this->focus or !this->visible)
            return false;
        this->setFocus(false);
        this->exit();
        return true;
    }


    template<typename D>
    bool MidiWatch<D>::rightAction() {
        if (!this->focus or !this->visible)
            return false;
        return true;
    }

    template<typename D>
    bool MidiWatch<D>::leftAction() {
        if (!this->focus or !this->visible)
            return false;
        return true;
    }
}
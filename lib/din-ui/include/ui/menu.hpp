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


#ifndef DINOCTOPUS_2040_MENU_HPP
#define DINOCTOPUS_2040_MENU_HPP

#include <cstdint>
#include <cstring>
#include "widget.hpp"
#include "ui/ui.hpp"

#define MENU_ITEM_LENGTH 20
#define MENU_ITEM_NUMBERS 3

typedef char menu_item[MENU_ITEM_LENGTH];

namespace UI::Widgets {
    template<typename D>
    class Menu : public Widget<D> {

    public:
        explicit Menu(Widget<D> &parent) : Widget<D>(parent) {}

        bool addItem(char *item);

        void onHighlightedCall(void (*fun)(uint8_t));

        void onSelectedCall(void (*fun)(uint8_t));

        void set_highlighted_item_to(uint8_t new_item);

        void set_selected_item_to(uint8_t new_item);

    protected:
        char buffer[UI_CHARACTERS_PER_LINE] = {0};
        uint8_t inserted_items = 0;
        uint8_t selected_item = 0;
        menu_item items[MENU_ITEM_NUMBERS] = {""};
        uint8_t current_item = 0;

        void (*selected_callbacks[UI_WIDGET_CALLBACKS])(uint8_t) = {nullptr};

        uint8_t inserted_selected_callbacks = 0;

        void (*highlighted_callbacks[UI_WIDGET_CALLBACKS])(uint8_t) = {nullptr};

        uint8_t inserted_highlighted_callbacks = 0;

        bool clickAction() override;

        bool leftAction() override;

        bool rightAction() override;

    };
} // Widgets

#endif //DINOCTOPUS_2040_MENU_HPP

//
// Created by bjo on 3/17/23.
//

#ifndef DINOCTOPUS_2040_MENU_HPP
#define DINOCTOPUS_2040_MENU_HPP

#include <cstdint>
#include <cstring>
#include "ui/widget.hpp"
#include "ui.hpp"

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

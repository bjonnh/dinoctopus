//
// Created by bjo on 3/17/23.
//

#ifndef DINOCTOPUS_2040_HORIZONTALMENU_HPP
#define DINOCTOPUS_2040_HORIZONTALMENU_HPP

#include <cstdint>
#include <cstring>
#include "ui/widget.hpp"

#define HORIZONTAL_MENU_ITEM_LENGTH 20
#define HORIZONTAL_MENU_ITEM_NUMBERS 3

typedef char menu_item[HORIZONTAL_MENU_ITEM_LENGTH];

namespace UI::Widgets {
    class Horizontal_menu : public UI::Widget { ;

        bool click();
        bool move_left();
        bool move_right();

        void onSelectedCall(void (*fun)(uint8_t));

    public:
        Horizontal_menu(U8G2_ST7567_JLX12864_F_4W_HW_SPI *display, Widget *parent): Widget(display, parent) {}

        bool add_item(char *item);

        void draw();

    private:
        uint8_t inserted_items = 0;
        uint8_t current_item = 0;
        uint8_t selected_item = 0;
        menu_item items[HORIZONTAL_MENU_ITEM_NUMBERS] = { "" };
        void (*selected_callbacks[UI_WIDGET_CALLBACKS])(uint8_t) = {nullptr};
        uint8_t inserted_selected_callbacks=0;

        void (*highlighted_callbacks[UI_WIDGET_CALLBACKS])(uint8_t) = {nullptr};
        uint8_t inserted_highlighted_callbacks=0;

        void set_selected_item_to(uint8_t new_item);

        void set_highlighted_item_to(uint8_t new_item);

        void onHighlightedCall(void (*fun)(uint8_t));
    };
} // Widgets

#endif //DINOCTOPUS_2040_HORIZONTALMENU_HPP

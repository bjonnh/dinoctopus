//
// Created by bjo on 3/17/23.
//

#include "ui/horizontalmenu.hpp"
#include "ui/ui.hpp"

namespace UI {
    namespace Widgets {
        bool Horizontal_menu::addItem(char *item) {
            if (inserted_items < HORIZONTAL_MENU_ITEM_NUMBERS) {
                memcpy(items[inserted_items++], item, HORIZONTAL_MENU_ITEM_LENGTH);
                return true;
            }

            return false;
        }

        void Horizontal_menu::set_selected_item_to(uint8_t new_item) {
            selected_item = new_item;
            for (const auto &item: selected_callbacks)
                if(item!= nullptr)
                    (*item)(new_item);
        }

        void Horizontal_menu::set_highlighted_item_to(uint8_t new_item) {
            current_item = new_item;
            for (const auto &item: highlighted_callbacks)
                if(item!= nullptr)
                    (*item)(new_item);
        }

        bool Horizontal_menu::clickAction() {
            if (!visible or !focus)
                return false;
            set_selected_item_to(current_item);
            return true;
        }

        bool Horizontal_menu::leftAction() {
            if (current_item==0 or !visible or !focus)
                return false;

            set_highlighted_item_to(current_item-1);
            return true;
        }

        bool Horizontal_menu::rightAction() {
            if ((current_item>inserted_items-1) or !visible or !focus)
                return false;

            set_highlighted_item_to(current_item+1);
            return true;
        }

        void Horizontal_menu::onSelectedCall(void (*fun)(uint8_t)) {
            if(inserted_selected_callbacks<UI_WIDGET_CALLBACKS-1)
                selected_callbacks[inserted_selected_callbacks++] = fun;
        }

        void Horizontal_menu::onHighlightedCall(void (*fun)(uint8_t)) {
            if(inserted_highlighted_callbacks<UI_WIDGET_CALLBACKS-1)
                highlighted_callbacks[inserted_highlighted_callbacks++] = fun;
        }

        char buffer[50];

        void Horizontal_menu::draw() {
            if (!visible)
                return;

            display->setFont(u8g2_font_5x8_mf);
            display->setDrawColor(1);

            for (uint8_t i=0; i < inserted_items;i++) {
                uint8_t shift_x = 48 * i;
                if (current_item == i) {
                    if (focus)
                        display->setDrawColor(0);
                    else
                        display->drawHLine(shift_x, UI_LINE_HEIGHT + 1, 50);
                }
                if (selected_item == i)
                    snprintf(buffer, 50, ">%s", items[i]);
                else
                    snprintf(buffer, 50, " %s", items[i]);
                display->drawStr(shift_x, UI_LINE_HEIGHT, buffer);
                display->setDrawColor(1);
            }
        }
    } // Manager
} // Widgets

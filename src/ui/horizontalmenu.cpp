//
// Created by bjo on 3/17/23.
//

#include "ui/horizontalmenu.hpp"
#include "ui/ui.hpp"

namespace UI {
    namespace Widgets {
        bool Horizontal_menu::add_item(char *item) {
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

        bool Horizontal_menu::click() {
            if (!visible)
                return false;
            set_selected_item_to(current_item);
            UI::Widget::click();
            return true;
        }

        bool Horizontal_menu::move_left() {
            if (!visible)
                return false;
            if (current_item>0)
                current_item--;

            UI::Widget::move_left();
            return true;
        }

        bool Horizontal_menu::move_right() {
            if (!visible)
                return false;
            if (current_item<inserted_items-1)
                current_item++;
            UI::Widget::move_right();
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

        void Horizontal_menu::draw() {
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
                display->drawStr(shift_x, UI_LINE_HEIGHT, items[i]);
                display->setDrawColor(1);
            }
        }
    } // Manager
} // Widgets
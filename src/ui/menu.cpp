//
// Created by bjo on 3/17/23.
//

#include "ui/menu.hpp"
#include "ui/ui.hpp"

namespace UI {
    namespace Widgets {
        bool Menu::addItem(char *item) {
            if (inserted_items < MENU_ITEM_NUMBERS) {
                memcpy(items[inserted_items++], item, MENU_ITEM_LENGTH);
                return true;
            }
            return false;
        }

        void Menu::set_selected_item_to(uint8_t new_item) {
            selected_item = new_item;
            for (const auto &item: selected_callbacks)
                if(item!= nullptr)
                    (*item)(new_item);
        }

        void Menu::set_highlighted_item_to(uint8_t new_item) {
            current_item = new_item;
            for (const auto &item: highlighted_callbacks)
                if(item!= nullptr)
                    (*item)(new_item);
        }

        bool Menu::clickAction() {
            if (!visible or !focus)
                return false;
            set_selected_item_to(current_item);
            return true;
        }

        bool Menu::leftAction() {
            if (current_item==0 or !visible or !focus)
                return false;

            set_highlighted_item_to(current_item-1);
            return true;
        }

        bool Menu::rightAction() {
            if ((current_item>=inserted_items-1) or !visible or !focus)
                return false;

            set_highlighted_item_to(current_item+1);
            return true;
        }

        void Menu::onSelectedCall(void (*fun)(uint8_t)) {
            if(inserted_selected_callbacks<UI_WIDGET_CALLBACKS-1)
                selected_callbacks[inserted_selected_callbacks++] = fun;
        }

        void Menu::onHighlightedCall(void (*fun)(uint8_t)) {
            if(inserted_highlighted_callbacks<UI_WIDGET_CALLBACKS-1)
                highlighted_callbacks[inserted_highlighted_callbacks++] = fun;
        }

        void Menu::draw() {
            Widget::draw();
        }

        char buffer[50];
    } // Manager
} // Widgets

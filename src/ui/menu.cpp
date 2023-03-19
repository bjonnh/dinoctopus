//
// Created by bjo on 3/17/23.
//

#include "ui/menu.hpp"
#include "ui/ui.hpp"

namespace UI::Widgets {
    template class Menu<UI_TYPE>;

    template<typename D>
    bool Menu<D>::addItem(char *item) {
        if (inserted_items < MENU_ITEM_NUMBERS) {
            memcpy(items[inserted_items++], item, MENU_ITEM_LENGTH);
            return true;
        }
        return false;
    }

    template<typename D>
    void Menu<D>::set_selected_item_to(uint8_t new_item) {
        selected_item = new_item;
        for (const auto &item: selected_callbacks)
            if (item != nullptr)
                (*item)(new_item);
    }

    template<typename D>
    void Menu<D>::set_highlighted_item_to(uint8_t new_item) {
        current_item = new_item;
        for (const auto &item: highlighted_callbacks)
            if (item != nullptr)
                (*item)(new_item);
    }

    template<typename D>
    bool Menu<D>::clickAction() {
        if (!this->visible or !this->focus)
            return false;
        set_selected_item_to(current_item);
        return true;
    }

    template<typename D>
    bool Menu<D>::leftAction() {
        if (current_item == 0 or !this->visible or !this->focus)
            return false;

        set_highlighted_item_to(current_item - 1);
        return true;
    }

    template<typename D>
    bool Menu<D>::rightAction() {
        if ((current_item >= inserted_items - 1) or !this->visible or !this->focus)
            return false;

        set_highlighted_item_to(current_item + 1);
        return true;
    }

    template<typename D>
    void Menu<D>::onSelectedCall(void (*fun)(uint8_t)) {
        if (inserted_selected_callbacks < UI_WIDGET_CALLBACKS - 1)
            selected_callbacks[inserted_selected_callbacks++] = fun;
    }

    template<typename D>
    void Menu<D>::onHighlightedCall(void (*fun)(uint8_t)) {
        if (inserted_highlighted_callbacks < UI_WIDGET_CALLBACKS - 1)
            highlighted_callbacks[inserted_highlighted_callbacks++] = fun;
    }

    template<>
    void Menu<UI_TYPE>::draw() {
        Widget::draw();
    }
} // Widgets

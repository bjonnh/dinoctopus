//
// Created by bjo on 3/17/23.
//

#include "ui/widget.hpp"

namespace UI {
    Widget::Widget(U8G2_ST7567_JLX12864_F_4W_HW_SPI *d, Widget *p) {
        parent = p;
        display = d;

        p->add_child(this);
    }

    Widget::Widget(U8G2_ST7567_JLX12864_F_4W_HW_SPI *d) {
        display = d;
    }

    bool Widget::click() {
        if(visible)
            for (const auto &item: childs)
                if (item != nullptr) {
                    bool out = item->click();
                    if (out)
                        return true;
                }
        return false;
    }

    bool Widget::move_left() {
        if(visible)
            for (const auto &item: childs)
                if (item != nullptr) {
                    bool out = item->move_left();
                    if (out)
                        return true;
                }
        return false;
    }

    bool Widget::move_right() {
        if(visible)
            for (const auto &item: childs)
                if (item != nullptr) {
                    bool out = item->move_right();
                    if (out)
                        return true;
                }
        return false;
    }

    void Widget::onFocusCall(void (*fun)(bool)) {
        if (inserted_focus_callback<=2) {
            focus_callbacks[inserted_focus_callback++] = fun;
        }
    }

    void Widget::draw() {
        if (send_draw_to_childs)
            for (const auto &item: childs)
                if (item != nullptr)
                    item->draw();
    }

    void Widget::onVisibleCall(void (*fun)(bool)) {
        if (inserted_visible_callback<=2) {
            visible_callbacks[inserted_visible_callback++] = fun;
        }
    }

    void Widget::add_child(Widget *pWidget) {
        // We still allow to add them but that's on you
        // Same thing if you make a loop
        if (current_child<UI_WIDGET_MAX_CHILDS-1)
            childs[current_child++] = pWidget;
    }
} // Manager

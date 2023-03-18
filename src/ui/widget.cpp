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
            if (focus)
                if (clickAction())
                    return true;
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
            if (focus)
                if (leftAction())
                    return true;
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
            if (focus)
                if (rightAction())
                    return true;
            for (const auto &item: childs)
                if (item != nullptr) {
                    bool out = item->move_right();
                    if (out)
                        return true;
                }
        return false;
    }

    void Widget::onFocusCall(void (*fun)(bool)) {
        if (inserted_focus_callback<=UI_WIDGET_CALLBACKS) {
            focus_callbacks[inserted_focus_callback++] = fun;
        }
    }

    void Widget::onVisibleCall(void (*fun)(bool)) {
        if (inserted_visible_callback<=UI_WIDGET_CALLBACKS) {
            visible_callbacks[inserted_visible_callback++] = fun;
        }
    }

    void Widget::onExitCall(void (*fun)()) {
        if (inserted_exit_callback<=UI_WIDGET_CALLBACKS) {
            exit_callbacks[inserted_exit_callback++] = fun;
        }
    }

    void Widget::onUpdateCall(void (*fun)()) {
        if (inserted_update_callback<=UI_WIDGET_CALLBACKS) {
            update_callbacks[inserted_update_callback++] = fun;
        }
    }

    void Widget::draw() {
        if (send_draw_to_childs & visible)
            for (const auto &item: childs)
                if (item != nullptr)
                    item->draw();
    }

    void Widget::add_child(Widget *pWidget) {
        // We still allow to add them but that's on you
        // Same thing if you make a loop
        if (current_child<UI_WIDGET_MAX_CHILDS-1)
            childs[current_child++] = pWidget;
    }

    void Widget::setFocus(bool f) {
        focus = f;
        if (f)
            parent->focus = false;  // We do not call the function for that as it would enter an endless loop for pages
        for (const auto &item: focus_callbacks)
            if (item != nullptr)
                (*item)(f);
    }

    void Widget::exit() {
        for (const auto &item: exit_callbacks)
            if (item != nullptr)
                (*item)();
    }

    void Widget::update() {
        for (const auto &item: update_callbacks)
            if (item != nullptr)
                (*item)();
    }

    bool Widget::clickAction() {
        return false;
    }

    bool Widget::leftAction() {
        return false;
    }

    bool Widget::rightAction() {
        return false;
    }
} // Manager

//
// Created by bjo on 3/17/23.
//

#include "../include/ui/widget.hpp"
#include "U8g2lib.h"


namespace UI {
    template class Widget<U8G2>; // TODO Move that somewhere u8g2 specific if possible?

    template<typename D>
    Widget<D>::Widget(Widget<D> &p) {
        parent = &p;
        display = p.display;

        p.add_child(this);
    }

    template<typename D>
    Widget<D>::Widget(D &d) {
        display = &d;
    }

    template<typename D>
    Widget<D>::Widget() { }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

    template<typename D>
    bool Widget<D>::click() {
        if (visible)
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

#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

    template<typename D>
    bool Widget<D>::move_left() {
        if (visible)
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

#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

    template<typename D>
    bool Widget<D>::move_right() {
        if (visible)
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

#pragma clang diagnostic pop

    template<typename D>
    void Widget<D>::onFocusCall(void (*fun)(bool)) {
        if (inserted_focus_callback < UI_WIDGET_CALLBACKS) {
            focus_callbacks[inserted_focus_callback++] = fun;
        }
    }

    template<typename D>
    void Widget<D>::onVisibleCall(void (*fun)(bool)) {
        if (inserted_visible_callback < UI_WIDGET_CALLBACKS) {
            visible_callbacks[inserted_visible_callback++] = fun;
        }
    }

    template<typename D>
    void Widget<D>::onExitCall(void (*fun)()) {
        if (inserted_exit_callback < UI_WIDGET_CALLBACKS) {
            exit_callbacks[inserted_exit_callback++] = fun;
        }
    }

    template<typename D>
    void Widget<D>::onUpdateCall(void (*fun)()) {
        if (inserted_update_callback < UI_WIDGET_CALLBACKS) {
            update_callbacks[inserted_update_callback++] = fun;
        }
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

    template<typename D>
    void Widget<D>::draw() {
        if (visible)
            for (const auto &item: childs)
                if (item != nullptr)
                    item->draw();
    }

#pragma clang diagnostic pop

    template<typename D>
    void Widget<D>::add_child(Widget<D> *pWidget) {
        // We still allow to add them but that's on you
        // Same thing if you make a loop
        if (current_child < UI_WIDGET_MAX_CHILDS - 1)
            childs[current_child++] = pWidget;
    }

    template<typename D>
    void Widget<D>::setFocus(bool f) {
        focus = f;
        if (f)
            parent->focus = false;  // We do not call the function for that as it would enter an endless loop for pages
        for (const auto &item: focus_callbacks)
            if (item != nullptr)
                (*item)(f);
    }

    template<typename D>
    void Widget<D>::start() {

    }

    template<typename D>
    void Widget<D>::exit() {
        for (const auto &item: exit_callbacks)
            if (item != nullptr)
                (*item)();
    }

    template<typename D>
    void Widget<D>::update() {
        for (const auto &item: update_callbacks)
            if (item != nullptr)
                (*item)();
    }

    template<typename D>
    bool Widget<D>::clickAction() {
        return false;
    }

    template<typename D>
    bool Widget<D>::leftAction() {
        return false;
    }

    template<typename D>
    bool Widget<D>::rightAction() {
        return false;
    }

    template<typename D>
    bool Widget<D>::focused() const {
        return focus;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

    template<typename D>
    void Widget<D>::setVisible(bool v) {
        visible = v;
        for (const auto &item: visible_callbacks)
            if (item != nullptr)
                (*item)(v);
        for (const auto &item: childs)
            if (item != nullptr)
                item->setVisible(v);
    }

#pragma clang diagnostic pop
} // Manager

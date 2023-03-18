//
// Created by bjo on 3/17/23.
//

#ifndef DINOCTOPUS_2040_WIDGET_HPP
#define DINOCTOPUS_2040_WIDGET_HPP

#include <cstdint>
#include "U8g2lib.h"

#define UI_WIDGET_CALLBACKS 3
#define UI_WIDGET_MAX_CHILDS 8

namespace UI {
    class Widget {
    public:
        Widget(U8G2_ST7567_JLX12864_F_4W_HW_SPI *display, Widget *parent);

        explicit Widget(U8G2_ST7567_JLX12864_F_4W_HW_SPI *display);

        void set_focus(bool f) {
            focus = f;
            for (const auto &item: focus_callbacks)
                if (item != nullptr)
                    (*item)(f);
        }

        virtual void onFocusCall(void (*fun)(bool));

        virtual void onVisibleCall(void (*fun)(bool));

        virtual bool click();

        virtual bool move_left();

        virtual bool move_right();

        bool focused() {
            return focus;
        }

        virtual void draw();

        void set_visible(bool v) {
            visible = v;
            for (const auto &item: visible_callbacks)
                if (item != nullptr)
                    (*item)(v);
            if (send_visible_to_childs)
                for (const auto &item: childs)
                    if (item != nullptr)
                        item->set_visible(v);
        }

    private:
        Widget *parent = nullptr;
        Widget *childs[UI_WIDGET_MAX_CHILDS] = {nullptr};
        uint8_t current_child = 0;
        bool send_visible_to_childs = true;
        bool send_draw_to_childs = true;

        void (*focus_callbacks[UI_WIDGET_CALLBACKS])(bool) = {nullptr};

        uint8_t inserted_focus_callback = 0;

        void (*visible_callbacks[UI_WIDGET_CALLBACKS])(bool) = {nullptr};

        uint8_t inserted_visible_callback = 0;
    protected:
        U8G2_ST7567_JLX12864_F_4W_HW_SPI *display;
        bool focus = false;
        bool visible = false;

        void add_child(Widget *pWidget);
    };

} // Manager

#endif //DINOCTOPUS_2040_WIDGET_HPP

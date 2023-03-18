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

        virtual void setFocus(bool f);

        virtual void onFocusCall(void (*fun)(bool));
        virtual void onVisibleCall(void (*fun)(bool));
        virtual void onExitCall(void (*fun)());
        virtual void onUpdateCall(void (*fun)());

        virtual void exit();

        virtual bool click();
        virtual bool move_left();
        virtual bool move_right();
        virtual void update();

        virtual bool clickAction();
        virtual bool leftAction();
        virtual bool rightAction();

        bool focused() const {
            return focus;
        }

        virtual void draw();

        virtual void setVisible(bool v) {
            visible = v;
            for (const auto &item: visible_callbacks)
                if (item != nullptr)
                    (*item)(v);
            if (send_visible_to_childs)
                for (const auto &item: childs)
                    if (item != nullptr)
                        item->setVisible(v);
        }

        Widget *childs[UI_WIDGET_MAX_CHILDS] = {nullptr};
    private:
        Widget *parent = nullptr;
        bool send_visible_to_childs = true;
        bool send_draw_to_childs = true;

        void (*focus_callbacks[UI_WIDGET_CALLBACKS])(bool) = {nullptr};
        uint8_t inserted_focus_callback = 0;

        void (*visible_callbacks[UI_WIDGET_CALLBACKS])(bool) = {nullptr};
        uint8_t inserted_visible_callback = 0;

        void (*exit_callbacks[UI_WIDGET_CALLBACKS])() = {nullptr};
        uint8_t inserted_exit_callback = 0;

        void (*update_callbacks[UI_WIDGET_CALLBACKS])() = {nullptr};
        uint8_t inserted_update_callback = 0;
    protected:
        U8G2_ST7567_JLX12864_F_4W_HW_SPI *display;
        bool focus = false;
        bool visible = false;

        void add_child(Widget *pWidget);

        uint8_t current_child = 0;
    };

} // Manager

#endif //DINOCTOPUS_2040_WIDGET_HPP

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
    template<typename D>
    class Widget {
    public:
        explicit Widget(Widget<D> *parent);
        /**
         * For the root widget only, all the child widgets will inherit the display (fancy right?)
         * @param display
         */
        explicit Widget(D &display);

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

        [[nodiscard]] bool focused() const;

        virtual void draw();

        virtual void setVisible(bool v);

        Widget *childs[UI_WIDGET_MAX_CHILDS] = {nullptr};
    private:
        Widget *parent = nullptr;

        void (*focus_callbacks[UI_WIDGET_CALLBACKS])(bool) = {nullptr};

        uint8_t inserted_focus_callback = 0;

        void (*visible_callbacks[UI_WIDGET_CALLBACKS])(bool) = {nullptr};

        uint8_t inserted_visible_callback = 0;

        void (*exit_callbacks[UI_WIDGET_CALLBACKS])() = {nullptr};

        uint8_t inserted_exit_callback = 0;

        void (*update_callbacks[UI_WIDGET_CALLBACKS])() = {nullptr};

        uint8_t inserted_update_callback = 0;

    protected:
        D *display;
        bool focus = false;
        bool visible = false;

        void add_child(Widget *pWidget);

        uint8_t current_child = 0;
    };

} // Manager

#endif //DINOCTOPUS_2040_WIDGET_HPP

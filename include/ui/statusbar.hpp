//
// Created by bjo on 3/18/23.
//

#ifndef DINOCTOPUS_2040_STATUSBAR_HPP
#define DINOCTOPUS_2040_STATUSBAR_HPP


#include "widget.hpp"

namespace UI::Widgets {
    class StatusBar : public UI::Widget {
        char buffer[50] = {0};
    public:
        StatusBar(U8G2_ST7567_JLX12864_F_4W_HW_SPI *display, Widget *parent) : Widget(display, parent) {
        }

        void draw() override;

        void set_message(char string[50]);
    };
}


#endif //DINOCTOPUS_2040_STATUSBAR_HPP

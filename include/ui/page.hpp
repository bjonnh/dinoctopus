//
// Created by bjo on 3/17/23.
//

#ifndef DINOCTOPUS_2040_PAGE_HPP
#define DINOCTOPUS_2040_PAGE_HPP

#include "widget.hpp"

namespace UI::Widgets {
    class Page : public UI::Widget { ;

    public:
        Page(U8G2_ST7567_JLX12864_F_4W_HW_SPI *display, Widget *parent): Widget(display, parent) {
        }

        void setVisible(bool f) override;

        // Pages focus the first child
        void setFocus(bool f) override;
    };
}

#endif //DINOCTOPUS_2040_PAGE_HPP

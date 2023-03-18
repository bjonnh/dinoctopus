//
// Created by bjo on 3/17/23.
//

#ifndef DINOCTOPUS_2040_VERTICALMENU_HPP
#define DINOCTOPUS_2040_VERTICALMENU_HPP

#include <cstdint>
#include <cstring>
#include "menu.hpp"

namespace UI::Widgets {
    class Vertical_menu : public Menu {

    public:
        Vertical_menu(U8G2_ST7567_JLX12864_F_4W_HW_SPI *display, Widget *parent) : Menu(display, parent) {}
        void draw() override;
    };
} // Widgets

#endif //DINOCTOPUS_2040_VERTICALMENU_HPP

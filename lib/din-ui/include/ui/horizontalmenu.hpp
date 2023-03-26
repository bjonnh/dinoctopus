//
// Created by bjo on 3/17/23.
//

#ifndef DINOCTOPUS_2040_HORIZONTAL_MENU_HPP
#define DINOCTOPUS_2040_HORIZONTAL_MENU_HPP

#include <cstdint>
#include <cstring>
#include "menu.hpp"

namespace UI::Widgets {
    template<typename D>
    class Horizontal_menu : public Menu<D> {

    public:
        explicit Horizontal_menu(Widget<D> &parent) : Menu<D>(parent) {}

        void draw() override;
    };
} // Widgets

#endif //DINOCTOPUS_2040_HORIZONTAL_MENU_HPP

//
// Created by bjo on 3/18/23.
//

#ifndef DINOCTOPUS_2040_STATUS_BAR_HPP
#define DINOCTOPUS_2040_STATUS_BAR_HPP


#include "widget.hpp"

namespace UI::Widgets {
    template<typename D>
    class StatusBar : public UI::Widget<D> {
        char buffer[50] = {0};
    public:
        explicit StatusBar(Widget<D> *parent) : Widget<D>(parent) {}

        void draw() override;

        void set_message(char string[50]);
    };
}


#endif //DINOCTOPUS_2040_STATUS_BAR_HPP

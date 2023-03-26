//
// Created by bjo on 3/18/23.
//

#ifndef DINOCTOPUS_2040_STATUS_BAR_HPP
#define DINOCTOPUS_2040_STATUS_BAR_HPP


#include "widget.hpp"
#include "ui.hpp"

namespace UI::Widgets {
    template<typename D>
    class StatusBar : public UI::Widget<D> {
        char buffer[UI_CHARACTERS_PER_LINE] = {0};
    public:
        explicit StatusBar(Widget<D> &parent) : Widget<D>(parent) {}

        void draw() override;

        void set_message(char string[UI_CHARACTERS_PER_LINE]);
    };
}

#endif //DINOCTOPUS_2040_STATUS_BAR_HPP

//
// Created by bjo on 3/17/23.
//

#ifndef DINOCTOPUS_2040_PAGE_HPP
#define DINOCTOPUS_2040_PAGE_HPP

#include "widget.hpp"

namespace UI::Widgets {
    template<typename D>
    class Page : public UI::Widget<D> { ;

    public:
        explicit Page(Widget<D> &parent) : Widget<D>(parent) {
        }

        void setVisible(bool f) override;

        // Pages focus the first child
        void setFocus(bool f) override;
    };
}

#endif //DINOCTOPUS_2040_PAGE_HPP

//
// Created by bjo on 3/18/23.
//

#ifndef DINOCTOPUS_2040_ROOT_HPP
#define DINOCTOPUS_2040_ROOT_HPP


#include "widget.hpp"

namespace UI::Widgets {
    template<class D>
    class Root : public UI::Widget<D> {

    public:
        Root();

        explicit Root(D &display) : Widget<D>(display) {}
    };
}

#endif //DINOCTOPUS_2040_ROOT_HPP

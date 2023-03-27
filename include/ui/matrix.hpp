/*
 * Copyright (c) 2023. Jonathan Bisson
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */


#ifndef DINOCTOPUS_2040_MATRIX_HPP
#define DINOCTOPUS_2040_MATRIX_HPP

#include <cstdint>
#include "ui/widget.hpp"
#include "../../lib/routing-matrix/routingmatrix.hpp"
#include "ui/ui.hpp"

namespace UI::Widgets {
    enum MatrixState {
        back, input_select, route_select
    };

    template<typename D>
    class Matrix : public UI::Widget<D> {
    public:
        explicit Matrix(Widget<D> &parent) : Widget<D>(parent) {}

        void draw() override;

        void set_matrix(RoutingMatrix &new_matrix);

        bool clickAction() override;

        bool leftAction() override;

        bool rightAction() override;

        RoutingMatrix & getMatrix();

        bool dirty = false;

        void set_dirty(bool b);

        [[nodiscard]] bool get_dirty() const;

        void onDirtyCall(void (*fun)(bool));

    private:
        uint8_t p0_selected_input = 1;
        uint8_t p0_selected_output = 0;
        char buffer[UI_CHARACTERS_PER_LINE] = {0};
        RoutingMatrix current_matrix;
        MatrixState state = back;

        void (*dirty_callbacks[UI_WIDGET_CALLBACKS])(bool) = {nullptr};

        uint8_t inserted_dirty_callback = 0;

    };
}

#endif //DINOCTOPUS_2040_MATRIX_HPP

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


#include "ui/matrix.hpp"
#include "U8g2lib.h"

namespace UI::Widgets {
    template class Matrix<U8G2>;

    template<typename D> void Matrix<D>::set_matrix(RoutingMatrix &new_matrix) {
        current_matrix.set_from_matrix(new_matrix);
    }

#pragma clang diagnostic pop

    template<typename D>
    bool Matrix<D>::leftAction() {
        if (!this->focus or !this->visible)
            return false;

        switch (state) {
            case back:
                break;
            case input_select:
                if (p0_selected_input > 0)
                    p0_selected_input--;
                if (p0_selected_input == 0)
                    state = back;
                break;
            case route_select:
                if (p0_selected_output > 0)
                    p0_selected_output--;
                if (p0_selected_output == 0)
                    state = input_select;
                break;
        }

        return true;
    }

    template<typename D>
    bool Matrix<D>::rightAction() {
        if (!this->focus or !this->visible)
            return false;

        switch (state) {
            case back:
                state = input_select;
                p0_selected_input = 1;
                break;
            case input_select:
                if (p0_selected_input < 4)
                    p0_selected_input++;
                break;
            case route_select:
                if (p0_selected_output < 4)
                    p0_selected_output++;
                break;
        }

        return true;
    }

    template<typename D>
    bool Matrix<D>::clickAction() {
        if (!this->focus or !this->visible)
            return false;

        switch (state) {
            case back:
                this->setFocus(false);
                this->exit();
                break;
            case input_select:
                state = route_select;
                p0_selected_output = 1;
                break;
            case route_select:
                set_dirty(true);
                uint32_t current_value = current_matrix.get_element_2d(p0_selected_input - 1,p0_selected_output - 1);
                uint32_t new_value = (current_value>0) ? 0 : 255;
                current_matrix.set_element_2d(p0_selected_input - 1, p0_selected_output - 1, new_value);
                this->update();
                break;
        }

        return true;
    }

    template<typename D>
    void Matrix<D>::onDirtyCall(void (*fun)(bool)) {
        if (inserted_dirty_callback < UI_WIDGET_CALLBACKS) {
            dirty_callbacks[inserted_dirty_callback++] = fun;
        }
    }

    template<typename D>
    void Matrix<D>::set_dirty(bool b) {
        dirty = b;
        for (const auto &item: dirty_callbacks)
            if (item != nullptr)
                (*item)(b);
    }

    template<typename D>
    bool Matrix<D>::get_dirty() const {
        return dirty;
    }

    template<typename D>
    RoutingMatrix & Matrix<D>::getMatrix() {
        return current_matrix;
    }
}


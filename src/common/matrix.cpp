//
// Created by bjo on 3/18/23.
//

#include "ui/matrix.hpp"
#include "ui/manager.hpp"
#include "U8g2lib.h"

namespace UI::Widgets {
    template class Matrix<U8G2>;

    template<typename D>
    void Matrix<D>::set_matrix(routing_matrix &new_matrix) {
        memcpy(current_matrix, new_matrix, sizeof(uint32_t) * 4 * 4);
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
                current_matrix[p0_selected_input - 1][p0_selected_output - 1] ^= 255;
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
    routing_matrix *Matrix<D>::getMatrix() {
        return &current_matrix;
    }
}


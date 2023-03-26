//
// Created by bjo on 3/18/23.
//

#ifndef DINOCTOPUS_2040_MATRIX_HPP
#define DINOCTOPUS_2040_MATRIX_HPP

#include <cstdint>
#include "ui/widget.hpp"
#include "routingmatrix.hpp"
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

        void set_matrix(routing_matrix &new_matrix);

        bool clickAction() override;

        bool leftAction() override;

        bool rightAction() override;

        routing_matrix *getMatrix();

        bool dirty = false;

        void set_dirty(bool b);

        [[nodiscard]] bool get_dirty() const;

        void onDirtyCall(void (*fun)(bool));

    private:
        uint8_t p0_selected_input = 1;
        uint8_t p0_selected_output = 0;
        char buffer[UI_CHARACTERS_PER_LINE] = {0};
        routing_matrix current_matrix = {0};
        MatrixState state = back;

        void (*dirty_callbacks[UI_WIDGET_CALLBACKS])(bool) = {nullptr};

        uint8_t inserted_dirty_callback = 0;

    };
}

#endif //DINOCTOPUS_2040_MATRIX_HPP

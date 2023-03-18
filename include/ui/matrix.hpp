//
// Created by bjo on 3/18/23.
//

#ifndef DINOCTOPUS_2040_MATRIX_HPP
#define DINOCTOPUS_2040_MATRIX_HPP


#include <cstdint>
#include "widget.hpp"
#include "routingmatrix.hpp"



namespace UI::Widgets {
    enum MatrixState { back, input_select, route_select };
    class Matrix : public UI::Widget {
        uint8_t p0_selected_input = 1;
        uint8_t p0_selected_output = 0;
        uint8_t zone = 0;
        uint8_t cursor_position = 0;
        char buffer[50] = {0};
        routing_matrix current_matrix = {0};
        MatrixState state;
    public:
        Matrix(U8G2_ST7567_JLX12864_F_4W_HW_SPI *display, Widget *parent) : Widget(display, parent) {
        }

        void draw() override;

        void set_matrix(routing_matrix &new_matrix);

        bool clickAction() override;

        bool leftAction();

        bool rightAction();

        routing_matrix *getMatrix() {
            return &current_matrix;
        }
    };
}

#endif //DINOCTOPUS_2040_MATRIX_HPP

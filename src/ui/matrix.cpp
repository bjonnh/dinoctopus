//
// Created by bjo on 3/18/23.
//

#include "ui/matrix.hpp"
#include "ui/ui.hpp"

namespace UI {
    namespace Widgets {
        void Matrix::set_matrix(routing_matrix &new_matrix) {
            memcpy(current_matrix, new_matrix, sizeof(uint32_t) * 4 * 4);
        }

        char matrix_sign(uint32_t v) {
            return v > 0 ? 'X' : '.';
        }

        void Matrix::draw() {
            if (!visible)
                return;

            display->setFont(u8g2_font_5x8_mf);
            if (focus) {
                display->setDrawColor(state != back);
                display->drawStr(0, 2.5 * UI_LINE_HEIGHT, "<");
            }
            display->setDrawColor(1);
            display->drawStr(5, 2.5 * UI_LINE_HEIGHT, " IO|   1   2   3   4 ");
            display->drawHLine(10, 2.5 * UI_LINE_HEIGHT, 100);
            for (uint8_t i = 0; i < 4; i++) {
                if ((state==input_select or state==route_select) && p0_selected_input == i + 1)
                    display->setDrawColor(0);
                else
                    display->setDrawColor(1);
                snprintf(buffer, 3, "%d", i + 1);
                display->drawStr(10, (3.5 + i) * UI_LINE_HEIGHT, buffer);
                display->setDrawColor(1);
                display->drawStr(20, (3.5 + i) * UI_LINE_HEIGHT, "|");
                for (int j = 0; j < 4; j++) {
                    if ((state==route_select) && (p0_selected_output == j + 1) && (p0_selected_input == i + 1))
                        display->setDrawColor(0);
                    else
                        display->setDrawColor(1);
                    snprintf(buffer, 50, "   %c", matrix_sign(current_matrix[i][j]));
                    display->drawStr(25 + 20 * j, (3.5 + i) * UI_LINE_HEIGHT, buffer);
                }
            }
            display->setDrawColor(1);
            if (focus)
                display->drawFrame(0, 9, 128, 64 - 10);
        }
        bool Matrix::leftAction() {
            if (!focus or !visible)
                return false;

            switch(state) {
                case back:
                    break;
                case input_select:
                    if (p0_selected_input>0)
                        p0_selected_input--;
                    if (p0_selected_input==0)
                        state=back;
                    break;
                case route_select:
                    if (p0_selected_output>0)
                        p0_selected_output--;
                    if (p0_selected_output==0)
                        state=input_select;
                    break;
            }

            return true;
        }

        bool Matrix::rightAction() {
            if (!focus or !visible)
                return false;

            switch(state) {
                case back:
                    state=input_select;
                    p0_selected_input=1;
                    break;
                case input_select:
                    if (p0_selected_input<4)
                        p0_selected_input++;
                    break;
                case route_select:
                    if (p0_selected_output<4)
                        p0_selected_output++;
                    break;
            }

            return true;
        }

        bool Matrix::clickAction() {
            if (!focus or !visible)
                return false;

            switch(state) {
                case back:
                    setFocus(false);
                    exit();
                    break;
                case input_select:
                    state = route_select;
                    p0_selected_output=1;
                    break;
                case route_select:
                    current_matrix[p0_selected_input-1][p0_selected_output-1] ^= 255;
                    update();
                    break;
            }

            return true;
        }
    }
}

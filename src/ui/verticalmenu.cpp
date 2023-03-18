//
// Created by bjo on 3/17/23.
//

#include "ui/verticalmenu.hpp"
#include "ui/ui.hpp"

namespace UI::Widgets {
        void Vertical_menu::draw() {
            if (!visible)
                return;

            display->setFont(u8g2_font_5x8_mf);
            display->setDrawColor(1);

            for (uint8_t i=0; i < inserted_items;i++) {
                uint8_t shift_y = 9 * (i+2);
                if (current_item == i) {
                    display->setDrawColor(0);
                    display->drawHLine(5, shift_y, 50);
                }
                if (selected_item == i)
                    snprintf(buffer, 50, ">%s", items[i]);
                else
                    snprintf(buffer, 50, " %s", items[i]);
                display->drawStr(5,shift_y, buffer);
                display->setDrawColor(1);
            }
        }
    } // Widgets

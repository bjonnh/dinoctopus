//
// Created by bjo on 3/17/23.
//

#include "ui/horizontalmenu.hpp"
#include "ui/ui.hpp"

namespace UI::Widgets {
    template<>
    void Horizontal_menu<UI_TYPE>::draw() {
        if (!visible)
            return;

        display->setFont(u8g2_font_5x8_mf);
        display->setDrawColor(1);

        for (uint8_t i = 0; i < inserted_items; i++) {
            uint8_t shift_x = 48 * i;
            if (current_item == i) {
                if (focus)
                    display->setDrawColor(0);
                else
                    display->drawHLine(shift_x, UI_LINE_HEIGHT + 1, 50);
            }
            if (!focus & (selected_item == i))
                snprintf(buffer, 50, ">%s", items[i]);
            else
                snprintf(buffer, 50, " %s", items[i]);
            display->drawStr(shift_x, UI_LINE_HEIGHT, buffer);
            display->setDrawColor(1);
        }
    }
} // Widgets

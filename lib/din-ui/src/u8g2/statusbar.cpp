//
// Created by bjo on 3/18/23.
//

#include "ui/statusbar.hpp"
#include "U8g2lib.h"
#include "ui/ui.hpp"

namespace UI::Widgets {
    template<>
    void StatusBar<U8G2>::draw() {
        display->setDrawColor(1);
        display->drawStr(0, 8 * UI_LINE_HEIGHT - 2, buffer);
    }
}

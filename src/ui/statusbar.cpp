//
// Created by bjo on 3/18/23.
//

#include "ui/statusbar.hpp"
#include "ui/ui.hpp"

namespace UI {
    namespace Widgets {
        void StatusBar::draw() {
            display->setDrawColor(1);
            display->drawStr(0, 8 * UI_LINE_HEIGHT - 2, buffer);
        }

        void StatusBar::set_message(char *string) {
            strncpy(buffer, string, 50);
        }
    }
}
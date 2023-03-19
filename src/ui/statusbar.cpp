//
// Created by bjo on 3/18/23.
//

#include "ui/statusbar.hpp"
#include "ui/ui.hpp"

namespace UI::Widgets {
    template class StatusBar<UI_TYPE>;

    template<>
    void StatusBar<UI_TYPE>::draw() {
        display->setDrawColor(1);
        display->drawStr(0, 8 * UI_LINE_HEIGHT - 2, buffer);
    }

    template<typename D>
    void StatusBar<D>::set_message(char *string) {
        strncpy(buffer, string, 50);
    }
}

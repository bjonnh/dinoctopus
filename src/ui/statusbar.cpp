//
// Created by bjo on 3/18/23.
//

#include "ui/statusbar.hpp"
#include "ui/ui.hpp"

namespace UI::Widgets {
    template class StatusBar<UI_TYPE>;

    template<typename D>
    void StatusBar<D>::set_message(char *string) {
        strncpy(buffer, string, 50);
    }
}

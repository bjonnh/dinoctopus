//
// Created by bjo on 3/18/23.
//

#include "../include/ui/statusbar.hpp"
#include "U8g2lib.h"

namespace UI::Widgets {
    template class StatusBar<U8G2>;

    template<typename D>
    void StatusBar<D>::set_message(char *string) {
        strncpy(buffer, string, UI_CHARACTERS_PER_LINE);
    }
}

#include "ui/manager.hpp"
#include "U8g2lib.h"
#include "config.hpp"
#include "ui/root.hpp"
#include "ui/widget.hpp"
#include "ui/horizontalmenu.hpp"
#include "ui/verticalmenu.hpp"
#include "ui/matrix.hpp"
#include "ui/statusbar.hpp"
#include "ui/page.hpp"
#include "storage.hpp"
#include "u8g2.h"
#include <iostream>

void UI::Manager::loop() {
    int k;
        while(true) {
    std::cout << "running a loop" << std::endl;

        display_update();
        do {
            k = u8g_sdl_get_key();
        } while (k < 0);

        if (k == 276) // left
            encoder_left();
        else if (k == 275) // right
            encoder_right();
        else if (k == 0) // enter
            encoder_click();
        else if (k== 113) // q
            return;
        else
            std::cout << "Unhandled key: " << k << std::endl;
    }
}

void UI::Manager::initEncoder() {}

// TODO Use another window for those

void UI::Manager::initStrip() {}

void UI::Manager::normalStrip() {}

void UI::Manager::dirtyStrip() {}

void UI::Manager::errorStrip() {}

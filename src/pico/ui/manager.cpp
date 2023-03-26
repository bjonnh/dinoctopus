//
// Created by bjo on 3/11/23.
//

#include <Arduino.h>
#include <SPI.h>
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
#include "utils_rp2040.hpp"

uint32_t last_update_time = CURRENT_TIME_MS;

bool updated_last_time = false;

void UI::Manager::loop() {
    encoderPoll();
    // We wait 10ms before trying to update if we had an interaction if not we wait 100ms
    if ((CURRENT_TIME_MS - last_update_time) > (updated_last_time ? 10 : 100)) {
        display_update();
        updated_last_time = false;
        last_update_time = CURRENT_TIME_MS;
    }
    if (updated) {
        last_update_time = CURRENT_TIME_MS;
        updated_last_time = true;
        updated = false;
    }
}

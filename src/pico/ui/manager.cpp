/*
 * Copyright (c) 2023. Jonathan Bisson
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

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

void UI::Manager::initHardwareLCD() {
    // Init the SPI
    SPI.setRX(4);
    SPI.setCS(1);
    SPI.setSCK(LCD_CLOCK);
    SPI.setTX(LCD_MOSI);
    SPI.begin();
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
}

void UI::Manager::loop() {
    static uint32_t last_update_time = CURRENT_TIME_MS-100;
    static bool updated_last_time = false;

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

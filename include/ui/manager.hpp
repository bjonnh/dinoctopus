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

#ifndef DINOCTOPUS_2040_MANAGER_HPP
#define DINOCTOPUS_2040_MANAGER_HPP

#include "../../lib/routing-matrix/routingmatrix.hpp"
#include "ui/ui.hpp"
#include "../../lib/storage/storage.hpp"
#include "U8g2lib.h"
#include "ui/root.hpp"
#include "ui/horizontalmenu.hpp"
#include "ui/statusbar.hpp"
#include "ui/page.hpp"
#include "matrix.hpp"
#include "ui/verticalmenu.hpp"

namespace UI {
    class Manager {
    public:
        explicit Manager(U8G2 &u8G2Lcd);

        void init();

        void loop();

        void set_latency(uint8_t cpu, uint32_t value);

        bool query_for_router();

        static void set_routing_response(RoutingMatrix &matrix);

        void encoder_click();

        void encoder_right();

        void encoder_left();

        bool update_for_router();

        static RoutingMatrix current_route();

        void has_update_for_router();

        [[maybe_unused]] static void errorStrip();

        static void dirtyStrip();

        static void normalStrip();

        bool latency_watch = false;

        UI::Widgets::Root<U8G2> root;
        UI::Widgets::Horizontal_menu<U8G2> main_menu;
        UI::Widgets::StatusBar<U8G2> status_bar;

        UI::Widgets::Page<U8G2> page_routing;
        UI::Widgets::Matrix<U8G2> matrix;

        UI::Widgets::Page<U8G2> page_settings;
        UI::Widgets::Vertical_menu<U8G2> settings_menu;

        UI::Widgets::Page<U8G2> page_debug;
        UI::Widgets::Vertical_menu<U8G2> debug_menu;

    private:
        char buffer[UI_CHARACTERS_PER_LINE] = {0};

        uint8_t latency_cpu[2] = {0, 0};

        bool updated = true;

        bool query_for_router_requested = false;

        bool update_for_router_ready = false;

        bool dirty = true;

        static void initStrip();

        void initLCD();

        static void initEncoder();

        void display_update();

        void encoderPoll();

        void readEncoder();

        U8G2 &u8g2_lcd;
    };
}

#endif //DINOCTOPUS_2040_MANAGER_HPP

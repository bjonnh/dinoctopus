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
#include "midiwatch.hpp"

typedef U8G2 LCD_t;

namespace UI {
    class Manager {
    public:
        explicit Manager(LCD_t &lcd);

        void init();

        void loop();

        void set_latency(uint8_t cpu, uint32_t value);

        bool query_for_router();

        static void set_routing_response(RoutingMatrix &matrix);

        void encoder_click();
        void encoder_right();
        void encoder_left();

        bool debug_for_router();
        bool update_for_router();

        static RoutingMatrix current_route();

        void has_update_for_router();

        bool latency_watch = false;

        UI::Widgets::Root<LCD_t> root;
        UI::Widgets::Horizontal_menu<LCD_t> main_menu;
        UI::Widgets::StatusBar<LCD_t> status_bar;

        UI::Widgets::Page<LCD_t> page_routing;
        UI::Widgets::Matrix<LCD_t> matrix;

        UI::Widgets::Page<LCD_t> page_settings;
        UI::Widgets::Vertical_menu<LCD_t> settings_menu;

        UI::Widgets::Page<LCD_t> page_debug;
        UI::Widgets::Vertical_menu<LCD_t> debug_menu;

        UI::Widgets::Page<LCD_t> page_watch;
        UI::Widgets::MidiWatch<LCD_t> midi_watch;

        void set_usb_enabled(bool i);

        void set_midi_message(const uint8_t midi_message[5]);

        bool debug_mode=false;
        bool debug_for_router_requested = false;
    private:
        char buffer[UI_CHARACTERS_PER_LINE] = {0};

        uint8_t latency_cpu[2] = {0, 0};

        bool updated = true;

        bool query_for_router_requested = false;
        bool update_for_router_ready = false;

        bool dirty = true;

        static void initStrip();

        void initLCD();

        static void initHardwareLCD();

        static void initEncoder();

        void display_update();

        void encoderPoll();

        void readEncoder();

        LCD_t &lcd;
        bool usb_enabled;
        uint8_t current_midi_message[5];
        bool midi_message_updated=false;
    };
}

#endif //DINOCTOPUS_2040_MANAGER_HPP

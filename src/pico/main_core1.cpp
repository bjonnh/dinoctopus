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

#include "ui/manager.hpp"
#include "config.hpp"
#include "queue.hpp"
#include "utils_rp2040.hpp"

extern queue_t request_queue;
extern queue_t response_queue;

U8G2_ST7567_JLX12864_F_4W_HW_SPI lcd(U8G2_R2, LCD_CS, LCD_RS, LCD_RESET);
UI::Manager ui(reinterpret_cast<LCD_t &>(lcd));

LATENCY_VARIABLES(1)


void setup1() {
    LATENCY_INIT(1)

    ui.init();
}

void loop1() {
    if (ui.latency_watch) {
        LATENCY_CALCULATOR(1)
        if (count_latency_1 == N_LOOPS_MAX_LATENCY_CORE1)
            ui.set_latency(1, processor_max_latency_1);
    }

    if (queue_get_level(&response_queue) > 0) {
        queue_response_t response;
        queue_remove_blocking(&response_queue, &response);

        switch (response.code) {
            case RESPONSE_LATENCY:
                if (ui.latency_watch)
                    ui.set_latency(0, response.data.latency);
                break;
            case RESPONSE_ROUTING:
                UI::Manager::current_route().load_from_array(response.data.matrix);
                break;
            case RESPONSE_GET_USB_ENABLED:
                ui.set_usb_enabled(response.data.b);
            case RESPONSE_SET_ROUTING:
                // Not handled by this core
                break;
            case RESPONSE_MIDI_INFO:
                ui.set_midi_message(response.data.midi);
                break;
        }
    }

    // Unless we are asking for something specific, let's just ask for latency
    if (ui.query_for_router()) {
        queue_request_t entry = {REQUEST_ROUTING, 0};
        queue_add_blocking(&request_queue, &entry);
    } else if (ui.update_for_router()) {
        queue_request_t entry = {REQUEST_SET_ROUTING};
        UI::Manager::current_route().save_to_array(entry.data);
        queue_add_blocking(&request_queue, &entry);
    } else if(ui.debug_for_router()) {
        queue_request_t entry = {REQUEST_DEBUG_MIDI, ui.debug_mode };
        queue_add_blocking(&request_queue, &entry);
    } else {
        if (ui.latency_watch) {
            queue_request_t entry = {REQUEST_LATENCY, 0};
            queue_add_blocking(&request_queue, &entry);
        }
    }
    ui.loop();
}


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

#include <Arduino.h>  // necessary for setup/loop
#include <SPI.h>
#include "midi/midirouter.hpp"
#include "ui/manager.hpp"
#include "queue.hpp"
#include "utils_rp2040.hpp"
#include "config.hpp"
#include "storage.hpp"

static MidiRouter midi_router;
U8G2_ST7567_JLX12864_F_4W_HW_SPI u8g2_lcd(U8G2_R2, LCD_CS, LCD_RS, LCD_RESET);
UI::Manager ui(reinterpret_cast<U8G2 &>(u8g2_lcd));

// Core 0, running the MIDI will only receive requests never send any
queue_t request_queue;
queue_t response_queue;

LATENCY_VARIABLES(0)
LATENCY_VARIABLES(1)

void setup() {
    LATENCY_INIT(0)
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);
    queue_init(&request_queue, sizeof(queue_request_t), 2);
    queue_init(&response_queue, sizeof(queue_response_t), 2);
    midi_router.init();
}

void setup1() {
    LATENCY_INIT(1)

    ui.init();
}

void response_usb_status(bool status) {
    queue_response_t entry = { RESPONSE_GET_USB_ENABLED };
    *(entry.data) = status;
    queue_add_blocking(&response_queue, &entry);
}

void loop() {
    static bool latency_watch_core0 = false;
    static bool old_usb_status = false;

    if (latency_watch_core0) {
        LATENCY_CALCULATOR(0);
    }

    if (queue_get_level(&request_queue) > 0) {
        queue_request_t request;
        queue_remove_blocking(&request_queue, &request);
        switch (request.code) {
            case REQUEST_LATENCY: {
                latency_watch_core0 = true;
                queue_response_t entry = {RESPONSE_LATENCY, processor_max_latency_0};
                queue_add_blocking(&response_queue, &entry);
            }
                break;
            case REQUEST_ROUTING: {
                queue_response_t entry = {RESPONSE_ROUTING};
                midi_router.get_matrix(entry.data);
                queue_add_blocking(&response_queue, &entry);
            }
                break;
            case REQUEST_SET_ROUTING:
                midi_router.set_matrix(request.data);
                break;
            case REQUEST_GET_USB_ENABLED:
                response_usb_status(old_usb_status);
                break;
        }
    }
    if (midi_router.usb_enabled() != old_usb_status) {
        old_usb_status = !old_usb_status;
        response_usb_status(old_usb_status);
    }
    midi_router.loop();
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
                    ui.set_latency(0, response.data[0]);
                break;
            case RESPONSE_ROUTING:
                UI::Manager::current_route().load_from_array(response.data);
                break;
            case RESPONSE_GET_USB_ENABLED:
                ui.set_usb_enabled(response.data[0]);
            case RESPONSE_SET_ROUTING:
                // Not handled by this core
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
    } else {
        if (ui.latency_watch) {
            queue_request_t entry = {REQUEST_LATENCY, 0};
            queue_add_blocking(&request_queue, &entry);
        }
    }
    ui.loop();
}

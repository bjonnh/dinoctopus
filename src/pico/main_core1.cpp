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

#include <Adafruit_USBD_CDC.h>
#include <config.hpp>

#include "queue.hpp"
#include "utils_rp2040.hpp"

extern queue_t request_queue;
extern queue_t response_queue;


LATENCY_VARIABLES(1)

void setup1() {
    LATENCY_INIT(1)

}

void loop1() {
//    LATENCY_CALCULATOR(1);
 //   Serial.println("Hello from the loop, current latency is: " + String(processor_max_latency_1) + "us");
    if (queue_get_level(&response_queue) > 0) {
        queue_response_t response;
        queue_remove_blocking(&response_queue, &response);

        switch (response.code) {
            case RESPONSE_LATENCY:
                break;
            case RESPONSE_ROUTING:
                // response.data.matrix
                break;
            case RESPONSE_GET_USB_ENABLED:
                // response.data.b
            case RESPONSE_SET_ROUTING:
                // Not handled by this core
                break;
            case RESPONSE_MIDI_INFO:
                // response.data.midi
                break;
        }
    }

    // Unless we are asking for something specific, let's just ask for latency
/*    if (ui.query_for_router()) {
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
    } */
}


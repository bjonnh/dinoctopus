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

#include <Arduino.h>
#include "midi/midirouter.hpp"
#include "queue.hpp"
#include "utils_rp2040.hpp"
#include "config.hpp"
#include "storage.hpp"

static MidiRouter midi_router;

// Core 0, running the MIDI will only receive requests never send any
queue_t request_queue;
queue_t response_queue;

LATENCY_VARIABLES(0)

void response_midi_message(uint8_t message[5]);

void setup() {
    LATENCY_INIT(0)

    queue_init(&request_queue, sizeof(queue_request_t), 2);
    queue_init(&response_queue, sizeof(queue_response_t), 2);
    midi_router.init();
}

void response_usb_status(bool status) {
    queue_response_t entry = {RESPONSE_GET_USB_ENABLED};
    entry.data.b = status;
    queue_add_blocking(&response_queue, &entry);
}


void response_midi_message(uint8_t message[5]) {
    queue_response_t entry = {RESPONSE_MIDI_INFO};
    memcpy(entry.data.midi, message, 5);
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
                queue_response_t entry = {RESPONSE_LATENCY};
                entry.data.latency = static_cast<uint8_t>(processor_max_latency_0);
                queue_add_blocking(&response_queue, &entry);
            }
                break;
            case REQUEST_ROUTING: {
                queue_response_t entry = {RESPONSE_ROUTING};
                midi_router.get_matrix(entry.data.matrix);
                queue_add_blocking(&response_queue, &entry);
            }
                break;
            case REQUEST_SET_ROUTING:
                midi_router.set_matrix(request.data);
                break;
            case REQUEST_GET_USB_ENABLED:
                response_usb_status(old_usb_status);
                break;
            case REQUEST_DEBUG_MIDI:
                midi_router.set_debug(request.data[0]);
                break;
        }
    }
    if (midi_router.usb_enabled() != old_usb_status) {
        old_usb_status = !old_usb_status;
        response_usb_status(old_usb_status);
    }
    if (midi_router.has_new_message()) {
        response_midi_message(midi_router.debug_midi_message);
    }
    midi_router.loop();
}

#include <Arduino.h>  // necessary for setup/loop
#include "midi_router.hpp"
#include "ui/ui.hpp"
#include "queue.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "storage.hpp"

MidiRouter midi_router;
UI::Manager ui;

// Core 0, running the MIDI will only receive requests never send any
queue_t request_queue;
queue_t response_queue;

LATENCY_VARIABLES(0)
LATENCY_VARIABLES(1)

void setup() {
    LATENCY_INIT(0)
    queue_init(&request_queue, sizeof(queue_request_t), 2);
    queue_init(&response_queue, sizeof(queue_response_t), 2);
    midi_router.init();
}

void setup1() {
    LATENCY_INIT(1)
    ui.init();
}

bool latency_watch_core0 = false;

void loop() {
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
        }
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
                    ui.set_latency(0, response.data[0][0]);
                break;
            case RESPONSE_ROUTING:
                UI::Manager::set_routing_response(response.data);
                break;
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
        memcpy(entry.data, UI::Manager::current_route(), 4 * 4 * sizeof(uint32_t));
        queue_add_blocking(&request_queue, &entry);
    } else {
        if (ui.latency_watch) {
            queue_request_t entry = {REQUEST_LATENCY, 0};
            queue_add_blocking(&request_queue, &entry);
        }
    }
    ui.loop();
}

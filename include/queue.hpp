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

#ifndef DINOCTOPUS_2040_QUEUE_HPP
#define DINOCTOPUS_2040_QUEUE_HPP

#include <pico/util/queue.h>
#include "../lib/routing-matrix/routingmatrix.hpp"

enum queue_request {
    REQUEST_LATENCY, REQUEST_ROUTING, REQUEST_SET_ROUTING, REQUEST_GET_USB_ENABLED, REQUEST_DEBUG_MIDI
};

enum queue_response {
    RESPONSE_LATENCY, RESPONSE_ROUTING, RESPONSE_SET_ROUTING, RESPONSE_GET_USB_ENABLED, RESPONSE_MIDI_INFO
};


typedef struct {
    queue_request code;
    uint8_t data[ROUTING_MATRIX_BINARY_SIZE];
} queue_request_t;

union queue_data {
    bool b;
    uint8_t matrix[ROUTING_MATRIX_BINARY_SIZE];
    uint8_t latency;
    uint8_t midi[5];
};

typedef struct {
    queue_response code;
    queue_data data;
} queue_response_t;

#endif //DINOCTOPUS_2040_QUEUE_HPP

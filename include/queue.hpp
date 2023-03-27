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
    REQUEST_LATENCY, REQUEST_ROUTING, REQUEST_SET_ROUTING
};
enum queue_response {
    RESPONSE_LATENCY, RESPONSE_ROUTING, RESPONSE_SET_ROUTING
};


typedef struct {
    queue_request code;
    uint8_t data[ROUTING_MATRIX_BINARY_SIZE];
} queue_request_t;

typedef struct {
    queue_response code;
    uint8_t data[ROUTING_MATRIX_BINARY_SIZE];
} queue_response_t;

#endif //DINOCTOPUS_2040_QUEUE_HPP

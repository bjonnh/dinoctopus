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


#ifndef DINOCTOPUS_2040_UTILS_RP2040_HPP
#define DINOCTOPUS_2040_UTILS_RP2040_HPP
#include <Arduino.h>  // necessary for setup/loop
#define CURRENT_TIME_MS to_ms_since_boot(get_absolute_time())

#define LATENCY_VARIABLES(id) static uint32_t count_latency_##id = 0; \
    static uint32_t processor_time_##id; \
    static uint32_t processor_max_latency_##id = 0;

#define LATENCY_INIT(id) processor_time_##id =  to_us_since_boot(get_absolute_time()); \

#define LATENCY_CALCULATOR(id) uint32_t processor_current_time_##id = to_us_since_boot(get_absolute_time()); \
        uint32_t processor_latency_##id = processor_current_time_##id - processor_time_##id; \
        processor_time_##id = processor_current_time_##id; \
        if (processor_latency_##id > processor_max_latency_##id) \
            processor_max_latency_##id = processor_latency_##id; \
        if (count_latency_##id++ > N_LOOPS_MAX_LATENCY_CORE##id) { \
            count_latency_##id = 0; \
            processor_max_latency_##id = 0; \
        }


#endif //DINOCTOPUS_2040_UTILS_RP2040_HPP

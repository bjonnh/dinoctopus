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

#include <cstddef>
#include <cstring>

#include "storage.hpp"

#ifdef ARDUINO

#include <EEPROM.h>

#else
#include <EEPROMS.h>
EEPROMSClass EEPROM;
#endif

void Storage::init() {
    EEPROM.begin(1024);
    if (!legit())
        erase();
}

bool Storage::legit() {
    // Check signature
    for (int i = 0; i < sizeof(signature); i++)
        if (EEPROM.read(i) != signature[i])
            return false;

    // Check Matrix Size
    if (EEPROM.read(offset_matrix_size) != ROUTING_MATRIX_SIZE)
        return false;

    return true;
}

void Storage::erase() {
    for (int i = 0; i < sizeof(signature); i++)
        EEPROM.write(i, signature[i]);

    for (int i = sizeof(signature); i < memory_size; i++)
        EEPROM.write(i, 0);

    EEPROM.write(offset_matrix_size, ROUTING_MATRIX_SIZE);
    EEPROM.commit();
}

void Storage::save_routing_matrix(RoutingMatrix &elements) {
    elements.save_to_array(routing_buffer);

    for (size_t i = 0; i < ROUTING_MATRIX_BINARY_SIZE; i++)
        EEPROM.write(offset_routing_table + i, routing_buffer[i]);

    EEPROM.commit();
}

void Storage::load_routing_matrix(RoutingMatrix &elements) {
    for (int i = 0; i < ROUTING_MATRIX_BINARY_SIZE; i++)
        routing_buffer[i] = EEPROM.read(offset_routing_table + i);
    elements.load_from_array(routing_buffer);
}

Storage storage;

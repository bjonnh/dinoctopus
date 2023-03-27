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

#ifndef DINOCTOPUS_2040_STORAGE_HPP
#define DINOCTOPUS_2040_STORAGE_HPP

#include <cstddef>
#include <cstdint>
#include "../routing-matrix/routingmatrix.hpp"



class Storage {
    char signature[8] = {0x42, 'D', 'I', 'N', 'V', '0', '0', '1'};
    const uint16_t offset_matrix_size = 8;
    const uint16_t offset_routing_table = 64;
    const uint16_t memory_size = 256;
    uint8_t routing_buffer[ROUTING_MATRIX_BINARY_SIZE];
    char buffer[8] = {0};

public:
    /**
     * Mandatory!
     */
    void init();

    /**
     * Erases the EEPROM
     */
    void erase();

    /**
     * Checks if the EEPROM segment contains our signature
     * @return true if yes
     */
    bool legit();

    /**
     * Save the given routing_matrix
     */
    void save_routing_matrix(RoutingMatrix &);

    /**
     * Load into the given routing_matrix
     */
    void load_routing_matrix(RoutingMatrix &);
};

extern Storage storage;

#endif //DINOCTOPUS_2040_STORAGE_HPP

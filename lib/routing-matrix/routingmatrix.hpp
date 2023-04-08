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

#ifndef DINOCTOPUS_2040_ROUTING_MATRIX_HPP
#define DINOCTOPUS_2040_ROUTING_MATRIX_HPP

#include <cstdint>
#include <cstring>

#define ROUTING_MATRIX_SIZE 5
#define ROUTING_MATRIX_ELEMENTS (ROUTING_MATRIX_SIZE*ROUTING_MATRIX_SIZE)
#define ROUTING_MATRIX_BINARY_SIZE (ROUTING_MATRIX_ELEMENTS*sizeof(uint32_t))

class RoutingMatrix {
public:
    RoutingMatrix() {}

    uint32_t get_element_2d(size_t i, size_t j) {
        return matrix[(ROUTING_MATRIX_SIZE * i + j) % ROUTING_MATRIX_ELEMENTS];
    }

    void load_from_array(uint8_t (*arr)) {
        memcpy(matrix, arr, ROUTING_MATRIX_BINARY_SIZE);
    }

    void save_to_array(uint8_t (*arr)) {
        memcpy(arr, matrix, ROUTING_MATRIX_BINARY_SIZE);
    }

    void set_element_2d(size_t i, size_t j, uint32_t value) {
        matrix[(i * ROUTING_MATRIX_SIZE + j) % ROUTING_MATRIX_ELEMENTS] = value;
    }

    void set_from_matrix(RoutingMatrix &source_matrix) {
        memcpy(matrix, source_matrix._get_elements(), ROUTING_MATRIX_BINARY_SIZE);
    }

    /*
     * Do NOT use outside of the copying mechanisms!!
     */
    uint32_t * _get_elements() {
        return matrix;
    }

private:
    uint32_t matrix[ROUTING_MATRIX_ELEMENTS] = {0};
};

#endif //DINOCTOPUS_2040_ROUTING_MATRIX_HPP

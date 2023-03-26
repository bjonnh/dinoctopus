//
// Created by bjo on 3/12/23.
//

#ifndef DINOCTOPUS_2040_ROUTING_MATRIX_HPP
#define DINOCTOPUS_2040_ROUTING_MATRIX_HPP

#include <cstdint>
#include <cstring>

#define ROUTING_MATRIX_SIZE 4
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

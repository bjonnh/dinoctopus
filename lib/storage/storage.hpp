//
// Created by bjo on 3/18/23.
//

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

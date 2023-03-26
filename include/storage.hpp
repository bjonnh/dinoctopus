//
// Created by bjo on 3/18/23.
//

#ifndef DINOCTOPUS_2040_STORAGE_HPP
#define DINOCTOPUS_2040_STORAGE_HPP

#include <cstddef>
#include "routingmatrix.hpp"

class Storage {
    char signature[8] = {0x42, 'D', 'I', 'N', 'V', '0', '0', '0'};
    const size_t offset_routing_table = 64;
    const size_t memory_size = 256;
    uint8_t routing_matrix_buffer[4 * 4 * 4] = {0}; // 4x4 for 32bits
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
    void save_routing_matrix(routing_matrix *);

    /**
     * Load into the given routing_matrix
     */
    void load_routing_matrix(routing_matrix *);
};

extern Storage storage;

#endif //DINOCTOPUS_2040_STORAGE_HPP

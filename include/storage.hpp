//
// Created by bjo on 3/18/23.
//

#ifndef DINOCTOPUS_2040_STORAGE_HPP
#define DINOCTOPUS_2040_STORAGE_HPP


#include <EEPROM.h>
#include "routingmatrix.hpp"

class Storage {
    char signature[8] = {0x42, 'D', 'I', 'N', 'V', '0', '0', '0'};
    const size_t offset_routing_table = 64;
    const size_t memory_size = 256;
    uint8_t routing_matrix_buffer[4 * 4 * 4] = {0}; // 4x4 for 32bits
public:
    void init();

    void erase();

    bool legit();

    void save_routing_matrix(routing_matrix *);

    void load_routing_matrix(routing_matrix *);

    char buffer[8];
};


#endif //DINOCTOPUS_2040_STORAGE_HPP

//
// Created by bjo on 3/18/23.
//

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

//
// Created by bjo on 3/18/23.
//

#include "storage.hpp"

void Storage::init() {
    EEPROM.begin(1024);
    if (!legit())
        erase();
}

bool Storage::legit() {
    for (int i = 0; i < sizeof(signature); i++) {
        if (EEPROM.read(i) != signature[i])
            return false;
    }
    return true;
}

void Storage::erase() {
    for (int i = 0; i < sizeof(signature); i++) {
        EEPROM.write(i, signature[i]);
    }
    for (int i = sizeof(signature); i < memory_size; i++) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
}

void Storage::save_routing_matrix(routing_matrix *elements) {
    memcpy(routing_matrix_buffer, elements, 4 * 4 * 4);
    for (int i = 0; i < 4 * 4 * 4; i++)
        EEPROM.write(offset_routing_table + i, routing_matrix_buffer[i]);
    EEPROM.commit();
}

void Storage::load_routing_matrix(routing_matrix *new_routing_matrix) {
    for (int i = 0; i < 4 * 4 * 4; i++)
        routing_matrix_buffer[i] = EEPROM.read(offset_routing_table + i);
    memcpy(new_routing_matrix, routing_matrix_buffer, 4 * 4 * 4);
}

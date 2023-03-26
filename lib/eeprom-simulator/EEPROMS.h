//
// Created by bjo on 3/25/23.
//

#ifndef EEPROM_SIMULATOR_HPP
#define EEPROM_SIMULATOR_HPP
#include <cstdint>
#include <cstddef>
#include <iostream>

class EEPROMSClass {
public:
    EEPROMSClass(void) {};
    void begin(size_t size);
    uint8_t read(int const address);
    void write(int const address, uint8_t const val);
    bool commit();
    bool end();
private:
    std::fstream *file;
    size_t internal_size;
};

#endif //EEPROM_SIMULATOR_HPP

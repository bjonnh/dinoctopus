//
// Created by bjo on 3/25/23.
//

#include <iostream>
#include <fstream>
#include "EEPROMS.h"

std::fstream efile;

void EEPROMSClass::begin(size_t size) {
    internal_size = size;

    efile.open("eeprom.bin", std::ios::binary | std::ios::out | std::ios::in);
    efile.seekg (0, efile.end);
    int length = efile.tellg();
    efile.seekg (0, efile.beg);
    if(length<size || length==-1)
    {
        std::cout << "Creating a new eeprom file" << std::endl;
        efile.close();
        efile.open("eeprom.bin", std::ios::binary | std::ios::out);
        for (int i=0 ; i<size;i++)
            write(i, 0);
        efile.close();
        efile.open("eeprom.bin", std::ios::binary | std::ios::out | std::ios::in);
    }


    efile.seekg (0, efile.end);
    length = efile.tellg();
    efile.seekg (0, efile.beg);
    std::cout << "EEPROM File has length: "<< length << std::endl;
}

uint8_t EEPROMSClass::read(int const address) {
    efile.seekg(address%internal_size);
    char x = 0;
    efile.read((&x), 1);
    return x;
}

void EEPROMSClass::write(int const address, uint8_t const val) {
    efile.seekg(address%internal_size);
    efile.write(reinterpret_cast<const char *>(&val), 1);
}

bool EEPROMSClass::commit() {
    efile.sync();
    return true;
}

bool EEPROMSClass::end() {
    efile.close();
    return true;
}

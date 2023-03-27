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

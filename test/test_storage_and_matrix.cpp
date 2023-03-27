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

#include <unity.h>
#include <iostream>

#ifdef ARDUINO
#include <Arduino.h>
#include <SPI.h>
#endif

#include "routingmatrix.hpp"
#include "storage.hpp"
#include "../lib/routing-matrix/routingmatrix.hpp"
#include "../lib/storage/storage.hpp"

void setUp(void)
{
    std::cout << "Setting up" << std::endl;
}

void tearDown(void) {
    std::cout << std::endl << "Tearing down" << std::endl;
}

void test_request(void)
{
    storage.init();
    storage.erase();
    RoutingMatrix r;
    storage.load_routing_matrix(r);
    // Test 0 by default
    for (int i=0;i<ROUTING_MATRIX_SIZE;i++)
        for (int j=0;j<ROUTING_MATRIX_SIZE;j++)
            TEST_ASSERT_EQUAL(0, r.get_element_2d(i,j));
    // Test setting values
    for (int i=0;i<ROUTING_MATRIX_SIZE;i++) {
        for (int j = 0; j < ROUTING_MATRIX_SIZE; j++) {
            r.set_element_2d(i, j, i * j + i);
        }
    }
    storage.save_routing_matrix(r);
    for (int i=0;i<ROUTING_MATRIX_SIZE;i++)
        for (int j=0;j<ROUTING_MATRIX_SIZE;j++)
            TEST_ASSERT_EQUAL(i * j + i, r.get_element_2d(i, j));
    // Test loading values
    for (int i=0;i<ROUTING_MATRIX_SIZE;i++)
        for (int j=0;j<ROUTING_MATRIX_SIZE;j++)
            r.set_element_2d(i,j, 0);
    storage.load_routing_matrix(r);
    for (int i=0;i<ROUTING_MATRIX_SIZE;i++)
        for (int j=0;j<ROUTING_MATRIX_SIZE;j++)
            TEST_ASSERT_EQUAL(i*j+i, r.get_element_2d(i,j));
    // Test overflows
    for (int i=0;i<ROUTING_MATRIX_SIZE*2;i++)
        for (int j=0;j<ROUTING_MATRIX_SIZE*2;j++)
            r.set_element_2d(i,j, 0);
    for (int i=0;i<ROUTING_MATRIX_SIZE*2;i++)
        for (int j=0;j<ROUTING_MATRIX_SIZE*2;j++)
            TEST_ASSERT_EQUAL(0, r.get_element_2d(i,j));
}
#ifdef ARDUINO
void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_request);

    UNITY_END();
}
void loop()
{

}
#else
int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_request);

    return UNITY_END();
}
#endif

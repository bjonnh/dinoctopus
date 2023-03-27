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

void test_storage_and_matrix(void);

void setUp(void)
{
    std::cout << "Setting up" << std::endl;

}

void tearDown(void) {
}

void tests() {
    RUN_TEST(test_storage_and_matrix);
}
#ifdef ARDUINO
void setup() {
    delay(2000);
    UNITY_BEGIN();
    tests();
    UNITY_END();
}
void loop()
{

}
#else
int main(int argc, char **argv)
{
    UNITY_BEGIN();
    tests();

    return UNITY_END();
}
#endif

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
    /*When(Method(ArduinoFake(Client), stop)).AlwaysReturn();
    When(Method(ArduinoFake(Client), available)).Return(1, 1, 1, 0);
    When(OverloadedMethod(ArduinoFake(Client), read, int())).Return(2, 0, 0);
    When(OverloadedMethod(ArduinoFake(Client), connect, int(const char*, uint16_t))).Return(1);

    Client* clientMock = ArduinoFakeMock(Client);

    MyService service(clientMock);

    String response = service.request("myserver.com");

    TEST_ASSERT_EQUAL(3, response.length());
    TEST_ASSERT_TRUE(response.equals("200"));

    Verify(Method(ArduinoFake(Client), stop)).Once();
    Verify(Method(ArduinoFake(Client), available)).Exactly(4_Times);
    Verify(OverloadedMethod(ArduinoFake(Client), read, int())).Exactly(3_Times);
    Verify(OverloadedMethod(ArduinoFake(Client), connect, int(const char*, uint16_t)).Using("myserver.com", 80)).Once();*/
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

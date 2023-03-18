//
// Created by bjo on 3/11/23.
//

#ifndef DINOCTOPUS_2040_UI_HPP
#define DINOCTOPUS_2040_UI_HPP


#include "../routingmatrix.hpp"

#define UI_LINE_HEIGHT 8

namespace UI {
    class Manager {
    public:
        void init();

        void loop();

        void set_latency(uint8_t cpu, uint32_t value);

        bool query_for_router();

        void set_routing_response(routing_matrix &matrix);

        void encoder_click();

        void encoder_right();

        void encoder_left();

        bool update_for_router();

        routing_matrix *current_route();

        void has_update_for_router();

        void errorStrip();

        static void dirtyStrip();

        static void normalStrip();

    private:
        char buffer[50];

        void initStrip();

        void initLCD();

        void initEncoder();

        void display_update();

        uint8_t latency_cpu[2] = {0, 0};

        void encoderPoll();

        bool query_for_router_requested;

        bool update_for_router_ready;

        bool updated=true;

        void readEncoder();

        bool dirty=true;

    };
}


#endif //DINOCTOPUS_2040_UI_HPP

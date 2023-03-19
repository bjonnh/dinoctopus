//
// Created by bjo on 3/11/23.
//

#ifndef DINOCTOPUS_2040_UI_HPP
#define DINOCTOPUS_2040_UI_HPP


#include "../routingmatrix.hpp"

#define UI_LINE_HEIGHT 8
#define UI_TYPE U8G2

namespace UI {
    class Manager {
    public:
        void init();

        void loop();

        void set_latency(uint8_t cpu, uint32_t value);

        bool query_for_router();

        static void set_routing_response(routing_matrix &matrix);

        void encoder_click();

        void encoder_right();

        void encoder_left();

        bool update_for_router();

        static routing_matrix *current_route();

        void has_update_for_router();

        [[maybe_unused]] static void errorStrip();

        static void dirtyStrip();

        static void normalStrip();

        bool latency_watch = false;
    private:
        char buffer[50] = {0};

        uint8_t latency_cpu[2] = {0, 0};

        bool updated = true;

        bool query_for_router_requested = false;

        bool update_for_router_ready = false;

        bool dirty = true;

        static void initStrip();

        static void initLCD();

        static void initEncoder();

        void display_update();

        void encoderPoll();

        void readEncoder();

    };
}

#endif //DINOCTOPUS_2040_UI_HPP

//
// Created by bjo on 3/11/23.
//
#include <cstdint>  // necessary for uint8_t
#include "../../lib/routing-matrix/routingmatrix.hpp"

#ifndef DINOCTOPUS_2040_MIDI_ROUTER_HPP
#define DINOCTOPUS_2040_MIDI_ROUTER_HPP

class MidiRouter {
public:
    static void init();

    void loop();

    void get_matrix(uint8_t *);

    void set_matrix(uint8_t *);

private:
    static uint8_t current_cable_limited();

    RoutingMatrix matrix;
};

#endif //DINOCTOPUS_2040_MIDI_ROUTER_HPP

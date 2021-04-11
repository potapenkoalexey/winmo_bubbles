#pragma once

#include <thread>

#include "../engine/engine.hxx"
#include "./global_variables.hxx"
#include "states/classic_state.hxx"
#include "states/extreme_state.hxx"
#include "states/game_over_state.hxx"
#include "states/level_complete_state.hxx"
#include "states/select_mode_state.hxx"

using clock_timer = std::chrono::high_resolution_clock;
using nano_sec = std::chrono::nanoseconds;
using milli_sec = std::chrono::milliseconds;
using time_point = std::chrono::time_point<clock_timer, nano_sec>;

#define FRAME_TIME 15

class game final {
public:
    game();

    ~game();

    int run();

    game(game const&) = delete;
    game& operator=(game const&) = delete;
    game(game&&) = delete;
    game& operator=(game&&) = delete;

private:
    clock_timer timer;
    time_point start;
    grottans::engine* engine;
};

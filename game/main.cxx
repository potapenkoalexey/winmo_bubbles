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

int main(int /*argc*/, char** /*argv*/)
{
    clock_timer timer;

    time_point start = timer.now();

    grottans::engine* engine = grottans::create_engine();

    //engine->set_window_size(480, 536);

    engine->initialize();

    engine->disable_mouse_moution_event();

    //loading states in the engine (simple state-machine)
    //using this index we can switch the states among themselves
    engine->push_state_and_init(select_mode_state::instance()); //0
    engine->push_state_and_init(classic_state::instance()); //1
    engine->push_state_and_init(extreme_state::instance()); //2
    engine->push_state_and_init(level_complete_state::instance()); //3
    engine->push_state_and_init(game_over_state::instance()); //4
    //set a first current state
    engine->current_state = engine->states[0];

    //main loop
    while (engine->loop) {
        time_point end_last_frame = timer.now();

        engine->current_state->handle_events();
        engine->current_state->update();
        engine->current_state->draw();

        g_frame_delta = std::chrono::duration_cast<milli_sec>(end_last_frame - start);
        if (g_frame_delta.count() < FRAME_TIME) { //~60fps
            std::this_thread::yield();
        }
        start = end_last_frame;
    }

    engine->uninitialize();

    grottans::destroy_engine(engine);

    return EXIT_SUCCESS;
}

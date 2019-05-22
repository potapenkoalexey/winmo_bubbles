#include <thread>

#include "../../src/engine/engine.hxx"
#include "./classic_state.hxx"
#include "./extreme_state.hxx"
#include "./game_over_state.hxx"
#include "./level_complete_state.hxx"
#include "./select_mode_state.hxx"

using clock_timer = std::chrono::high_resolution_clock;
using nano_sec = std::chrono::nanoseconds;
using milli_sec = std::chrono::milliseconds;
using time_point = std::chrono::time_point<clock_timer, nano_sec>;

int main(int /*argc*/, char** /*argv*/)
{
    clock_timer timer;
    time_point start = timer.now();

    grottans::engine* engine = grottans::create_engine();

    ///initialize the engine
    engine->initialize();

    engine->disable_mouse_moution_event();

    ///loading states in vector
    engine->push_state(select_mode_state::instance()); //0
    engine->push_state(classic_state::instance()); //1
    engine->push_state(extreme_state::instance()); //2
    engine->push_state(level_complete_state::instance()); //3
    engine->push_state(game_over_state::instance()); //4
    engine->current_state = engine->states[0];

    ///main loop
    while (engine->loop) {
        time_point end_last_frame = timer.now();

        engine->current_state->handle_events(engine);
        engine->current_state->update(engine);
        engine->current_state->draw(engine);

        milli_sec frame_delta = std::chrono::duration_cast<milli_sec>(end_last_frame - start);
        if (frame_delta.count() < 15) {
            std::this_thread::yield();
        }
        start = end_last_frame;
    }

    engine->uninitialize();

    return EXIT_SUCCESS;
}

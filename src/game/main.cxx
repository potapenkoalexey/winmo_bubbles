#include <thread>

#include "../../src/engine/engine.hxx"
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

    ///loading intro state
    engine->push_state(select_mode_state::instance());

    ///main loop
    while (engine->loop) {
        time_point end_last_frame = timer.now();

        engine->states.back()->handle_events(engine);
        engine->states.back()->update(engine);
        engine->states.back()->draw(engine);

        milli_sec frame_delta = std::chrono::duration_cast<milli_sec>(end_last_frame - start);
        if (frame_delta.count() < 15) {
            std::this_thread::yield();
        }
        start = end_last_frame;
    }

    engine->uninitialize();

    return EXIT_SUCCESS;
}

#include <fstream>

#include "./select_mode_state.hxx"
#include "game_over_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

game_over_state game_over_state::m_game_over_state;

bool game_over_state::init(grottans::engine* engine)
{
    //block_classic = new block;
    block_back = std::unique_ptr<block>(new block);

    block_back->texture = engine->create_texture("./data/images/my/game_over.png");

    // loading vertex_buffers from files
    std::ifstream file("./data/vertex_buffers/vert_buffers_for_full_monitor.txt");
    if (!file) {
        std::cerr << "can't load vert_buffers_for_full_monitor.txt\n";
        return EXIT_FAILURE;
    } else {
        file >> tr[0] >> tr[1];
        if (!sizeof(tr[1])) {
            std::cerr << "can't create vertex buffer\n";
            return EXIT_FAILURE;
        }
    }
    file.close();

    block_back->v_buf = engine->create_vertex_buffer(&tr[0], 2);

    ///playing sound
    sound_game_over = uni_ptr_sound(engine->create_sound_buffer("./data/sounds/03_game_over"));
    sound_game_over->play(grottans::sound_buffer::properties::once);

    return EXIT_SUCCESS;
}

void game_over_state::cleanup(grottans::engine*)
{
}

void game_over_state::pause(grottans::engine*)
{
}

void game_over_state::resume(grottans::engine*)
{
}

void game_over_state::handle_events(grottans::engine* engine)
{
    grottans::event e;

    engine->input(/*out*/ e);

    switch (e) {
    case grottans::event::turn_off: {
        engine->loop = false;
        break;
    }
    case grottans::event::mouse_motion: {
        //how to skip this iteration (render) of main loop?
        break;
    }
    case grottans::event::start_released: {
        while (engine->states.size()) {
            engine->pop_state();
        }
        engine->push_state(select_mode_state::instance());
        break;
    }
    }
}

void game_over_state::update(grottans::engine*)
{
}

void game_over_state::draw(grottans::engine* engine)
{
    engine->render(*block_back->v_buf, block_back->texture, block_back->move * engine->scale);
    engine->swap_buffers();
}

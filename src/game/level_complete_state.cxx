#include <fstream>

#include "./global_variables.hxx"
#include "./level_complete_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

level_complete_state level_complete_state::m_level_complete_state;

bool level_complete_state::init(grottans::engine* engine)
{
    block_back = std::unique_ptr<block>(new block);

    level_number = g_LEVEL;

    tex_even = engine->create_texture("./data/images/my/level_even.png");
    tex_uneven = engine->create_texture("./data/images/my/level_uneven.png");

    // loading vertex_buffers from files
    std::ifstream file("./data/vertex_buffers/vert_buffers_for_full_monitor.txt");
    if (!file) {
        std::cerr << "can't load vert_buffers_for_full_monitor.txt\n";
        return EXIT_FAILURE;
    } else {
        file >> tr[0] >> tr[1];
        if (!sizeof(tr[1])) {
            std::cerr << "can't create vertex buffer for level complete\n";
            return EXIT_FAILURE;
        }
    }
    file.close();

    block_back->v_buf = engine->create_vertex_buffer(&tr[0], 2);

    ///sounds
    sound_even = engine->create_sound_buffer("./data/sounds/07_level_even");
    sound_uneven = engine->create_sound_buffer("./data/sounds/04_level_uneven");

    if (g_LEVEL % 2) {
        block_back->texture = tex_even;
        sound_even->play(grottans::sound_buffer::properties::once);
    } else {
        block_back->texture = tex_uneven;
        sound_uneven->play(grottans::sound_buffer::properties::once);
    }
    return EXIT_SUCCESS;
}

void level_complete_state::cleanup(grottans::engine*) {}

void level_complete_state::pause(grottans::engine*) {}

void level_complete_state::resume(grottans::engine* engine)
{
    level_number = g_LEVEL;

    if (level_number % 2) {
        block_back->texture = tex_even;
        sound_even->play(grottans::sound_buffer::properties::once);
    } else {
        block_back->texture = tex_uneven;
        sound_uneven->play(grottans::sound_buffer::properties::once);
    }
}

void level_complete_state::handle_events(grottans::engine* engine)
{
    grottans::event e;

    engine->input(/*out*/ e);

    switch (e) {
    case grottans::event::turn_off: {
        engine->loop = false;
        break;
    }
    case grottans::event::start_released: {
        engine->swap_last_two_states();
        break;
    }
    }
}

void level_complete_state::update(grottans::engine*)
{
}

void level_complete_state::draw(grottans::engine* engine)
{
    engine->render(*block_back->v_buf, block_back->texture, block_back->move * engine->scale);
    engine->swap_buffers();
}

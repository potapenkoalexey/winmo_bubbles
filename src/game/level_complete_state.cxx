#include <fstream>

#include "./global_variables.hxx"
#include "./level_complete_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

level_complete_state level_complete_state::m_level_complete_state;

bool level_complete_state::init(grottans::engine* engine)
{
    block_back = std::unique_ptr<block>(new block);

    ///counter
    m_counter = std::unique_ptr<counter>(new counter);
    m_counter->set_quantity_of_digits(2, counter::sign::unsign);
    m_counter->init(engine);
    m_counter->set_vertexes(0.13f, -0.385f, 0.19f, 0.19f);
    m_counter->set_color({ 1.0f, 1.0f, 1.0f, 1.0f });
    m_counter->set_vertex_buffer(engine);
    m_counter->set_displayed_number(0);
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

    return EXIT_SUCCESS;
}

void level_complete_state::cleanup(grottans::engine*) {}

void level_complete_state::pause(grottans::engine*) {}

void level_complete_state::resume(grottans::engine* engine)
{
    level_number = g_LEVEL;

    m_counter->set_displayed_number(level_number);

    ///select sound even/uneven
    if (level_number % 2) {
        block_back->texture = tex_even;
        if (g_SOUND) {
            sound_even->play(grottans::sound_buffer::properties::once);
        }
    } else {
        block_back->texture = tex_uneven;
        if (g_SOUND) {
            sound_uneven->play(grottans::sound_buffer::properties::once);
        }
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
    case grottans::event::mouse_pressed: {
        if (g_MODE == MODE::classic) {
            ///go to classic_mode new level
            engine->switch_to_state(engine->states[1]);
        } else {
            ///go to extreme_mode new level
            engine->switch_to_state(engine->states[2]);
            //break;
        }
        break;
    }
    case grottans::event::start_released: {
        if (g_MODE == MODE::classic) {
            ///go to classic_mode new level
            engine->switch_to_state(engine->states[1]);
        } else {
            ///go to extreme_mode new level
            engine->switch_to_state(engine->states[2]);
            //break;
        }
        break;
    }
    }
}

void level_complete_state::update(grottans::engine*)
{
}

void level_complete_state::draw(grottans::engine* engine)
{
    block_back->draw(engine);

    m_counter->draw(engine);

    engine->swap_buffers();
}

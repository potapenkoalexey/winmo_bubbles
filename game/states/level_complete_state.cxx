#include <fstream>

#include "../global_variables.hxx"
#include "level_complete_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

level_complete_state level_complete_state::m_level_complete_state;

level_complete_state::~level_complete_state()
{
    delete tex_even;
    delete tex_uneven;
    delete block_back->v_buf;

    delete sound_even;
    delete sound_uneven;
}

bool level_complete_state::init(grottans::engine* e)
{
    engine = e;

    block_back = std::make_unique<block>(engine);

    ///counter
    counter_level = std::make_unique<counter>(engine);
    counter_level->set_quantity_of_digits(2, counter::sign::unsign);
    counter_level->init();
    counter_level->set_vertexes(0.13f, -0.385f, 0.19f, 0.19f);
    counter_level->set_color({ 1.0f, 1.0f, 1.0f, 1.0f });
    counter_level->set_vertex_buffer();
    counter_level->set_displayed_number(0);
    
    level_number = g_LEVEL;

    tex_even = engine->create_texture("./data/images/gui/level_even.png");
    tex_uneven = engine->create_texture("./data/images/gui/level_uneven.png");

    auto text = engine->load_txt_and_filter_comments("./data/vertex_buffers/vert_buffers_for_full_monitor.txt");
    text >> vert_buf_tr[0] >> vert_buf_tr[1];

    block_back->v_buf = engine->create_vertex_buffer(&vert_buf_tr[0], 2);

    ///sounds
    sound_even = engine->create_sound_buffer("./data/sounds/07_level_even");
    sound_uneven = engine->create_sound_buffer("./data/sounds/04_level_uneven");

    return EXIT_SUCCESS;
}

void level_complete_state::cleanup() {}

void level_complete_state::pause() {}

void level_complete_state::resume()
{
    level_number = g_LEVEL;

    counter_level->set_displayed_number(level_number);

    ///select sound even/uneven
    if (level_number % 2) {
        block_back->texture = tex_even;
        if (g_SOUND && level_number > 1) {
            sound_even->play(grottans::sound_buffer::properties::once);
        }
    } else {
        block_back->texture = tex_uneven;
        if (g_SOUND && level_number > 1) {
            sound_uneven->play(grottans::sound_buffer::properties::once);
        }
    }
}

void level_complete_state::handle_events()
{
    grottans::event e;

    engine->input(/*out*/ e);

    switch (e) {
    case grottans::event::turn_off: {
        engine->loop = false;
        break;
    }
    case grottans::event::mouse_left_released: {
        if (g_MODE == MODE::classic) {
            ///go to classic_mode new level
            engine->switch_to_state_and_resume(engine->states[1]);
        } else {
            ///go to extreme_mode new level
            engine->switch_to_state_and_resume(engine->states[2]);
        }
        break;
    }
    case grottans::event::start_released: {
        if (g_MODE == MODE::classic) {
            ///go to classic_mode new level
            engine->switch_to_state_and_resume(engine->states[1]);
        } else {
            ///go to extreme_mode new level
            engine->switch_to_state_and_resume(engine->states[2]);
        }
        break;
    }
    case grottans::event::escape_released: {
        //go to the select_game_mode
        engine->switch_to_state_and_resume(engine->states[0]);
        break;
    }
    default:
        break;
    }
}

void level_complete_state::update()
{
}

void level_complete_state::draw()
{
    block_back->draw();

    counter_level->draw();

    engine->swap_buffers();
}

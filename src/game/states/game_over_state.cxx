#include <fstream>

#include "../global_variables.hxx"
#include "./game_over_state.hxx"
#include "./select_mode_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

game_over_state game_over_state::m_game_over_state;

bool game_over_state::init(grottans::engine* engine)
{
    //block_classic = new block;
    block_back = std::unique_ptr<block>(new block);

    block_back->texture = engine->create_texture("./data/images/my/game_over.png");

    auto text = engine->load_txt_and_filter_comments("./data/vertex_buffers/vert_buffers_for_full_monitor.txt");
    text >> tr[0] >> tr[1];

    block_back->v_buf = engine->create_vertex_buffer(&tr[0], 2);

    ///counter
    m_counter = std::unique_ptr<counter>(new counter);
    m_counter->set_quantity_of_digits(5, counter::sign::unsign);
    m_counter->init(engine);
    m_counter->set_vertexes(-0.40f, -0.60f, 0.16f, 0.16f);
    m_counter->set_color({ 1.0f, 1.0f, 1.0f, 1.0f });
    m_counter->set_vertex_buffer(engine);
    m_counter->set_displayed_number(0);
    m_counter->set_hide_zeros(false);

    ///playing sound
    sound_game_over = uni_ptr_sound(engine->create_sound_buffer("./data/sounds/03_game_over"));

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
    g_score_in_the_end_of_level = 0;

    m_counter->set_displayed_number(g_SCORE);

    if (g_SOUND) {
        sound_game_over->play(grottans::sound_buffer::properties::once);
    }
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
    case grottans::event::mouse_released: {
        ///go to select_mode_state
        engine->switch_to_state(engine->states[0]);
        break;
    }
    case grottans::event::start_released: {
        ///go to select_mode_state
        engine->switch_to_state(engine->states[0]);
        break;
    }
    }
}

void game_over_state::update(grottans::engine*)
{
}

void game_over_state::draw(grottans::engine* engine)
{
    block_back->draw(engine);
    m_counter->draw(engine);

    engine->swap_buffers();
}
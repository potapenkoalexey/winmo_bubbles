#include <fstream>

#include "../global_variables.hxx"
#include "./game_over_state.hxx"
#include "./select_mode_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

game_over_state game_over_state::m_game_over_state;

game_over_state::~game_over_state()
{
    delete sound_game_over;
}

bool game_over_state::init(grottans::engine* e)
{
    engine = e;

    //block_classic = new block;
    block_back = std::make_unique<block>(engine);

    block_back->texture = engine->create_texture("./data/images/gui/game_over.png");

    auto text = engine->load_txt_and_filter_comments("./data/vertex_buffers/vert_buffers_for_full_monitor.txt");
    text >> vert_buf_tr[0] >> vert_buf_tr[1];

    block_back->v_buf = engine->create_vertex_buffer(&vert_buf_tr[0], 2);

    ///counter
    counter_final_score = std::make_unique<counter>(engine);
    counter_final_score->set_quantity_of_digits(5, counter::sign::unsign);
    counter_final_score->init();
    counter_final_score->set_vertexes(-0.40f, -0.60f, 0.16f, 0.16f);
    counter_final_score->set_color({ 1.0f, 1.0f, 1.0f, 1.0f });
    counter_final_score->set_vertex_buffer();
    counter_final_score->set_displayed_number(0);
    counter_final_score->set_hide_zeros(false);

    ///playing sound
    sound_game_over = engine->create_sound_buffer("./data/sounds/03_game_over");

    return EXIT_SUCCESS;
}

void game_over_state::cleanup()
{
}

void game_over_state::pause()
{
}

void game_over_state::resume()
{
    g_score_in_the_end_of_level = 0;

    counter_final_score->set_displayed_number(g_SCORE);

    if (g_SOUND) {
        sound_game_over->play(grottans::sound_buffer::properties::once);
    }
}

void game_over_state::handle_events()
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
        engine->switch_to_state_and_resume(engine->states[0]);
        break;
    }
    case grottans::event::start_released: {
        ///go to select_mode_state
        engine->switch_to_state_and_resume(engine->states[0]);
        break;
    }
    default:
        break;
    }
}

void game_over_state::update()
{
}

void game_over_state::draw()
{
    block_back->draw();
    counter_final_score->draw();

    engine->swap_buffers();
}

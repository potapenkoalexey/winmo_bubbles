#include <fstream>

#include "../global_variables.hxx"
#include "./classic_state.hxx"
#include "./extreme_state.hxx"
#include "./select_mode_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

select_mode_state select_mode_state::m_select_mode_state;

select_mode_state::~select_mode_state()
{
    delete tex_back_sound_on;
    delete tex_back_sound_off;
    delete v_buf_classic;
    delete v_buf_extreme;
}

bool select_mode_state::init(grottans::engine* e)
{
    engine = e;

    block_back = std::unique_ptr<block>(new block(engine));
    block_select = std::unique_ptr<block>(new block(engine));

    tex_back_sound_on = engine->create_texture("./data/images/my/select_game_type.png");
    tex_back_sound_off = engine->create_texture("./data/images/my/select_game_type_no_sound.png");

    block_back->texture = tex_back_sound_on;
    block_select->texture = engine->create_texture("./data/images/my/main_select.png");

    /// loading vertex_buffers from files
    /// tr0-1 - for back
    /// tr2-3 - for classic mode selecter
    /// tr4-5 - for extreme mode selecter
    auto text = engine->load_txt_and_filter_comments("./data/vertex_buffers/vert_buffers_for_full_monitor.txt");
    text >> vert_buf[0] >> vert_buf[1] >> vert_buf[2] >> vert_buf[3] >> vert_buf[4] >> vert_buf[5];

    block_back->v_buf = engine->create_vertex_buffer(&vert_buf[0], 2);
    v_buf_classic = engine->create_vertex_buffer(&vert_buf[2], 2);
    v_buf_extreme = engine->create_vertex_buffer(&vert_buf[4], 2);
    block_select->v_buf = v_buf_classic;

    sound_on = uni_ptr_sound(engine->create_sound_buffer("./data/sounds/10_sound_on.wav"));

    return EXIT_SUCCESS;
}

void select_mode_state::cleanup() {}

void select_mode_state::pause() {}

void select_mode_state::resume()
{
    g_score_in_the_end_of_level = 0;
    g_LEVEL = 1;
    g_SCORE = 0;
}

void select_mode_state::sound_turn_on()
{
    sound_on->play(grottans::sound_buffer::properties::once);
    block_back->texture = tex_back_sound_on;
    g_SOUND = true;
}

void select_mode_state::sound_turn_off()
{
    block_back->texture = tex_back_sound_off;
    g_SOUND = false;
}

void select_mode_state::handle_mouse_event(
    grottans::engine* engine,
    grottans::event& e)
{
    float w = engine->get_window_width();
    float h = engine->get_window_height();
    size_t m_x = engine->mouse_coord_pressed.x;
    size_t m_y = engine->mouse_coord_pressed.y;
    float scale_x = engine->scale.col0.x; //0.625
    float scale_y = engine->scale.col1.y; //1

    if (w >= h) {
        ///handling sound button
        if ((m_y > 0.5f * h) && (m_y < 0.62f * h) && (m_x > 0.45f * w) && (m_x < 0.55f * w)) {
            if (g_SOUND) {
                sound_turn_off();
            } else {
                sound_turn_on();
            }
        }
        ///classic button
        if ((m_y > 0.68f * h) && (m_y < 0.79f * h) && (m_x > 0.26f * w) && (m_x < 0.49f * w)) {
            block_select->v_buf = v_buf_classic;
            g_MODE = MODE::classic;
            e = grottans::event::start_released;
        }
        ///extreme button
        if ((m_y > 0.68f * h) && (m_y < 0.79f * h) && (m_x > 0.52f * w) && (m_x < 0.75f * w)) {
            block_select->v_buf = v_buf_extreme;
            g_MODE = MODE::extreme;
            e = grottans::event::start_released;
        }
    }
    if (w < h) {
        ///handling sound button
        if ((m_y > 0.5f * h) && (m_y < 0.58f * h) && (m_x > 0.45f * w) && (m_x < 0.55f * w)) {
            if (g_SOUND) {
                sound_turn_off();
            } else {
                sound_turn_on();
            }
        }
        ///classic button
        if ((m_y > 0.58f * h) && (m_y < 0.68f * h) && (m_x > 0.10f * w) && (m_x < 0.48f * w)) {
            block_select->v_buf = v_buf_classic;
            g_MODE = MODE::classic;
            e = grottans::event::start_released;
        }
        ///extreme button
        if ((m_y > 0.58f * h) && (m_y < 0.68f * h) && (m_x > 0.52f * w) && (m_x < 0.90f * w)) {
            block_select->v_buf = v_buf_extreme;
            g_MODE = MODE::extreme;
            e = grottans::event::start_released;
        }
    }
}

void select_mode_state::handle_events()
{
    grottans::event e;
    engine->input(e);

    if (e == grottans::event::mouse_pressed) {
        handle_mouse_event(engine, e);
    }
    switch (e) {
    case grottans::event::turn_off: {
        engine->loop = false;
        break;
    }
    case grottans::event::escape_released: {
        engine->loop = false;
        break;
    }
    case grottans::event::left_released: {
        block_select->v_buf = v_buf_classic;
        g_MODE = MODE::classic;
        break;
    }
    case grottans::event::right_released: {
        block_select->v_buf = v_buf_extreme;
        g_MODE = MODE::extreme;
        break;
    }
    case grottans::event::up_released: {
        if (g_SOUND) {
            sound_turn_off();
        } else {
            sound_turn_on();
        }
        break;
    }
    case grottans::event::start_released: {

        engine->switch_to_state_and_resume(engine->states[3]);

        break;
    }
    }
}

void select_mode_state::update()
{
}

void select_mode_state::draw()
{
    block_back->draw();
    block_select->draw();

    engine->swap_buffers();
}

#include <fstream>

#include "../global_variables.hxx"
#include "./classic_state.hxx"
#include "./extreme_state.hxx"
#include "./select_mode_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

select_mode_state select_mode_state::m_select_mode_state;

select_mode_state::~select_mode_state()
{
    delete tex_back;
    delete tex_sound_on;
    delete tex_sound_off;
    delete tex_selector;
    delete block_back->v_buf;
    delete block_sound->v_buf;
    delete block_select->v_buf;

    delete sound_on;
}

bool select_mode_state::init(grottans::engine* e)
{
    engine = e;

    block_back = std::make_unique<block>(engine);
    block_select = std::make_unique<block>(engine);
    block_sound = std::make_unique<block>(engine);

    tex_back = engine->create_texture("./data/images/gui/select_game_type.png");
    tex_sound_on = engine->create_texture("./data/images/gui/sound_on.png");
    tex_sound_off = engine->create_texture("./data/images/gui/sound_off.png");
    tex_selector = engine->create_texture("./data/images/gui/main_select.png");

    block_back->texture = tex_back;
    block_select->texture = tex_selector;
    block_sound->texture = tex_sound_on;

    /// loading vertex_buffers from files
    /// tr0-1 - for background image
    /// tr2-3 - for classic mode button
    /// tr4-5 - for extreme mode button
    /// tr6-7 - for sound button
    /// tr8-9 - for load button
    auto text = engine->load_txt_and_filter_comments("./data/vertex_buffers/vert_buffers_for_full_monitor.txt");
    text >> vert_buf[0] >> vert_buf[1]
         >> vert_buf[2] >> vert_buf[3]
         >> vert_buf[4] >> vert_buf[5]
         >> vert_buf[6] >> vert_buf[7]
         >> vert_buf[8] >> vert_buf[9] ;

    block_back->v_buf = engine->create_vertex_buffer(&vert_buf[0], 2);

    v_buf_classic = engine->create_vertex_buffer(&vert_buf[2], 2);
    v_buf_extreme = engine->create_vertex_buffer(&vert_buf[4], 2);
    v_buf_load_save = engine->create_vertex_buffer(&vert_buf[8], 2);
    block_select->v_buf = v_buf_classic;

    block_sound->v_buf = engine->create_vertex_buffer(&vert_buf[6], 2);

    sound_on = engine->create_sound_buffer("./data/sounds/10_sound_on.wav");

    return EXIT_SUCCESS;
}

void select_mode_state::cleanup() {}

void select_mode_state::pause() {}

void select_mode_state::resume()
{
    g_LEVEL = 1;
    g_SCORE = 0;
    g_score_in_the_end_of_level = 0;
}

void select_mode_state::sound_turn_on()
{
    sound_on->play(grottans::sound_buffer::properties::once);
    block_sound->texture = tex_sound_on;
    g_SOUND = true;
}

void select_mode_state::sound_turn_off()
{
    block_sound->texture = tex_sound_off;
    g_SOUND = false;
}

void select_mode_state::handle_mouse_event(
    grottans::engine* /*eng*/,
    grottans::event& e)
{
    float w = engine->get_window_width();
    float h = engine->get_window_height();

    if (w >= h) { ///landscape mode
        ///handling sound button
        if (engine->is_mouse_clicked_in_coord(0.45f, 0.55f, 0.5f, 0.62f)) {
            if (g_SOUND) {
                sound_turn_off();
            } else {
                sound_turn_on();
            }
        }
        ///classic button
        if (engine->is_mouse_clicked_in_coord(0.26f, 0.49f, 0.68f, 0.79f)) {
            block_select->v_buf = v_buf_classic;
            engine->load_original_settings();
            g_MODE = MODE::classic;
            e = grottans::event::start_released;
        }
        ///extreme button
        if (engine->is_mouse_clicked_in_coord(0.52f, 0.75f, 0.68f, 0.79f)) {
            block_select->v_buf = v_buf_extreme;
            engine->load_original_settings();
            g_MODE = MODE::extreme;
            e = grottans::event::start_released;
        }
        ///load_save button
        if (engine->is_mouse_clicked_in_coord(0.39f, 0.63f, 0.81f, 0.92f)) {
            block_select->v_buf = v_buf_load_save;
            engine->load_saved_settings();
            g_LOAD_SAVED_STATE = true;
            e = grottans::event::start_released;
        }
    }
    if (w < h) { ///portrait mode
        ///handling sound button
        if (engine->is_mouse_clicked_in_coord(0.45f, 0.55f, 0.50f, 0.58f)) {
            if (g_SOUND) {
                sound_turn_off();
            } else {
                sound_turn_on();
            }
        }
        ///classic button
        if (engine->is_mouse_clicked_in_coord(0.10f, 0.48f, 0.58f, 0.68f)) {
            block_select->v_buf = v_buf_classic;
            g_MODE = MODE::classic;
            e = grottans::event::start_released;
        }
        ///extreme button
        if (engine->is_mouse_clicked_in_coord(0.52f, 0.90f, 0.58f, 0.68f)) {
            block_select->v_buf = v_buf_extreme;
            g_MODE = MODE::extreme;
            e = grottans::event::start_released;
        }
        ///load_save button
        if (engine->is_mouse_clicked_in_coord(0.31f, 0.69f, 0.70f, 0.80f)) {
            block_select->v_buf = v_buf_load_save;
            engine->load_saved_settings();
            g_LOAD_SAVED_STATE = true;
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
        engine->load_original_settings();
        g_MODE = MODE::classic;
        g_LOAD_SAVED_STATE = false;
        break;
    }
    case grottans::event::right_released: {
        block_select->v_buf = v_buf_extreme;
        engine->load_original_settings();
        g_MODE = MODE::extreme;
        g_LOAD_SAVED_STATE = false;
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
    case grottans::event::down_released:{
        block_select->v_buf = v_buf_load_save;
        // set and load save state
        engine->load_saved_settings();
        g_LOAD_SAVED_STATE = true;
        break;
    }
    case grottans::event::start_released: {
        ///transfer to level_complete_state
        engine->switch_to_state_and_resume(engine->states[3]);

        break;
    }
    default:
        break;
    }
}

void select_mode_state::update()
{
}

void select_mode_state::draw()
{
    block_back->draw();
    block_select->draw();
    block_sound->draw();

    engine->swap_buffers();
}

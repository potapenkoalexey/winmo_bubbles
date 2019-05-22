#include <fstream>

#include "./classic_state.hxx"
#include "./extreme_state.hxx"
#include "./global_variables.hxx"
#include "./select_mode_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

select_mode_state select_mode_state::m_select_mode_state;

bool select_mode_state::init(grottans::engine* engine)
{
    block_back = std::unique_ptr<block>(new block);
    block_select = std::unique_ptr<block>(new block);

    tex_back_sound_on = engine->create_texture("./data/images/my/select_game_type.png");
    tex_back_sound_off = engine->create_texture("./data/images/my/select_game_type_no_sound.png");

    block_back->texture = tex_back_sound_on;
    block_select->texture = engine->create_texture("./data/images/my/main_select.png");

    /// loading vertex_buffers from files
    /// tr0-1 - for back
    /// tr2-3 - for classic mode selecter
    /// tr4-5 - for extreme mode selecter
    std::ifstream file("./data/vertex_buffers/vert_buffers_for_full_monitor.txt");
    if (!file) {
        std::cerr << "can't load vert_buffers_for_full_monitor.txt\n";
        return EXIT_FAILURE;
    } else {
        file >> tr[0] >> tr[1] >> tr[2] >> tr[3] >> tr[4] >> tr[5];
        if (!sizeof(tr[1])) {
            std::cerr << "can't create vertex buffer\n";
            return EXIT_FAILURE;
        }
    }
    file.close();

    block_back->v_buf = engine->create_vertex_buffer(&tr[0], 2);
    v_buf_classic = engine->create_vertex_buffer(&tr[2], 2);
    v_buf_extreme = engine->create_vertex_buffer(&tr[4], 2);
    block_select->v_buf = v_buf_classic;

    sound_on = uni_ptr_sound(engine->create_sound_buffer("./data/sounds/10_sound_on.wav"));
    //sound_on = engine->create_sound_buffer("./data/sounds/10_sound_on.wav"); //without uni_ptr

    return EXIT_SUCCESS;
}

void select_mode_state::cleanup(grottans::engine*)
{
}

void select_mode_state::pause(grottans::engine*)
{
}

void select_mode_state::resume(grottans::engine*)
{
}

void select_mode_state::handle_events(grottans::engine* engine)
{
    grottans::event e;
    engine->input(e);
    switch (e) {
    case grottans::event::turn_off: {
        engine->loop = false;
        break;
    }
    case grottans::event::mouse_released: {
        handle_mouse_event(engine, e);
        break;
    }
    case grottans::event::left_released: {
        block_select->v_buf = v_buf_classic;
        break;
    }
    case grottans::event::right_released: {
        block_select->v_buf = v_buf_extreme;
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
        if (block_select->v_buf == v_buf_classic) {
            engine->push_state(classic_state::instance());
        }
        if (block_select->v_buf == v_buf_extreme) {
            engine->push_state(extreme_state::instance());
        }

        break;
    }
    }
}

void select_mode_state::update(grottans::engine*)
{
}

void select_mode_state::draw(grottans::engine* engine)
{
    engine->render(*block_back->v_buf, block_back->texture, block_back->move * engine->scale);
    engine->render(*block_select->v_buf, block_select->texture, block_select->move * engine->scale);
    engine->swap_buffers();
}

void select_mode_state::handle_mouse_event(
    grottans::engine* engine,
    grottans::event& e)
{
    float w = engine->get_window_width();
    float h = engine->get_window_height();
    size_t m_x = engine->mouse_coord.x;
    size_t m_y = engine->mouse_coord.y;
    float scale_x = engine->scale.col0.x; //0.625
    float scale_y = engine->scale.col1.y; //1

    ///handling sound button
    if (m_y < 0.65f * h && m_y > 0.5f * h && m_x > 0.45f * w && m_x < 0.55f * w) {
        if (g_SOUND) {
            sound_turn_off();
        } else {
            sound_turn_on();
        }
    }

    ///handling classic button
    //if (m_y < 0.9 * h && m_y > 0.67 * h && m_x > 0.45 * w && m_x < 0.55 * w) {
    //}
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

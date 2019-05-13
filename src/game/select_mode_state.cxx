#include <fstream>

#include "./classic_state.hxx"
#include "./extreme_state.hxx"
#include "./select_mode_state.hxx"

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
    std::ifstream file("./data/vertex_buffers/vert_tex_color.txt");
    if (!file) {
        std::cerr << "can't load vert_tex_color.txt\n";
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

    sound_on = engine->create_sound_buffer("./data/sounds/10_sound_on.wav");

    width = engine->get_window_width();
    height = engine->get_window_height();

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
    case grottans::event::mouse_motion: {
        //how to skip iteration of main loop?
        //engine->mouse_moution = true;
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
        if (settings::SOUND < 0) {
            sound_on->play(grottans::sound_buffer::properties::once);
            block_back->texture = tex_back_sound_on;
            settings::SOUND *= -1;
        } else {
            block_back->texture = tex_back_sound_off;
            settings::SOUND *= -1;
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

#include <fstream>

#include "./classic_state.hxx"
#include "./select_mode_state.hxx"

select_mode_state select_mode_state::m_select_mode_state;

void select_mode_state::init(grottans::engine* engine)
{
    block_select = std::unique_ptr<block>(new block);

    block_select->texture = engine->create_texture("./data/images/my/select_game_type.png");

    // loading vertex_buffers from files
    std::ifstream file("./data/vertex_buffers/vert_tex_color.txt");
    if (!file) {
        std::cerr << "can't load vert_tex_color.txt\n";
        //return EXIT_FAILURE;
    } else {
        file >> tr[0] >> tr[1];
        if (!sizeof(tr[1])) {
            std::cerr << "can't create vertex buffer\n";
            //return EXIT_FAILURE;
        }
    }
    file.close();

    block_select->v_buf = engine->create_vertex_buffer(&tr[0], 2);

    sound_on = engine->create_sound_buffer("./data/sounds/10_sound_on.wav");

    width = engine->get_window_width();
    height = engine->get_window_height();
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
    case grottans::event::up_pressed: {
        sound_on->play(grottans::sound_buffer::properties::once);
        break;
    }
    case grottans::event::start_pressed: {
        engine->change_state(classic_state::instance());
        break;
    }
    }
}

void select_mode_state::update(grottans::engine*)
{
}

void select_mode_state::draw(grottans::engine* engine)
{
    if (engine->mouse_moution == false) {
        engine->render(*block_select->v_buf, block_select->texture, block_select->move * engine->scale);
        engine->swap_buffers();
        return;
    } else
        engine->mouse_moution = false;
}

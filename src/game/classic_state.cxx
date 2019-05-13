#include <fstream>

#include "./classic_state.hxx"
#include "./select_mode_state.hxx"

classic_state classic_state::m_classic_state;

void classic_state::init(grottans::engine* engine)
{
    //block_classic = new block;
    block_classic = std::unique_ptr<block>(new block);

    block_classic->texture = engine->create_texture("./data/images/my/select_game_type_no_sound.png");

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

    block_classic->v_buf = engine->create_vertex_buffer(&tr[0], 2);

    width = engine->get_window_width();
    height = engine->get_window_height();
}

void classic_state::cleanup(grottans::engine*)
{
}

void classic_state::pause(grottans::engine*)
{
}

void classic_state::resume(grottans::engine*)
{
}

void classic_state::handle_events(grottans::engine* engine)
{
    grottans::event e;

    engine->input(/*out*/ e);

    switch (e)
    {
    case grottans::event::turn_off: {
        engine->loop = false;
        break;
    }
    case grottans::event::mouse_motion: {
        //how to skip this iteration (render) of main loop?
        break;
    }
    case grottans::event::start_pressed: {
        engine->change_state(select_mode_state::instance());
        break;
    }
    }
}

void classic_state::update(grottans::engine*)
{
}

void classic_state::draw(grottans::engine* engine)
{
    engine->render(*block_classic->v_buf, block_classic->texture, block_classic->move * engine->scale);
    engine->swap_buffers();
}

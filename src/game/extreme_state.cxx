#include <fstream>

#include "./extreme_state.hxx"
#include "./game_over_state.hxx"
#include "./select_mode_state.hxx"

extreme_state extreme_state::m_extreme_state;

bool extreme_state::init(grottans::engine* engine)
{
    //block_classic = new block;
    block_back = std::unique_ptr<block>(new block);

    block_back->texture = engine->create_texture("./data/images/my/level_red.png");

    // loading vertex_buffers from files
    std::ifstream file("./data/vertex_buffers/vert_tex_color.txt");
    if (!file) {
        std::cerr << "can't load vert_tex_color.txt\n";
        return EXIT_FAILURE;
    } else {
        file >> tr[0] >> tr[1];
        if (!sizeof(tr[1])) {
            std::cerr << "can't create vertex buffer\n";
            return EXIT_FAILURE;
        }
    }
    file.close();

    block_back->v_buf = engine->create_vertex_buffer(&tr[0], 2);

    width = engine->get_window_width();
    height = engine->get_window_height();

    ///field
    game_field = std::unique_ptr<field>(new field);
    game_field->initialization(engine);
    game_field->fill_extreme();

    return EXIT_SUCCESS;
}

void extreme_state::cleanup(grottans::engine*)
{
}

void extreme_state::pause(grottans::engine*)
{
}

void extreme_state::resume(grottans::engine*)
{
}

void extreme_state::handle_events(grottans::engine* engine)
{
    grottans::event e;

    engine->input(/*out*/ e);

    switch (e) {
    case grottans::event::turn_off: {
        engine->loop = false;
        break;
    }
    case grottans::event::mouse_motion: {
        //how to skip this iteration (render) of main loop?
        break;
    }
    case grottans::event::escape_released: {
        engine->pop_state();
        break;
    }
    case grottans::event::start_released: {
        engine->push_state(game_over_state::instance());
        break;
    }
    }
}

void extreme_state::update(grottans::engine*)
{
}

void extreme_state::draw(grottans::engine* engine)
{
    //engine->render(*block_classic->v_buf, block_classic->texture, block_classic->move * engine->scale);
    game_field->render(engine);
    engine->swap_buffers();
}

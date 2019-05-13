#include <fstream>

#include "./classic_state.hxx"
#include "./game_over_state.hxx"
#include "./select_mode_state.hxx"

classic_state classic_state::m_classic_state;

bool classic_state::init(grottans::engine* engine)
{
    ///field
    game_field = std::unique_ptr<field>(new field);
    game_field->initialization(engine);
    game_field->fill_clasic();

    ///progress desk
    progress = std::unique_ptr<progress_desk>(new progress_desk);
    progress->init(engine);
    progress->set_line_in_null();
    //progress->set_line_in_full();

    return EXIT_SUCCESS;
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
    case grottans::event::left_released: {
        if (game_field->selector->position.x > 0) {
            game_field->selector->position.x -= 1.f;
        } else {
            game_field->selector->position.x = 9.f;
        }
        break;
    }
    case grottans::event::right_released: {
        if (game_field->selector->position.x < 9) {
            game_field->selector->position.x += 1.f;
        } else {
            game_field->selector->position.x = 0.f;
        }
        break;
    }
    case grottans::event::up_released: {
        if (game_field->selector->position.y > 0) {
            game_field->selector->position.y -= 1.f;
        } else {
            game_field->selector->position.y = 9.f;
        }
        break;
    }
    case grottans::event::down_released: {
        if (game_field->selector->position.y < 9) {
            game_field->selector->position.y += 1.f;
        } else {
            game_field->selector->position.y = 0.f;
        }
        break;
    }
    }
}

void classic_state::update(grottans::engine*)
{
}

void classic_state::draw(grottans::engine* engine)
{
    //engine->render(*block_classic->v_buf, block_classic->texture, block_classic->move * engine->scale);
    game_field->render(engine);
    progress->draw(engine);
    engine->swap_buffers();
}

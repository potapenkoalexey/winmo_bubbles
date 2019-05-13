#include <fstream>

#include "./extreme_state.hxx"
#include "./game_over_state.hxx"
#include "./select_mode_state.hxx"

extreme_state extreme_state::m_extreme_state;

bool extreme_state::init(grottans::engine* engine)
{
    ///field
    game_field = std::unique_ptr<field>(new field);
    game_field->initialization(engine);
    game_field->fill_extreme();

    ///progress desk
    progress = std::unique_ptr<progress_desk>(new progress_desk);
    progress->init(engine);

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

void extreme_state::update(grottans::engine*)
{
}

void extreme_state::draw(grottans::engine* engine)
{
    game_field->render(engine);

    progress->draw(engine);

    engine->swap_buffers();
}

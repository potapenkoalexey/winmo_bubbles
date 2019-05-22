#include <cmath>
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
    progress->set_line_in_null();

    return EXIT_SUCCESS;
}

void extreme_state::cleanup(grottans::engine*) {}

void extreme_state::pause(grottans::engine*) {}

void extreme_state::resume(grottans::engine*)
{
    game_field->fill_extreme();

    game_field->selector->position.x = 5;
    game_field->selector->position.y = 5;

    progress->set_line_in_null();
    progress->level_complete_flag = false;
}

void extreme_state::handle_events(grottans::engine* engine)
{
    grottans::event e;

    engine->input(/*out*/ e);

    if (e == grottans::event::mouse_released) {

        handle_mouse_event(engine);

        e = grottans::event::start_pressed;
    }

    switch (e) {
    case grottans::event::turn_off: {
        engine->loop = false;
        break;
    }
    case grottans::event::escape_released: {
        engine->clear_states();
        engine->push_state(select_mode_state::instance());
        break;
    }
    case grottans::event::start_released: {
        //engine->push_state(game_over_state::instance());
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

void extreme_state::update(grottans::engine*) {}

void extreme_state::draw(grottans::engine* engine)
{
    game_field->render(engine);

    progress->draw(engine);

    engine->swap_buffers();
}

void extreme_state::handle_mouse_event(grottans::engine* engine)
{
    size_t w = engine->get_window_width();
    size_t h = engine->get_window_height();
    double i = 0;
    double j = 0;

    if (w > h) {
        int block = h / 11;
        //find centr of the screen
        int centr_x = w / 2;
        //take mouse cursor coordintes in engine
        size_t m_x = engine->mouse_coord.x;
        size_t m_y = engine->mouse_coord.y;
        //find delta
        double delta_x = m_x - static_cast<double>(centr_x);
        //find delta in size block size
        delta_x = delta_x / static_cast<double>(block);

        j = 5 + delta_x;

        i = floor(m_y / static_cast<double>(h) * 11); // work !!!!!!!!!!!
        ///blocking missclicks on progress_desk

        if (j < 0 || j > 10 || i < 0 || i > 9)
            return;
    } else {
        int block = w / 11;
        int centr_y = h / 2 - block / 2;
        size_t m_x = engine->mouse_coord.x;
        size_t m_y = engine->mouse_coord.y;
        double delta_y = (m_y - static_cast<double>(centr_y)) / static_cast<double>(block);
        j = floor(m_x / static_cast<double>(w) * 11 - 0.5);
        i = 5 + delta_y;
        if (j < 0 || j > 9 || i > 10 || i < 0)
            return;
    }

    game_field->selector->position.x = j;
    game_field->selector->position.y = i;
}

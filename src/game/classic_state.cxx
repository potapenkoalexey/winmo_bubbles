#include <cmath>
#include <fstream>

#include "./classic_state.hxx"
#include "./game_over_state.hxx"
#include "./level_complete_state.hxx"
#include "./select_mode_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

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
    ///debagging
    ///progress->set_line_in_full();

    sound_fall = uni_ptr_sound(engine->create_sound_buffer("./data/sounds/00_falling"));
    sound_destroy_big_form = uni_ptr_sound(engine->create_sound_buffer("./data/sounds/02_destroy_big_form"));

    return EXIT_SUCCESS;
}

void classic_state::cleanup(grottans::engine*) {}

void classic_state::pause(grottans::engine*) {}

///////////////////////////////////////////////////////////////////////////////
/// \brief used for new level configuration after level_comlete_state
void classic_state::resume(grottans::engine*)
{
    game_field->fill_clasic();
    game_field->selector->position.x = 5;
    game_field->selector->position.y = 5;
    progress->set_line_in_null();
    progress->level_comlete_flag = false;
}

void classic_state::handle_events(grottans::engine* engine)
{
    grottans::event e;

    engine->input(/*out*/ e);

    if (e == grottans::event::mouse_released) {

        handle_mouse_event(engine);
        /// replacing event to start_pressed
        e = grottans::event::start_released;
    }

    switch (e) {
    case grottans::event::turn_off: {
        engine->loop = false;
        break;
    }
    case grottans::event::escape_released: {
        engine->pop_state();
        break;
    }
    case grottans::event::start_released: {

        size_t i = static_cast<size_t>(game_field->selector->position.y);
        size_t j = static_cast<size_t>(game_field->selector->position.x);
        bool search = game_field->can_select(i, j);

        // if block can be selected - marking them
        if (search && game_field->gems[i][j]->visible == true && game_field->gems[i][j]->color != block::palette::non) {
            game_field->gems[i][j]->selected = true;

            game_field->select_around(i, j);
            size_t delta_score = game_field->selecting();

            // sound
            if (delta_score > 0) {
                if (settings::SOUND) {
                    sound_fall->play(grottans::sound_buffer::properties::once);
                }
            }
            // sound destroy_big_form
            if (delta_score > 9) {
                if (settings::SOUND) {
                    sound_destroy_big_form->play(grottans::sound_buffer::properties::once);
                }
            }

            progress->increase_progress(delta_score, 20);

            if (progress->level_comlete_flag) {
                //settings::LEVEL += 1;
                engine->push_state(level_complete_state::instance());
            }

            game_field->unselect_all();
        }
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
    game_field->render(engine);
    progress->draw(engine);
    engine->swap_buffers();
}

void classic_state::handle_mouse_event(grottans::engine* engine)
{
    size_t w = engine->get_window_width();
    size_t h = engine->get_window_height();
    double i = 0;
    double j = 0;

    if (w > h) {
        int block = static_cast<int>(h) / 11;
        //find centr of the screen
        int centr_x = static_cast<int>(w) / 2;
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
        int block = static_cast<int>(w) / 11;
        int centr_y = h / 2 - block / 2;
        size_t m_x = engine->mouse_coord.x;
        size_t m_y = engine->mouse_coord.y;
        double delta_y = (m_y - static_cast<double>(centr_y)) / static_cast<double>(block);
        j = floor(m_x / static_cast<double>(w) * 11 - 0.5);
        i = 5 + delta_y;
        if (j < 0 || j > 9 || i > 10 || i < 0)
            return;
    }

    game_field->selector->position.x = static_cast<float>(j);
    game_field->selector->position.y = static_cast<float>(i);
}

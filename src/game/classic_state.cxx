#include <cmath>
#include <fstream>

#include "./classic_state.hxx"
#include "./game_over_state.hxx"
#include "./global_variables.hxx"
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
    game_field->undisappearing_all();
    game_field->unfalling_all();
    game_field->visible_all();
    game_field->fill_clasic();
    game_field->selector->position.x = 5;
    game_field->selector->position.y = 5;
    progress->set_line_in_null();
    progress->set_level_complete_flag(false);
}

void classic_state::handle_events(grottans::engine* engine)
{
    grottans::event e;

    engine->input(/*out*/ e);

    ///blocking handling in non-fixed states of the field
    if (game_field->f_state != field::field_state::fixed)
        return;

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
        ///go to select_mode_state
        engine->switch_to_state(engine->states[0]);
        g_LEVEL = 1;
        g_SCORE = 0;
        break;
    }
    case grottans::event::start_released: {

        size_t i = static_cast<size_t>(game_field->selector->position.y);
        size_t j = static_cast<size_t>(game_field->selector->position.x);

        bool search = game_field->can_select(i, j);

        /// if block can be selected - marking them
        if (search && game_field->gems[i][j]->visible == true && game_field->gems[i][j]->color != block::palette::non) {
            game_field->gems[i][j]->selected = true;

            size_t delta_score = game_field->selecting();

            ///blocking handling_event
            if (delta_score) {
                game_field->f_state = field::field_state::disappearing;
                //game_field->mark_falling_blocks();
            }

            /// sound
            if (delta_score > 0 && g_SOUND) {
                sound_fall->play(grottans::sound_buffer::properties::once);
            }
            /// sound destroy_big_form
            if (delta_score > 9 && g_SOUND) {
                sound_destroy_big_form->play(grottans::sound_buffer::properties::once);
            }

            size_t points = progress->delta_to_points(delta_score);
            progress->increase_progress(points, g_LEVEL);

            game_field->unselect_all();

            if (progress->get_level_complete_flag()) {
                ///go to level_complete_mode
                engine->switch_to_state(engine->states[3]);
                g_LEVEL++;
            }
        }
        break;
    }
    case grottans::event::left_released: {
        if (game_field->selector->position.x > 0) {
            game_field->selector->position.x -= g_OFFSET;
        } else {
            game_field->selector->position.x = 9.f;
        }
        break;
    }
    case grottans::event::right_released: {
        if (game_field->selector->position.x < 9) {
            game_field->selector->position.x += g_OFFSET;
        } else {
            game_field->selector->position.x = 0.f;
        }
        break;
    }
    case grottans::event::up_released: {
        if (game_field->selector->position.y > 0) {
            game_field->selector->position.y -= g_OFFSET;
        } else {
            game_field->selector->position.y = 9.f;
        }
        break;
    }
    case grottans::event::down_released: {
        if (game_field->selector->position.y < 9) {
            game_field->selector->position.y += g_OFFSET;
        } else {
            game_field->selector->position.y = 0.f;
        }
        break;
    }
    }
}

void classic_state::update(grottans::engine* engine)
{
    game_field->update_blocks_coord();

    if (game_field->is_all_fixed()) {
        game_field->f_state = field::field_state::fixed;
        game_field->mark_falling_blocks();
        if (!game_field->are_there_falling_blocks()) { ///if the field static
            game_field->mark_shifting_blocks();
        }
    }
}

void classic_state::draw(grottans::engine* engine)
{
    game_field->draw(engine);
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

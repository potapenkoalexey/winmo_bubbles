#include <cmath>
#include <fstream>

#include "../global_variables.hxx"
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
    game_field->init(engine);
    game_field->fill_clasic();

    ///progress desk
    progress = std::unique_ptr<progress_desk>(new progress_desk);
    progress->init(engine);
    progress->set_line_in_null(engine);

    ///counter
    m_counter = std::unique_ptr<counter>(new counter);
    m_counter->set_quantity_of_digits(5, counter::sign::unsign);
    m_counter->init(engine);
    m_counter->set_vertexes(0.55f, -0.87f, 0.08f, 0.08f);
    m_counter->set_color({ 1.0f, 1.0f, 1.0f, 1.0f });
    m_counter->set_vertex_buffer(engine);
    m_counter->set_displayed_number(0);

    ///sounds
    sound_fall = uni_ptr_sound(engine->create_sound_buffer("./data/sounds/00_falling"));
    sound_destroy_big_form = uni_ptr_sound(engine->create_sound_buffer("./data/sounds/02_destroy_big_form"));

    return EXIT_SUCCESS;
}

void classic_state::cleanup(grottans::engine*) {}

void classic_state::pause(grottans::engine*) {}

void classic_state::resume(grottans::engine* engine)
{
    game_field->undisappearing_all();
    game_field->unfalling_unshifting_all();
    game_field->visible_all();
    game_field->fill_clasic();
    game_field->selector->position.x = 5;
    game_field->selector->position.y = 5;
    progress->set_line_in_null(engine);
    progress->set_dispayed_number(0);
    progress->set_level_complete_flag(false);
    m_counter->set_displayed_number(g_SCORE);
}

void classic_state::handle_events(grottans::engine* engine)
{
    grottans::event e;

    engine->input(/*out*/ e);

    ///blocking handling in non-fixed states of the field
    if (game_field->f_state != field::field_state::fixed)
        return;

    if (e == grottans::event::mouse_pressed) {
        if (handle_mouse_event(engine)) {
            /// replacing event to start_pressed
            e = grottans::event::start_released;
        }
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
        g_score_in_the_end_of_level = 0;
        break;
    }
    case grottans::event::start_released: {
        handle_start_released_event(engine);
        break;
    }
    case grottans::event::left_released: {
        handle_left_released_event(engine);
        break;
    }
    case grottans::event::right_released: {
        handle_right_released_event(engine);
        break;
    }
    case grottans::event::up_released: {
        handle_up_released_event(engine);
        break;
    }
    case grottans::event::down_released: {
        handle_down_released_event(engine);
        break;
    }
    }
}

void classic_state::update(grottans::engine* engine)
{
    m_counter->set_displayed_number(g_SCORE);

    game_field->update_blocks_coord();

    if (game_field->is_all_fixed()) {
        game_field->f_state = field::field_state::fixed;
        ///restore broken blocks
        ///game_field->undisappearing_all();

        game_field->mark_falling_blocks();
        if (!game_field->are_there_falling_blocks()) {

            game_field->is_right_row_free();
            game_field->mark_shifting_blocks();

            if (game_field->is_all_fixed()) {

                game_field->f_state = field::field_state::fixed;
                ///check game_over
                if (game_field->is_game_over_classic()) {
                    engine->switch_to_state(engine->states[4]);
                }
                ///check new level
                if (progress->get_level_complete_flag()) {
                    g_LEVEL++;
                    ///go to level_complete_mode
                    engine->switch_to_state(engine->states[3]);
                }
            }
        }
    }
}

void classic_state::draw(grottans::engine* engine)
{
    game_field->draw(engine);
    progress->draw(engine);
    m_counter->draw(engine);

    engine->swap_buffers();
}

bool classic_state::handle_mouse_event(grottans::engine* engine)
{
    double i = 0;
    double j = 0;

    if (game_field->is_mouse_clicked_in_field(i, j, engine)) {

        game_field->selector->position.x = static_cast<float>(j);
        game_field->selector->position.y = static_cast<float>(i);
    } else {
        return false;
    }
    return true;
}

void classic_state::handle_start_released_event(grottans::engine* engine)
{
    size_t i = static_cast<size_t>(game_field->selector->position.y);
    size_t j = static_cast<size_t>(game_field->selector->position.x);

    bool search = game_field->can_select(i, j);

    /// if block can be selected - marking them
    if (search && game_field->gems[i][j]->visible == true && game_field->gems[i][j]->color != block::palette::non) {
        game_field->gems[i][j]->selected = true;

        size_t selected_blocks = game_field->selecting_to_disappearing();

        ///blocking handling_event
        if (selected_blocks) {
            game_field->f_state = field::field_state::disappearing;
        }

        /// sound
        if (selected_blocks > 1 && g_SOUND) {
            sound_fall->play(grottans::sound_buffer::properties::once);
        }
        /// sound destroy_big_form
        if (selected_blocks > 9 && g_SOUND) {
            sound_destroy_big_form->play(grottans::sound_buffer::properties::once);
        }

        size_t points = progress->blocks_to_points(selected_blocks);
        g_SCORE += points;
        progress->increase_progress(engine, points, g_LEVEL);
        progress->set_dispayed_number(points);

        game_field->unselect_all();
    }
}

void classic_state::handle_left_released_event(grottans::engine* engine)
{
    if (game_field->selector->position.x > 0) {
        game_field->selector->position.x -= g_OFFSET;
    } else {
        game_field->selector->position.x = 9.f;
    }
}

void classic_state::handle_right_released_event(grottans::engine* engine)
{
    if (game_field->selector->position.x < 9) {
        game_field->selector->position.x += g_OFFSET;
    } else {
        game_field->selector->position.x = 0.f;
    }
}

void classic_state::handle_up_released_event(grottans::engine* engine)
{
    if (game_field->selector->position.y > 0) {
        game_field->selector->position.y -= g_OFFSET;
    } else {
        game_field->selector->position.y = 9.f;
    }
}

void classic_state::handle_down_released_event(grottans::engine* engine)
{
    if (game_field->selector->position.y < 9) {
        game_field->selector->position.y += g_OFFSET;
    } else {
        game_field->selector->position.y = 0.f;
    }
}
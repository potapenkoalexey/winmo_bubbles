#include <cmath>
#include <fstream>

#include "./extreme_state.hxx"
#include "./game_over_state.hxx"
#include "./select_mode_state.hxx"

#define uni_ptr_sound std::unique_ptr<grottans::sound_buffer>

extreme_state extreme_state::m_extreme_state;

bool extreme_state::init(grottans::engine* engine)
{
    ///field
    game_field = std::unique_ptr<field>(new field);
    game_field->init(engine);
    game_field->fill_extreme();

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
    sound_flip = uni_ptr_sound(engine->create_sound_buffer("./data/sounds/08_flip"));
    sound_cant_flip = uni_ptr_sound(engine->create_sound_buffer("./data/sounds/09_cant_flip"));
    return EXIT_SUCCESS;
}

void extreme_state::cleanup(grottans::engine*) {}

void extreme_state::pause(grottans::engine*) {}

///////////////////////////////////////////////////////////////////////////////
/// \brief used for new level configuration after level_comlete_state
void extreme_state::resume(grottans::engine* engine)
{
    game_field->undisappearing_all();
    game_field->unfalling_unshifting_all();
    game_field->visible_all();
    game_field->fill_extreme();
    game_field->selector->position.x = 5;
    game_field->selector->position.y = 5;
    progress->set_line_in_null(engine);
    progress->set_dispayed_number(0);
    progress->set_level_complete_flag(false);
    m_counter->set_displayed_number(g_SCORE);
}

void extreme_state::handle_events(grottans::engine* engine)
{
    grottans::event e;

    engine->input(/*out*/ e);

    ///blocking handling in non-fixed states of the field
    if (game_field->f_state != field::field_state::fixed)
        return;

    handle_mouse_pressed_event(e, engine);
    handle_mouse_released_event(e, engine);

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
        size_t i = static_cast<size_t>(game_field->selector->position.y);
        size_t j = static_cast<size_t>(game_field->selector->position.x);

        if (game_field->gems[i][j]->color == block::palette::black) {
            break;
        }

        handle_start_released_event(i, j, engine);
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

void extreme_state::update(grottans::engine* engine)
{
    game_field->flip_gems_after_animation();
    game_field->update_blocks_coord();

    if (game_field->is_all_fixed()) {
        game_field->f_state = field::field_state::fixed;
        game_field->add_blocks_at_the_top_of_field();
        game_field->mark_falling_blocks();
        if (!game_field->are_there_falling_blocks()) {

            ///if the field static - check is_game_over
            if (game_field->is_game_over_extreme()) {
                engine->switch_to_state(engine->states[4]);
            }

            if (progress->get_level_complete_flag()) {
                g_LEVEL++;
                ///go to level_complete_mode
                engine->switch_to_state(engine->states[3]);
            }
        }
    }
}

void extreme_state::draw(grottans::engine* engine)
{
    game_field->draw(engine);
    progress->draw(engine);
    m_counter->draw(engine);

    engine->swap_buffers();
}

bool extreme_state::handle_mouse_pressed_event(grottans::event& e, grottans::engine* engine)
{
    bool result = true;

    if (e == grottans::event::mouse_pressed) {

        double i = 0;
        double j = 0;

        if (game_field->is_mouse_clicked_in_field(i, j, engine)) {

            game_field->selector->position.x = static_cast<float>(j);
            game_field->selector->position.y = static_cast<float>(i);
            e = grottans::event::start_released;
        }
    }
    return result;
}

bool extreme_state::handle_mouse_released_event(grottans::event& e, grottans::engine* engine)
{
    if (e == grottans::event::mouse_released) {
        double i = 0;
        double j = 0;
        if (game_field->is_mouse_clicked_in_field(i, j, engine)) {

            size_t w = engine->get_window_width();
            size_t h = engine->get_window_height();

            size_t mouse_x_pressed = engine->mouse_coord_pressed.x;
            size_t mouse_y_pressed = engine->mouse_coord_pressed.y;
            size_t mouse_x_released = engine->mouse_coord_released.x;
            size_t mouse_y_released = engine->mouse_coord_released.y;

            int delta_x = mouse_x_pressed - mouse_x_released;
            int delta_y = mouse_y_pressed - mouse_y_released;

            // do not handle small offsets
            if (abs(delta_x) < w / 20.f && abs(delta_y) < (h - w / 10.f) / 20.f) {
                game_field->selector->texture = game_field->tex_selector_clutch;
                return false;
            }
            // first quadrant
            if (delta_x >= 0 && delta_y >= 0) {
                if (abs(delta_x) >= abs(delta_y)) {
                    e = grottans::event::left_released;
                } else {
                    e = grottans::event::up_released;
                }
            }
            // third
            if (delta_x >= 0 && delta_y < 0) {
                if (abs(delta_x) >= abs(delta_y)) {
                    e = grottans::event::left_released;
                } else {
                    e = grottans::event::down_released;
                }
            }
            // second quardant
            if (delta_x < 0 && delta_y >= 0) {
                if (abs(delta_x) >= abs(delta_y)) {
                    e = grottans::event::right_released;
                } else {
                    e = grottans::event::up_released;
                }
            }
            // fourth quadrant
            if (delta_x < 0 && delta_y < 0) {
                if (abs(delta_x) >= abs(delta_y)) {
                    e = grottans::event::right_released;
                } else {
                    e = grottans::event::down_released;
                }
            }
        }

    } // end of mouse_released
    return true;
}

void extreme_state::handle_start_released_event(const size_t& i, const size_t& j, grottans::engine* engine)
{
    size_t selected_blocks = 0;
    if (game_field->gems[i][j]->motion == false) {
        game_field->unmotion_all();
        game_field->gems[i][j]->motion = true;
        ///change selector texture on "clutch"
        game_field->selector->texture = game_field->tex_selector_clutch;
    } else {
        ///restore standart selector texture
        game_field->selector->texture = game_field->tex_selector;

        // searching all selected blocks
        game_field->gems[i][j]->selected = true;

        if (game_field->gems[i][j]->color == block::palette::bomb) {
            selected_blocks = game_field->select_around_bomb(i, j);
        } else {
            selected_blocks = game_field->selecting_to_disappearing();
        }

        if (selected_blocks >= 3) {
            ///mark block
            //game_field->gems[i][j]->selected = true;
            game_field->f_state = field::field_state::disappearing;

            if (g_SOUND) {
                sound_fall->play(
                    grottans::sound_buffer::properties::once);
            }

            if (selected_blocks > 15 && g_SOUND) {
                sound_destroy_big_form->play(
                    grottans::sound_buffer::properties::once);
            }
            ///translate selected blocks in points
            size_t points = progress->blocks_to_points(selected_blocks);
            g_SCORE += points;
            progress->increase_progress(engine, points, g_LEVEL);
            progress->set_dispayed_number(points);

            game_field->unselect_all();

        } else {
            game_field->unselect_all();
            game_field->undisappearing_all();
        }
    }
    m_counter->set_displayed_number(g_SCORE);
}

void extreme_state::handle_left_released_event(grottans::engine*)
{
    game_field->selector->texture = game_field->tex_selector;

    size_t i = static_cast<size_t>(game_field->selector->position.y);
    size_t j = static_cast<size_t>(game_field->selector->position.x);

    if (game_field->gems[i][j]->motion == false) {
        if (game_field->selector->position.x > 0) {
            game_field->selector->position.x -= g_OFFSET;
        } else {
            game_field->selector->position.x = 9.f;
        }
    } else {
        if (game_field->can_flip(i, j, field::direction::left)) {
            //flip blocks with animation on 180 degrees
            game_field->gems[i][j]->state = block::block_state::fliping_over;
            game_field->gems[i][j]->flip_direction = block::block_direction::left;
            game_field->gems[i][j - 1]->state = block::block_state::fliping_under;
            game_field->gems[i][j - 1]->flip_direction = block::block_direction::right;

            game_field->f_draw_direction = field::draw_direction::clockwise;

            if (game_field->selector->position.x > 0) {
                game_field->selector->position.x -= g_OFFSET;
            } else {
                game_field->selector->position.x = 9.f;
            }
            if (g_SOUND) {
                sound_flip->play(grottans::sound_buffer::properties::once);
            }
        } else {
            //
            //flip blocks with animation on 360 degrees OR without any animation
            //
            if (g_SOUND) {
                sound_cant_flip->play(grottans::sound_buffer::properties::once);
            }
        }
        game_field->gems[i][j]->motion = false;
    }
}

void extreme_state::handle_right_released_event(grottans::engine*)
{
    game_field->selector->texture = game_field->tex_selector;

    size_t i = static_cast<size_t>(game_field->selector->position.y);
    size_t j = static_cast<size_t>(game_field->selector->position.x);

    if (game_field->gems[i][j]->motion == false) {
        if (game_field->selector->position.x < 9.f) {
            game_field->selector->position.x += g_OFFSET;
        } else {
            game_field->selector->position.x = 0.f;
        }
    } else {
        if (game_field->can_flip(i, j, field::direction::right)) {
            game_field->gems[i][j]->state = block::block_state::fliping_over;
            game_field->gems[i][j]->flip_direction = block::block_direction::right;
            game_field->gems[i][j + 1]->state = block::block_state::fliping_under;
            game_field->gems[i][j + 1]->flip_direction = block::block_direction::left;

            game_field->f_draw_direction = field::draw_direction::contr_clockwise;

            if (game_field->selector->position.x < 9.f) {
                game_field->selector->position.x += g_OFFSET;
            } else {
                game_field->selector->position.x = 0.f;
            }
            if (g_SOUND) {
                sound_flip->play(grottans::sound_buffer::properties::once);
            }
        } else {
            //
            //flip blocks with animation on 360 degrees
            //
            if (g_SOUND) {
                sound_cant_flip->play(grottans::sound_buffer::properties::once);
            }
        }
        game_field->gems[i][j]->motion = false; //is i need???
    }
}

void extreme_state::handle_up_released_event(grottans::engine*)
{
    game_field->selector->texture = game_field->tex_selector;

    size_t i = static_cast<size_t>(game_field->selector->position.y);
    size_t j = static_cast<size_t>(game_field->selector->position.x);

    if (game_field->gems[i][j]->motion == false) {
        if (game_field->selector->position.y > 0) {
            game_field->selector->position.y -= g_OFFSET;
        } else {
            game_field->selector->position.y = 9.f;
        }
    } else {
        if (game_field->can_flip(i, j, field::direction::up)) {
            game_field->gems[i][j]->state = block::block_state::fliping_over;
            game_field->gems[i][j]->flip_direction = block::block_direction::up;
            game_field->gems[i - 1][j]->state = block::block_state::fliping_under;
            game_field->gems[i - 1][j]->flip_direction = block::block_direction::down;

            game_field->f_draw_direction = field::draw_direction::clockwise;

            if (game_field->selector->position.y > 0) {
                game_field->selector->position.y -= g_OFFSET;
            } else {
                game_field->selector->position.y = 9.f;
            }
            if (g_SOUND) {
                sound_flip->play(grottans::sound_buffer::properties::once);
            }
        } else {
            //
            //flip blocks with animation on 360 degrees
            //
            if (g_SOUND) {
                sound_cant_flip->play(grottans::sound_buffer::properties::once);
            }
        }
        game_field->gems[i][j]->motion = false; //is i need???
    }
}

void extreme_state::handle_down_released_event(grottans::engine*)
{
    game_field->selector->texture = game_field->tex_selector;

    size_t i = static_cast<size_t>(game_field->selector->position.y);
    size_t j = static_cast<size_t>(game_field->selector->position.x);

    if (game_field->gems[i][j]->motion == false) {
        if (game_field->selector->position.y < 9) {
            game_field->selector->position.y += 1.f;
        } else {
            game_field->selector->position.y = 0.f;
        }
    } else {
        if (game_field->can_flip(i, j, field::direction::down)) {
            game_field->gems[i][j]->state = block::block_state::fliping_over;
            game_field->gems[i][j]->flip_direction = block::block_direction::down;
            game_field->gems[i + 1][j]->state = block::block_state::fliping_under;
            game_field->gems[i + 1][j]->flip_direction = block::block_direction::up;

            game_field->f_draw_direction = field::draw_direction::contr_clockwise;

            if (game_field->selector->position.y < 9) {
                game_field->selector->position.y += 1.f;
            } else {
                game_field->selector->position.y = 0.f;
            }
            if (g_SOUND) {
                sound_flip->play(grottans::sound_buffer::properties::once);
            }
        } else {
            //
            //flip blocks with animation on 360 degrees
            //
            if (g_SOUND) {
                sound_cant_flip->play(grottans::sound_buffer::properties::once);
            }
        }
        game_field->gems[i][j]->motion = false; //is i need???
    }
}

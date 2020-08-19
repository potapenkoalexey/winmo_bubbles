#include <cmath>
#include <fstream>

#include "./extreme_state.hxx"
#include "./game_over_state.hxx"
#include "./select_mode_state.hxx"

extreme_state extreme_state::m_extreme_state;

extreme_state::~extreme_state()
{
    delete sound_fall;
    delete sound_destroy_big_form;
    delete sound_flip;
    delete sound_cant_flip;
}

bool extreme_state::init(grottans::engine* e)
{
    engine = e;

    ///field
    game_field = std::make_unique<field>(engine);
    game_field->init();
    game_field->fill_extreme();

    ///progress desk
    progress = std::make_unique<progress_desk>(engine);
    progress->init();
    progress->set_line_in_null();

    ///sounds
    sound_fall = engine->create_sound_buffer("./data/sounds/00_falling");
    sound_destroy_big_form = engine->create_sound_buffer("./data/sounds/02_destroy_big_form");
    sound_flip = engine->create_sound_buffer("./data/sounds/08_flip");
    sound_cant_flip = engine->create_sound_buffer("./data/sounds/09_cant_flip");

    return EXIT_SUCCESS;
}

void extreme_state::cleanup() {}

void extreme_state::pause() {}

///////////////////////////////////////////////////////////////////////////////
/// \brief used for new level configuration after level_comlete_state
void extreme_state::resume()
{
    game_field->undisappearing_all();
    game_field->unfalling_unshifting_all();
    game_field->visible_all();
    if (g_LOAD_SAVED_STATE){
        engine->load_saved_settings();
        game_field->load_field_from_file();
        progress->update_progress_line_after_settings_restore();
        g_LOAD_SAVED_STATE = false;
    } else {
        game_field->fill_extreme();
    }
    game_field->selector->position.x = 5;
    game_field->selector->position.y = 5;
    progress->set_line_in_null();
    //need to be called only if the load_save mode selected
    //progress->update_progress_line_after_settings_restore();
    progress->set_dispayed_points(0);
    progress->set_level_complete_flag(false);
    progress->set_displayed_score(g_SCORE);
}

void extreme_state::handle_events()
{
    grottans::event e;

    engine->input(/*out*/ e);

    ///blocking handling in non-fixed states of the field
    if (game_field->f_state != field::field_state::fixed)
        return;

    handle_mouse_pressed_event(e);
    handle_mouse_released_event(e);

    switch (e) {
    case grottans::event::turn_off: {
        engine->loop = false;
        break;
    }
    case grottans::event::escape_released: {
        ///go to select_mode_state
        engine->save_settings();
        game_field->save_field_to_file();
        g_LEVEL = 1;
        g_SCORE = 0;
        g_score_in_the_end_of_level = 0;
        engine->switch_to_state_and_resume(engine->states[0]);

        break;
    }
    case grottans::event::start_released: {
        size_t i = static_cast<size_t>(game_field->selector->position.y);
        size_t j = static_cast<size_t>(game_field->selector->position.x);

        if (game_field->gems[i][j]->color == palette::black) {
            break;
        }

        handle_start_released_event(i, j);
        break;
    }
    case grottans::event::left_released: {
        handle_left_released_event();
        break;
    }
    case grottans::event::right_released: {
        handle_right_released_event();
        break;
    }
    case grottans::event::up_released: {
        handle_up_released_event();
        break;
    }
    case grottans::event::down_released: {
        handle_down_released_event();
        break;
    }
    default:
        break;
    }
}

void extreme_state::update()
{
    game_field->flip_gems_after_animation();
    game_field->update_blocks_coord();

    if (game_field->is_all_fixed()) {
        game_field->add_blocks_at_the_top_of_field();
        game_field->mark_falling_blocks();
        if (!game_field->are_there_falling_blocks()) {

            ///if the field static - check is_game_over
            if (game_field->is_game_over_extreme()) {
                engine->switch_to_state_and_resume(engine->states[4]);
            }

            if (progress->get_level_complete_flag()) {
                g_LEVEL++;
                ///go to level_complete_mode
                engine->switch_to_state_and_resume(engine->states[3]);
            }
        }
    }
}

void extreme_state::draw()
{
    game_field->draw();
    progress->draw();

    engine->swap_buffers();
}

bool extreme_state::handle_mouse_pressed_event(grottans::event& e)
{
    bool result = true;

    if (e == grottans::event::mouse_pressed) {

        double i = 0;
        double j = 0;

        if (game_field->is_mouse_clicked_in_field(i, j)) {

            game_field->selector->position.x = static_cast<float>(j);
            game_field->selector->position.y = static_cast<float>(i);
            e = grottans::event::start_released;
        } else {
            result = false;
        }
    }
    return result;
}

bool extreme_state::handle_mouse_released_event(grottans::event& e)
{
    if (e == grottans::event::mouse_released) {
        double i = 0;
        double j = 0;
        if (game_field->is_mouse_clicked_in_field(i, j)) {

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

void extreme_state::handle_start_released_event(const size_t& i, const size_t& j)
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

        if (game_field->gems[i][j]->color == palette::bomb) {
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
            progress->increase_progress(points, g_LEVEL);
            progress->set_dispayed_points(points);

            game_field->unselect_all();

        } else {
            game_field->unselect_all();
            game_field->undisappearing_all();
        }
    }
    progress->set_displayed_score(g_SCORE);
}

void extreme_state::handle_left_released_event()
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
            game_field->gems[i][j]->state = block_state::fliping_over;
            game_field->gems[i][j]->flip_direction = block_direction::left;
            game_field->gems[i][j - 1]->state = block_state::fliping_under;
            game_field->gems[i][j - 1]->flip_direction = block_direction::right;

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

void extreme_state::handle_right_released_event()
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
            game_field->gems[i][j]->state = block_state::fliping_over;
            game_field->gems[i][j]->flip_direction = block_direction::right;
            game_field->gems[i][j + 1]->state = block_state::fliping_under;
            game_field->gems[i][j + 1]->flip_direction = block_direction::left;

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

void extreme_state::handle_up_released_event()
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
            game_field->gems[i][j]->state = block_state::fliping_over;
            game_field->gems[i][j]->flip_direction = block_direction::up;
            game_field->gems[i - 1][j]->state = block_state::fliping_under;
            game_field->gems[i - 1][j]->flip_direction = block_direction::down;

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

void extreme_state::handle_down_released_event()
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
            game_field->gems[i][j]->state = block_state::fliping_over;
            game_field->gems[i][j]->flip_direction = block_direction::down;
            game_field->gems[i + 1][j]->state = block_state::fliping_under;
            game_field->gems[i + 1][j]->flip_direction = block_direction::up;

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

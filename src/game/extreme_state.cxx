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
    game_field->initialization(engine);
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

    if (e == grottans::event::mouse_released) {
        if (handle_mouse_event(engine)) {
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
        size_t i = static_cast<size_t>(game_field->selector->position.y);
        size_t j = static_cast<size_t>(game_field->selector->position.x);

        size_t selected_blocks = 0;

        if (game_field->gems[i][j]->color == block::palette::black) {
            break;
        }

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

                game_field->unselect_all();

                if (progress->get_level_complete_flag()) {
                    g_LEVEL++;
                    ///go to level_complete_mode
                    engine->switch_to_state(engine->states[3]);
                }
            } else {
                game_field->unselect_all();
                game_field->undisappearing_all();
            }
            //}
        }

        m_counter->set_displayed_number(g_SCORE);
        break;
    }
    case grottans::event::left_released: {
        game_field->selector->texture = game_field->tex_selector;

        size_t i = static_cast<size_t>(game_field->selector->position.y);
        size_t j = static_cast<size_t>(game_field->selector->position.x);

        if (game_field->gems[i][j]->motion == false) {
            if (game_field->selector->position.x > 0) {
                game_field->selector->position.x -= 1.f;
            } else {
                game_field->selector->position.x = 9.f;
            }
        } else {
            if (game_field->can_flip(i, j, field::direction::left)) {
                game_field->swap_gems(i, j, i, j - 1);
                //
                //flip blocks with animation on 180 degrees
                //
                if (game_field->selector->position.x > 0) {
                    game_field->selector->position.x -= 1.f;
                } else {
                    game_field->selector->position.x = 9.f;
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
        break;
    }
    case grottans::event::right_released: {
        game_field->selector->texture = game_field->tex_selector;

        size_t i = static_cast<size_t>(game_field->selector->position.y);
        size_t j = static_cast<size_t>(game_field->selector->position.x);

        if (game_field->gems[i][j]->motion == false) {
            if (game_field->selector->position.x < 9.f) {
                game_field->selector->position.x += 1.f;
            } else {
                game_field->selector->position.x = 0.f;
            }
        } else {
            if (game_field->can_flip(i, j, field::direction::right)) {
                game_field->swap_gems(i, j, i, j + 1);
                //
                //flip blocks with animation on 180 degrees
                //
                if (game_field->selector->position.x < 9.f) {
                    game_field->selector->position.x += 1.f;
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
        break;
    }
    case grottans::event::up_released: {
        game_field->selector->texture = game_field->tex_selector;

        size_t i = static_cast<size_t>(game_field->selector->position.y);
        size_t j = static_cast<size_t>(game_field->selector->position.x);

        if (game_field->gems[i][j]->motion == false) {
            if (game_field->selector->position.y > 0) {
                game_field->selector->position.y -= 1.f;
            } else {
                game_field->selector->position.y = 9.f;
            }
        } else {
            if (game_field->can_flip(i, j, field::direction::up)) {
                game_field->swap_gems(i - 1, j, i, j);
                //
                //flip blocks with animation on 180 degrees
                //
                if (game_field->selector->position.y > 0) {
                    game_field->selector->position.y -= 1.f;
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
        break;
    }
    case grottans::event::down_released: {
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
                game_field->swap_gems(i + 1, j, i, j);
                //
                //flip blocks with animation on 180 degrees
                //
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
        break;
    }
    }
}

void extreme_state::update(grottans::engine* engine)
{
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

bool extreme_state::handle_mouse_event(grottans::engine* engine)
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
        if (j < 0 || j > 10 || i < 0 || i > 9) {
            return false;
        }
    } else {
        int block = static_cast<int>(w) / 11;
        int centr_y = h / 2 - block / 2;
        size_t m_x = engine->mouse_coord.x;
        size_t m_y = engine->mouse_coord.y;
        double delta_y = (m_y - static_cast<double>(centr_y)) / static_cast<double>(block);
        j = floor(m_x / static_cast<double>(w) * 11 - 0.5);
        i = 5 + delta_y;
        if (j < 0 || j > 9 || i > 10 || i < 0)
            return false;
    }

    game_field->selector->position.x = static_cast<float>(j);
    game_field->selector->position.y = static_cast<float>(i);
    return true;
}

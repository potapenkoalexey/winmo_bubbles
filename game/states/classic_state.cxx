#include <cmath>
#include <fstream>

#include "../global_variables.hxx"
#include "classic_state.hxx"
#include "game_over_state.hxx"
#include "level_complete_state.hxx"
#include "select_mode_state.hxx"

classic_state classic_state::m_classic_state;

classic_state::~classic_state()
{

    delete sound_fall;
    delete sound_destroy_big_form;
}

bool classic_state::init(grottans::engine* e)
{
    engine = e;

    ///field
    game_field = std::make_unique<field>(engine);
    game_field->init();
    game_field->fill_clasic();

    ///progress desk
    progress = std::make_unique<progress_desk>(engine);
    progress->init();
    progress->set_line_in_null();

    ///sounds
    sound_fall = engine->create_sound_buffer("./data/sounds/00_falling");
    sound_destroy_big_form = engine->create_sound_buffer("./data/sounds/02_destroy_big_form");

    //    //background
    //    auto text = engine->load_txt_and_filter_comments("./data/vertex_buffers/vert_buffers_for_full_monitor.txt");
    //    text >> tr[0] >> tr[1];

    //    block_back = std::make_unique<block>(engine);
    //    block_back->texture = engine->create_texture("./data/images/my/back_3.png");
    //    block_back->v_buf = engine->create_vertex_buffer(&tr[0], 2);

    return EXIT_SUCCESS;
}

void classic_state::cleanup() {}

void classic_state::pause() {}

void classic_state::resume()
{
    game_field->undisappearing_all();
    game_field->unfalling_unshifting_all();
    game_field->visible_all();
    game_field->selector->position.x = 5;
    game_field->selector->position.y = 5;
    progress->set_line_in_null();
    progress->set_displayed_points(0);
    progress->set_level_complete_flag(false);
    
    if (g_LOAD_SAVED_STATE){
        ini_handl->load_settings_from_section("Saved");
        if (g_SCORE != 0){
            // preventing of loading saved field in case of previous game over
            game_field->load_field_from_file();
        }
        progress->update_progress_line_after_restore();
        g_LOAD_SAVED_STATE = false;
    } else {
        game_field->fill_clasic();
    }

    progress->set_displayed_score(g_SCORE);
}

void classic_state::handle_events()
{
    grottans::event e;

    engine->input(/*out*/ e);

    ///blocking handling in non-fixed states of the field
    if (game_field->f_state != field::field_state::fixed)
        return;

    if (e == grottans::event::mouse_right_released) {
        if (update_selector_position(grottans::event::mouse_right_released)) {
            // show value of the combo
            e = grottans::event::start_pressed;
        }
    }

    // if(event.button.button == SDL_BUTTON_LEFT)

    if (e == grottans::event::mouse_left_pressed) {
        if (update_selector_position(grottans::event::mouse_left_pressed)) {
            // replacing event to start_pressed
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
        ini_handl->update();
        ini_handl->save_settings_to_file();
        game_field->save_field_to_file();
        g_LEVEL = 1;
        g_SCORE = 0;
        g_score_in_the_end_of_level = 0;
        engine->switch_to_state_and_resume(engine->states[0]);

        break;
    }
    case grottans::event::start_pressed: {
        handle_start_pressed_event();
        break;
    }
    case grottans::event::start_released: {
        handle_start_released_event();
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

void classic_state::update()
{
    game_field->update_blocks_coord();

    if (game_field->is_all_fixed()) {
        ///restore broken blocks
        //game_field->undisappearing_all();

        game_field->mark_falling_blocks();

        if (!game_field->are_there_falling_blocks()) {

            game_field->if_right_row_free_add_right_row();
            game_field->mark_shifting_blocks();

            if (game_field->is_all_fixed()) {

                //game_field->unfalling_unshifting_all();
                //game_field->undisappearing_all();

                ///check game_over
                if (game_field->is_game_over_classic()) {
                    engine->switch_to_state_and_resume(engine->states[4]);
                }
                ///check new level
                if (progress->get_level_complete_flag()) {
                    g_LEVEL++;
                    ///go to level_complete_mode
                    engine->switch_to_state_and_resume(engine->states[3]);
                }
            }
        }
    }
}

void classic_state::draw()
{
    //background
    //    block_back->draw(engine);
    game_field->draw();
    progress->draw();

    engine->swap_buffers();
}

bool classic_state::update_selector_position(grottans::event event)
{
    double i = 0;
    double j = 0;

    if (game_field->is_mouse_clicked_in_field(event, i, j)) {

        game_field->selector->position.x = static_cast<float>(j);
        game_field->selector->position.y = static_cast<float>(i);
    } else {
        return false;
    }
    return true;
}

void classic_state::handle_start_pressed_event()
{
    size_t combo = game_field->get_current_combo_blocks_number();
    size_t points = progress->blocks_to_points(combo);
    progress->set_combo_points(points);
}

void classic_state::handle_start_released_event()
{
    size_t i = static_cast<size_t>(game_field->selector->position.y);
    size_t j = static_cast<size_t>(game_field->selector->position.x);

    bool search = game_field->can_select(i, j);

    /// if block can be selected - marking them
    if (search && game_field->gems[i][j]->visible == true && game_field->gems[i][j]->color != palette::non) {
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
        progress->increase_progress(points, g_LEVEL);
        progress->set_displayed_points(points);

        game_field->unselect_all();
    }
    progress->set_displayed_score(g_SCORE);
}

void classic_state::handle_left_released_event()
{
    if (game_field->selector->position.x > 0) {
        game_field->selector->position.x -= g_OFFSET;
    } else {
        game_field->selector->position.x = 9.f;
    }

    size_t combo = game_field->get_current_combo_blocks_number();
    size_t points = progress->blocks_to_points(combo);
    progress->set_combo_points(points);
}

void classic_state::handle_right_released_event()
{
    if (game_field->selector->position.x < 9) {
        game_field->selector->position.x += g_OFFSET;
    } else {
        game_field->selector->position.x = 0.f;
    }

    size_t combo = game_field->get_current_combo_blocks_number();
    size_t points = progress->blocks_to_points(combo);
    progress->set_combo_points(points);
}

void classic_state::handle_up_released_event()
{
    if (game_field->selector->position.y > 0) {
        game_field->selector->position.y -= g_OFFSET;
    } else {
        game_field->selector->position.y = 9.f;
    }

    size_t combo = game_field->get_current_combo_blocks_number();
    size_t points = progress->blocks_to_points(combo);
    progress->set_combo_points(points);
}

void classic_state::handle_down_released_event()
{
    if (game_field->selector->position.y < 9) {
        game_field->selector->position.y += g_OFFSET;
    } else {
        game_field->selector->position.y = 0.f;
    }

    size_t combo = game_field->get_current_combo_blocks_number();
    size_t points = progress->blocks_to_points(combo);
    progress->set_combo_points(points);
}

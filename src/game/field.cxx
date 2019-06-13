#include <algorithm>
#include <fstream>

#include "./field.hxx"
#include "./global_variables.hxx"

field::field() {}

bool field::init(grottans::engine* engine)
{
    width = g_FIELD_WIDTH;
    height = g_FIELD_HEIGHT;

    tex_selector_clutch = engine->create_texture("./data/images/my/selector_clutch.png");
    tex_selector = engine->create_texture("./data/images/my/selector.png");
    tex_yellow = engine->create_texture("./data/images/yellow.png");
    tex_purple = engine->create_texture("./data/images/purple.png");
    tex_green = engine->create_texture("./data/images/green.png");
    tex_black = engine->create_texture("./data/images/black.png");
    tex_blue = engine->create_texture("./data/images/blue.png");
    tex_bomb = engine->create_texture("./data/images/bomb.png");
    tex_red = engine->create_texture("./data/images/red.png");

    std::ifstream file_falling("./data/vertex_buffers/vert_buffers_for_gems.txt");
    if (!file_falling) {
        std::cerr << "can't load vert_buffers_for_gems.txt\n";
        return EXIT_FAILURE;
    } else {
        file_falling >> tr[0] >> tr[1] >> tr[2] >> tr[3];
        if (!sizeof(tr[1])) {
            std::cerr << "can't create vertex triangles for gems\n";
            return EXIT_FAILURE;
        }
    }
    file_falling.close();

    //creating 16 pairs uv-triangles
    for (int i = 31; i >= 1; i -= 2) {
        v_buf_disappear[i] = tr[1];
        v_buf_disappear[i - 1] = tr[0];

        tr[0].v[0].uv.y -= 0.0625f;
        tr[0].v[1].uv.y -= 0.0625f;
        tr[0].v[2].uv.y -= 0.0625f;
        tr[1].v[0].uv.y -= 0.0625f;
        tr[1].v[1].uv.y -= 0.0625f;
        tr[1].v[2].uv.y -= 0.0625f;
    }

    //creating 100 pairs xy-triangles
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            v_buf_grid[(i * width + j) * 2] = tr[2];
            v_buf_grid[(i * width + j) * 2 + 1] = tr[3];

            tr[2].v[0].pos.x += 0.18f;
            tr[2].v[1].pos.x += 0.18f;
            tr[2].v[2].pos.x += 0.18f;
            tr[3].v[0].pos.x += 0.18f;
            tr[3].v[1].pos.x += 0.18f;
            tr[3].v[2].pos.x += 0.18f;

            if (j == 9) {
                tr[2].v[0].pos.y -= 0.18f;
                tr[2].v[1].pos.y -= 0.18f;
                tr[2].v[2].pos.y -= 0.18f;
                tr[3].v[0].pos.y -= 0.18f;
                tr[3].v[1].pos.y -= 0.18f;
                tr[3].v[2].pos.y -= 0.18f;

                //restore original values
                tr[2].v[0].pos.x = -0.90f;
                tr[2].v[1].pos.x = -0.72f;
                tr[2].v[2].pos.x = -0.72f;
                tr[3].v[0].pos.x = -0.90f;
                tr[3].v[1].pos.x = -0.72f;
                tr[3].v[2].pos.x = -0.90f;
            }
        }
    }

    gems.resize(10);
    for (int i = 0, size = gems.size(); i < size; ++i) {
        gems[i].resize(10);
    }

    ///creating 100 blocks-tiles
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            //gems[i][j] = std::unique_ptr<block>(new block());
            gems[i][j] = new block();
            gems[i][j]->tr_disappear[0] = v_buf_disappear[0];
            gems[i][j]->tr_disappear[1] = v_buf_disappear[1];
        }
    }

    selector = std::unique_ptr<block>(new block);
    selector->texture = tex_selector;
    selector->position = { 5.f, 5.f };
    selector->tr_disappear[0] = v_buf_disappear[0];
    selector->tr_disappear[1] = v_buf_disappear[1];

    return EXIT_SUCCESS;
}

void field::fill_clasic()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j]->get_random_color_from_classic();
            associate_texture_with_gem(i, j);
        }
    }
}

void field::fill_extreme()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j]->get_random_color_from_extreme();
            associate_texture_with_gem(i, j);
        }
    }
}

void field::associate_texture_with_gem(const size_t& i, const size_t& j)
{
    switch (gems[i][j]->color) {
    case block::palette::red: {
        gems[i][j]->texture = tex_red;
        break;
    }
    case block::palette::green: {
        gems[i][j]->texture = tex_green;
        break;
    }
    case block::palette::yellow: {
        gems[i][j]->texture = tex_yellow;
        break;
    }
    case block::palette::blue: {
        gems[i][j]->texture = tex_blue;
        break;
    }
    case block::palette::purple: {
        gems[i][j]->texture = tex_purple;
        break;
    }
    case block::palette::black: {
        gems[i][j]->texture = tex_black;
        break;
    }
    case block::palette::bomb: {
        gems[i][j]->texture = tex_bomb;
        break;
    }
    }
}

bool field::can_select(const size_t& i, const size_t& j)
{
    // i - line, j - row
    bool result = false; // if search find nothing

    // if block is black of unvisible - return false
    if (gems[i][j]->color == block::palette::black || gems[i][j]->color == block::palette::non)
        return result;
    if (gems[i][j]->visible == false)
        return result;

    // gorizontal search
    if (j < 9) { // search right
        if (gems[i][j + 1]->color != block::palette::black || gems[i][j + 1]->color != block::palette::bomb) {
            if (gems[i][j + 1]->color == gems[i][j]->color && gems[i][j + 1]->visible == true) {
                result = true;
            }
        }
    }
    if (j > 0) { // search left
        if (gems[i][j - 1]->color != block::palette::black || gems[i][j - 1]->color != block::palette::bomb) {
            if (gems[i][j - 1]->color == gems[i][j]->color && gems[i][j - 1]->visible == true) {
                result = true;
            }
        }
    }
    // vertical search
    if (i < 9) { // search down
        if (gems[i + 1][j]->color != block::palette::black || gems[i + 1][j]->color != block::palette::bomb) {
            if (gems[i + 1][j]->color == gems[i][j]->color && gems[i + 1][j]->visible == true) {
                result = true;
            }
        }
    }
    if (i > 0) { // search up
        if (gems[i - 1][j]->color != block::palette::black || gems[i - 1][j]->color != block::palette::bomb) {
            if (gems[i - 1][j]->color == gems[i][j]->color && gems[i - 1][j]->visible == true) {
                result = true;
            }
        }
    }
    return result; // if block can be select
}

bool field::select_around(const size_t& i, const size_t& j)
{
    // i - line, j - row
    bool result = false; /// if block can be select

    if (gems[i][j]->color == block::palette::black) {
        gems[i][j]->selected = false;
        return result;
    }
    // gorizontal search
    if (j < 9) { // search right
        if (gems[i][j + 1]->color == gems[i][j]->color && gems[i][j + 1]->visible == true) {
            result = true;
            gems[i][j + 1]->selected = true;
            gems[i][j + 1]->state = block::block_state::disappearing;
        }
    }

    if (j > 0) { // search left
        if (gems[i][j - 1]->color == gems[i][j]->color && gems[i][j - 1]->visible == true) {
            result = true;
            gems[i][j - 1]->selected = true;
            gems[i][j - 1]->state = block::block_state::disappearing;
        }
    }
    // vertical search
    if (i < 9) { // search down
        if (gems[i + 1][j]->color == gems[i][j]->color && gems[i + 1][j]->visible == true) {
            result = true;
            gems[i + 1][j]->selected = true;
            gems[i + 1][j]->state = block::block_state::disappearing;
        }
    }
    if (i > 0) { // search up
        if (gems[i - 1][j]->color == gems[i][j]->color && gems[i - 1][j]->visible == true) {
            result = true;
            gems[i - 1][j]->selected = true;
            gems[i - 1][j]->state = block::block_state::disappearing;
        }
    }

    return result;
}

size_t field::select_around_bomb(const size_t& i, const size_t& j)
{
    size_t result = 1;

    gems[i][j]->selected = true;
    gems[i][j]->state = block::block_state::disappearing;
    if (j > 0) {
        gems[i][j - 1]->selected = true;
        gems[i][j - 1]->state = block::block_state::disappearing;
        result++;
    }
    if (j < 9) {
        gems[i][j + 1]->selected = true;
        gems[i][j + 1]->state = block::block_state::disappearing;
        result++;
    }
    if (i > 0) {
        gems[i - 1][j]->selected = true;
        gems[i - 1][j]->state = block::block_state::disappearing;
        result++;
        if (j < 9) {
            gems[i - 1][j + 1]->selected = true;
            gems[i - 1][j + 1]->state = block::block_state::disappearing;
            result++;
        }
        if (j > 0) {
            gems[i - 1][j - 1]->selected = true;
            gems[i - 1][j - 1]->state = block::block_state::disappearing;
            result++;
        }
    }
    if (i < 9) {
        gems[i + 1][j]->selected = true;
        gems[i + 1][j]->state = block::block_state::disappearing;
        result++;
        if (j > 0) {
            gems[i + 1][j - 1]->selected = true;
            gems[i + 1][j - 1]->state = block::block_state::disappearing;
            result++;
        }
        if (j < 9) {
            gems[i + 1][j + 1]->selected = true;
            gems[i + 1][j + 1]->state = block::block_state::disappearing;
            result++;
        }
    }
    return result;
}

size_t field::selecting_to_disappearing()
{
    size_t number_of_selected_blocks = 0;

    for (size_t k = 0; k < 9; k++) {
        for (size_t i = 0; i < width; i++) {
            for (size_t j = 0; j < height; j++) {
                if (gems[i][j]->selected) {
                    select_around(i, j);
                }
            }
        }
    }
    // counting numberOfSelectedBlocks
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            if (gems[i][j]->selected) {
                number_of_selected_blocks++;
            }
        }
    }

    return number_of_selected_blocks;
}

void field::unselect_all()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j]->selected = false;
        }
    }
}

void field::undisappearing_all()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            if (gems[i][j]->state != block::block_state::fixed) {
                gems[i][j]->state = block::block_state::fixed;
                gems[i][j]->tr_disappear[0] = v_buf_disappear[0];
                gems[i][j]->tr_disappear[1] = v_buf_disappear[1];
                gems[i][j]->current_time = 0.f;
            }
        }
    }
}

void field::unselect_undisappearing_all()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j]->selected = false;
            if (gems[i][j]->state != block::block_state::fixed) {
                gems[i][j]->state = block::block_state::fixed;
                gems[i][j]->tr_disappear[0] = v_buf_disappear[0];
                gems[i][j]->tr_disappear[1] = v_buf_disappear[1];
                gems[i][j]->current_time = 0.f;
            }
        }
    }
}

void field::unfalling_unshifting_all()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j]->move.delta.y = 0.f;
            gems[i][j]->falling_frame_index = 0;
            gems[i][j]->move.delta.x = 0.f;
            gems[i][j]->shifting_frame_index = 0;
        }
    }
}

void field::unmotion_all()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j]->motion = false;
        }
    }
}

void field::visible_all()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j]->visible = true;
        }
    }
}

bool field::is_all_fixed()
{
    bool result = true;
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            if (gems[i][j]->state != block::block_state::fixed) {
                result = false;
                return result;
            }
        }
    }
    return result;
}

bool field::are_there_falling_blocks()
{
    bool result = false;
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            if (gems[i][j]->state == block::block_state::falling) {
                result = true;
                return result;
            }
        }
    }
    return result;
}

bool field::is_right_row_free()
{
    bool result = false;
    if (gems[9][9]->visible == false) {
        add_right_row();
        result = true;
    }
    return result;
}

void field::add_right_row()
{
    size_t j = width - 1;
    for (size_t i = 0; i < height; i++) {
        gems[i][j]->visible = true;
        gems[i][j]->get_random_color_from_classic();
        associate_texture_with_gem(i, j);
    }
}

void field::add_blocks_at_the_top_of_field()
{
    for (size_t j = 0; j < height; j++) {
        if (gems[0][j]->visible == false) {
            gems[0][j]->get_random_color_from_extreme_with_bomb();

            gems[0][j]->visible = true;
            gems[0][j]->selected = false;
            gems[0][j]->motion = false;
            gems[0][j]->state = block::block_state::fixed;
            gems[0][j]->falling_frame_index = 0;
            gems[0][j]->shifting_frame_index = 0;

            associate_texture_with_gem(0, j);
        }
    }
}

void field::swap_gems(const size_t& i, const size_t& j, const size_t& m, const size_t& n)
{
    std::unique_ptr<block> copy = std::unique_ptr<block>(new block);

    copy->color = gems[i][j]->color;
    copy->texture = gems[i][j]->texture;
    copy->selected = gems[i][j]->selected;
    copy->visible = gems[i][j]->visible;
    copy->state = gems[i][j]->state;

    gems[i][j]->color = gems[m][n]->color;
    gems[i][j]->texture = gems[m][n]->texture;
    gems[i][j]->selected = gems[m][n]->selected;
    gems[i][j]->visible = gems[m][n]->visible;
    gems[i][j]->state = gems[m][n]->state;

    gems[m][n]->color = copy->color;
    gems[m][n]->texture = copy->texture;
    gems[m][n]->selected = copy->selected;
    gems[m][n]->visible = copy->visible;
    gems[m][n]->state = copy->state;
}

bool field::can_flip(const size_t& i, const size_t& j, field::direction dir)
{
    // if checking pair block-to-bomb -> always true, because bomb selecting return 9 !!!

    bool result = false;

    // if block Black - return false
    if (gems[i][j]->color == block::palette::black || gems[i][j]->color == block::palette::bomb)
        return result;

    // sort by direction
    if (dir == direction::up) {
        if (i > 0) {
            if (gems[i - 1][j]->color != block::palette::black) {

                if (gems[i - 1][j]->color == block::palette::bomb) {
                    swap_gems(i, j, i - 1, j);
                    gems[i - 1][j]->selected = true;
                    if (selecting_to_disappearing() >= 3) {
                        unselect_undisappearing_all();
                        result = true;
                    }
                    unselect_undisappearing_all();
                    swap_gems(i - 1, j, i, j);
                    return result;
                }

                swap_gems(i, j, i - 1, j);
                gems[i][j]->selected = true;
                if (selecting_to_disappearing() >= 3) {
                    unselect_undisappearing_all();
                    result = true;
                } else {
                    unselect_undisappearing_all();
                    gems[i - 1][j]->selected = true;
                    if (selecting_to_disappearing() >= 3) {
                        unselect_undisappearing_all();
                        result = true;
                    }
                }
                //restore previous MAP state
                unselect_undisappearing_all();
                swap_gems(i - 1, j, i, j);
                return result;
            }
        }
    }
    if (dir == direction::down) {
        if (i < 9) {
            if (gems[i + 1][j]->color != block::palette::black) {
                if (gems[i + 1][j]->color == block::palette::bomb) {
                    swap_gems(i, j, i + 1, j);
                    gems[i + 1][j]->selected = true;
                    if (selecting_to_disappearing() >= 3) {
                        unselect_undisappearing_all();
                        result = true;
                    }
                    unselect_undisappearing_all();
                    swap_gems(i + 1, j, i, j);
                    return result;
                }
                swap_gems(i, j, i + 1, j);
                gems[i][j]->selected = true;
                if (selecting_to_disappearing() >= 3) {
                    unselect_undisappearing_all();
                    result = true;
                } else {
                    unselect_undisappearing_all();
                    gems[i + 1][j]->selected = true;
                    if (selecting_to_disappearing() >= 3) {
                        unselect_undisappearing_all();
                        result = true;
                    }
                }
                //restore previous MAP state
                unselect_undisappearing_all();
                swap_gems(i + 1, j, i, j);
                return result;
            }
        }
    }
    if (dir == direction::left) {
        if (j > 0) {
            if (gems[i][j - 1]->color != block::palette::black) {
                if (gems[i][j - 1]->color == block::palette::bomb) {
                    swap_gems(i, j, i, j - 1);
                    gems[i][j - 1]->selected = true;
                    if (selecting_to_disappearing() >= 3) {
                        unselect_undisappearing_all();
                        result = true;
                    }
                    unselect_undisappearing_all();
                    swap_gems(i, j - 1, i, j);
                    return result;
                }

                swap_gems(i, j, i, j - 1);
                gems[i][j]->selected = true;
                if (selecting_to_disappearing() >= 3) {
                    unselect_undisappearing_all();
                    result = true;
                } else {
                    unselect_undisappearing_all();
                    gems[i][j - 1]->selected = true;
                    if (selecting_to_disappearing() >= 3) {
                        unselect_undisappearing_all();
                        result = true;
                    }
                }
                //restore previous MAP state
                unselect_undisappearing_all();
                swap_gems(i, j, i, j - 1);
                return result;
            }
        }
    }
    if (dir == direction::right) {
        if (j < 9) {
            if (gems[i][j + 1]->color != block::palette::black) {

                if (gems[i][j + 1]->color == block::palette::bomb) {
                    swap_gems(i, j, i, j + 1);
                    gems[i][j + 1]->selected = true;
                    if (selecting_to_disappearing() >= 3) {
                        unselect_undisappearing_all();
                        result = true;
                    }
                    unselect_undisappearing_all();
                    swap_gems(i, j + 1, i, j);
                    return result;
                }

                swap_gems(i, j, i, j + 1);
                gems[i][j]->selected = true;
                if (selecting_to_disappearing() >= 3) {
                    unselect_undisappearing_all();
                    result = true;
                } else {
                    unselect_undisappearing_all();
                    gems[i][j + 1]->selected = true;
                    if (selecting_to_disappearing() >= 3) {
                        unselect_undisappearing_all();
                        result = true;
                    }
                }
                //restore previous MAP state
                unselect_undisappearing_all();
                swap_gems(i, j, i, j + 1);
                return result;
            }
        }
    }

    return result;
}

void field::update_fliping_view(const size_t& i, const size_t& j, const milli_sec& delta_time)
{
    if (gems[i][j]->flip_direction == block::block_direction::non)
        return;

    gems[i][j]->current_time += delta_time.count() / 1000.f;

    float one_frame_delta = 1.f / g_FPS / g_FPS_fliping_factor;

    size_t how_may_frames_from_start = static_cast<size_t>(gems[i][j]->current_time / one_frame_delta);

    size_t current_frame_index = how_may_frames_from_start % g_FRAME_OF_FLIPING;

    ///assign new xy-position
    if (how_may_frames_from_start > gems[i][j]->fliping_frame_index) {
        ///0.18f - offset without rows in screen coord -1;1
        float offset = (g_offset_in_screen_coord / static_cast<float>(g_FRAME_OF_FLIPING - 1) * g_FPS_fliping_factor);

        gems[i][j]->fliping_frame_index++;

        switch (gems[i][j]->flip_direction) {
        case (block::block_direction::up): {
            gems[i][j]->move.delta.y += offset;
            break;
        }
        case (block::block_direction::down): {
            gems[i][j]->move.delta.y -= offset;
            break;
        }
        case (block::block_direction::left): {
            gems[i][j]->move.delta.x -= offset;
            break;
        }
        case (block::block_direction::right): {
            gems[i][j]->move.delta.x += offset;
            break;
        }
        }
    }

    if (gems[i][j]->state == block::block_state::fliping_under) {
        if (current_frame_index == 0) {
            gems[i][j]->tr_disappear[0] = v_buf_disappear.at(26);
            gems[i][j]->tr_disappear[1] = v_buf_disappear.at(27);
        }
    }
}

void field::flip_gems_after_animation()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            if (gems[i][j]->flip_direction != block::block_direction::non) {
                if (gems[i][j]->fliping_frame_index == g_FRAME_OF_FLIPING - 1) {
                    switch (gems[i][j]->flip_direction) {
                    case block::block_direction::down: {
                        gems[i][j]->restore_original_parameters(v_buf_disappear);
                        gems[i + 1][j]->restore_original_parameters(v_buf_disappear);
                        swap_gems(i, j, i + 1, j);
                        break;
                    }
                    case block::block_direction::right: {
                        gems[i][j]->restore_original_parameters(v_buf_disappear);
                        gems[i][j + 1]->restore_original_parameters(v_buf_disappear);
                        swap_gems(i, j, i, j + 1);
                        break;
                    }
                    }
                    return;
                }
            }
        }
    }
}

void field::mark_falling_blocks()
{
    for (int i = width - 2; i >= 0; i--) {
        for (size_t j = 0; j < height; j++) {
            if (gems[i][j]->visible == true) {
                if (gems[i + 1][j]->state == block::block_state::falling || gems[i + 1][j]->visible == false) {
                    gems[i][j]->state = block::block_state::falling;
                }
            }
        }
    }
}

void field::update_coord_falling_blocks(const size_t& i, const size_t& j, const milli_sec& delta_time)
{
    if (i == 9)
        return;
    if (gems[i][j]->state != block::block_state::falling) {
        return;
    }

    gems[i][j]->current_time += delta_time.count() / 1000.f;

    float one_frame_delta = 1.f / g_FPS / g_FPS_falling_factor;

    size_t how_may_frames_from_start = static_cast<size_t>(gems[i][j]->current_time / one_frame_delta);

    ///assign new xy-position
    if (how_may_frames_from_start > gems[i][j]->falling_frame_index) {
        ///0.18f - offset without rows in screen coord -1;1
        gems[i][j]->move.delta.y -= (g_offset_in_screen_coord / static_cast<float>(g_FRAME_OF_FALLING - 1) * g_FPS_falling_factor);
        gems[i][j]->falling_frame_index++;
    }

    if (how_may_frames_from_start > g_FRAME_OF_FALLING - 1) {
        //restore original state
        gems[i][j]->current_time = 0.f;
        gems[i][j]->falling_frame_index = 0;
        gems[i][j]->move.delta.y = 0.f;
        gems[i][j]->state = block::block_state::fixed;
        //swap with bottom block
        swap_gems(i, j, i + 1, j);
    }
}

void field::mark_shifting_blocks()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 1; j < height; j++) {
            if (gems[i][j]->visible == true) {
                if (gems[i][j - 1]->state == block::block_state::shifting || gems[i][j - 1]->visible == false) {
                    gems[i][j]->state = block::block_state::shifting;
                }
            }
        }
    }
}

void field::update_coord_shifting_blocks(const size_t& i, const size_t& j, const milli_sec& delta_time)
{
    if (j == 0)
        return;
    if (gems[i][j]->state != block::block_state::shifting) {
        return;
    }

    gems[i][j]->current_time += delta_time.count() / 1000.f;

    float one_frame_delta = 1.f / g_FPS / g_FPS_shifting_factor;

    size_t how_may_frames_from_start = static_cast<size_t>(gems[i][j]->current_time / one_frame_delta);

    ///assign new xy-position
    if (how_may_frames_from_start > gems[i][j]->shifting_frame_index) {
        ///0.18f - offset without columns in screen coord -1;1
        gems[i][j]->move.delta.x -= (g_offset_in_screen_coord / static_cast<float>(g_FRAME_OF_DISAPPEARING - 1) * g_FPS_shifting_factor);
        gems[i][j]->shifting_frame_index++;
    }

    if (how_may_frames_from_start > g_FRAME_OF_DISAPPEARING - 1) {
        //restore original state
        gems[i][j]->current_time = 0.f;
        gems[i][j]->shifting_frame_index = 0;
        gems[i][j]->move.delta.x = 0.f;
        gems[i][j]->state = block::block_state::fixed;
        //swap with left block
        swap_gems(i, j, i, j - 1);
    }
}

void field::update_blocks_coord()
{
    for (int i = width - 1; i >= 0; i--) {
        for (int j = 0; j < height; j++) {

            //update uv-triangles for disappeating blocks
            gems[i][j]->update_uv_coord(v_buf_disappear, g_frame_delta);

            //update vertical position for falling blocks
            //and gorizontal position for shifting blocks
            update_coord_falling_blocks(i, j, g_frame_delta);

            if (g_MODE == MODE::classic) {
                update_coord_shifting_blocks(i, j, g_frame_delta);
            }

            if (g_MODE == MODE::extreme) {
                update_fliping_view(i, j, g_frame_delta);
            }
        }
    }

    //update selector
    size_t j = static_cast<size_t>(selector->position.x);
    size_t i = static_cast<size_t>(selector->position.y);
    v_buf_tmp_selector[0] = v_buf_grid[(i * 10 + j) * 2] + selector->tr_disappear[0];
    v_buf_tmp_selector[1] = v_buf_grid[(i * 10 + j) * 2 + 1] + selector->tr_disappear[1];
}

bool field::is_game_over_classic()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            if (can_select(i, j))
                return false;
        }
    }
    return true;
}

bool field::is_game_over_extreme()
{
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            if (can_flip(i, j, direction::up)) {
                return false;
            }
            if (can_flip(i, j, direction::down)) {
                return false;
            }
            if (can_flip(i, j, direction::left)) {
                return false;
            }
            if (can_flip(i, j, direction::right)) {
                return false;
            }
        }
    }
    return true;
}

bool field::is_mouse_clicked_in_field(double& i /*out*/, double& j /*out*/, grottans::engine* engine)
{
    bool result = true;

    size_t w = engine->get_window_width();
    size_t h = engine->get_window_height();

    if (w > h) {
        int block = static_cast<int>(h) / 11;
        //find centr of the screen
        int centr_x = static_cast<int>(w) / 2;
        //take mouse cursor coordintes in engine
        size_t m_x = engine->mouse_coord_pressed.x;
        size_t m_y = engine->mouse_coord_pressed.y;
        //find delta
        double delta_x = m_x - static_cast<double>(centr_x);
        //find delta in size block size
        delta_x = delta_x / static_cast<double>(block);
        j = 5 + delta_x;
        i = floor(m_y / static_cast<double>(h) * 11); // work !!!!!!!!!!!
        ///blocking missclicks on progress_desk
        if (j < 0 || j > 10 || i < 0 || i > 9) {
            result = false;
        }
    } else {
        int block = static_cast<int>(w) / 11;
        int centr_y = h / 2 - block / 2;
        size_t m_x = engine->mouse_coord_pressed.x;
        size_t m_y = engine->mouse_coord_pressed.y;
        double delta_y = (m_y - static_cast<double>(centr_y)) / static_cast<double>(block);
        j = floor(m_x / static_cast<double>(w) * 11 - 0.5);
        i = 5 + delta_y;
        if (j < 0 || j > 9 || i > 10 || i < 0)
            result = false;
    }
    return result;
}

void field::draw(grottans::engine* engine)
{
    //drawing blocks
    if (f_draw_direction == draw_direction::clockwise) {
        for (size_t i = 0; i < width; i++) {
            for (size_t j = 0; j < height; j++) {
                if (gems[i][j]->visible) {
                    //drawing blocks
                    //calculate new ver_buff_triangles
                    v_buf_tmp[0] = v_buf_grid[(i * 10 + j) * 2] + gems[i][j]->tr_disappear[0];
                    v_buf_tmp[1] = v_buf_grid[(i * 10 + j) * 2 + 1] + gems[i][j]->tr_disappear[1];

                    //creating new vertex buffer
                    gems[i][j]->v_buf = engine->create_vertex_buffer(&v_buf_tmp[0], 2);
                    gems[i][j]->draw(engine);
                    engine->destroy_vertex_buffer(gems[i][j]->v_buf);
                }
            }
        }
    } else {
        for (int i = width - 1; i >= 0; i--) {
            for (int j = height - 1; j >= 0; j--) {
                if (gems[i][j]->visible) {
                    //drawing blocks
                    //calculate new ver_buff_triangles
                    v_buf_tmp[0] = v_buf_grid[(i * 10 + j) * 2] + gems[i][j]->tr_disappear[0];
                    v_buf_tmp[1] = v_buf_grid[(i * 10 + j) * 2 + 1] + gems[i][j]->tr_disappear[1];

                    //creating new vertex buffer
                    gems[i][j]->v_buf = engine->create_vertex_buffer(&v_buf_tmp[0], 2);
                    gems[i][j]->draw(engine);
                    engine->destroy_vertex_buffer(gems[i][j]->v_buf);
                }
            }
        }
    }

    //drawing selector only in field::fixed mode
    if (f_state == field_state::fixed) {
        selector->v_buf = engine->create_vertex_buffer(&v_buf_tmp_selector[0], 2);
        selector->draw(engine);
        engine->destroy_vertex_buffer(selector->v_buf);
    }
}

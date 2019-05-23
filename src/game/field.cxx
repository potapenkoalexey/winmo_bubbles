#include <fstream>

#include "./field.hxx"
#include "./global_variables.hxx"

field::field()
{
    width = g_FIELD_WIDTH;
    height = g_FIELD_HEIGHT;
    m_state = field_state::fixed;
}

bool field::initialization(grottans::engine* engine)
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
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            v_buf_grid[(i * 10 + j) * 2] = tr[2];
            v_buf_grid[(i * 10 + j) * 2 + 1] = tr[3];

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

    ///creating 100 blocks-tiles
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j] = std::unique_ptr<block>(new block);
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

void field::generate_on_top() {}

void field::fill_clasic()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j]->get_random_color_from_5();
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
            }
        }
    }
}

void field::fill_extreme()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j]->get_random_color_from_6();
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
            }
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

    if (gems[i][j]->color == block::palette::bomb) {
        gems[i][j]->selected = true;
        if (j > 0)
            gems[i][j - 1]->selected = true;
        if (j < 9)
            gems[i][j + 1]->selected = true;
        if (i > 0) {
            gems[i - 1][j]->selected = true;
            if (j < 9)
                gems[i - 1][j + 1]->selected = true;
            if (j > 0)
                gems[i - 1][j - 1]->selected = true;
        }
        if (i < 9) {
            gems[i + 1][j]->selected = true;
            if (j > 0)
                gems[i + 1][j - 1]->selected = true;
            if (j < 9)
                gems[i + 1][j + 1]->selected = true;
        }
        return true;
    }

    // gorizontal search
    if (j < 9) { // search right
        if (gems[i][j + 1]->color == gems[i][j]->color && gems[i][j + 1]->visible == true) {
            result = true;
            gems[i][j + 1]->selected = true;
            //for disappearing
            gems[i][j + 1]->state = block::block_state::disappearing;
        }
    }

    if (j > 0) { // search left
        if (gems[i][j - 1]->color == gems[i][j]->color && gems[i][j - 1]->visible == true) {
            result = true;
            gems[i][j - 1]->selected = true;
            //for disappearing
            gems[i][j - 1]->state = block::block_state::disappearing;
        }
    }
    // vertical search
    if (i < 9) { // search down
        if (gems[i + 1][j]->color == gems[i][j]->color && gems[i + 1][j]->visible == true) {
            result = true;
            gems[i + 1][j]->selected = true;
            //for disappearing
            gems[i + 1][j]->state = block::block_state::disappearing;
        }
    }
    if (i > 0) { // search up
        if (gems[i - 1][j]->color == gems[i][j]->color && gems[i - 1][j]->visible == true) {
            result = true;
            gems[i - 1][j]->selected = true;
            //for disappearing
            gems[i - 1][j]->state = block::block_state::disappearing;
        }
    }

    return result;
}

size_t field::selecting()
{
    size_t number_of_selected_blocks = 0;

    for (size_t k = 0; k < 10; k++) {
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
            gems[i][j]->state = block::block_state::fixed;
            gems[i][j]->tr_disappear[0] = v_buf_disappear[0];
            gems[i][j]->tr_disappear[1] = v_buf_disappear[1];
            gems[i][j]->current_time = 0.f;
        }
    }
}

bool field::is_all_fixed()
{
    bool result = true;
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            if (gems[i][j]->state != block::block_state::fixed)
                result = false;
        }
    }
    return result;
}

void field::update()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            //update uv-triangles for disappeating blocks
            gems[i][j]->update_uv_coord(v_buf_disappear, frame_delta);
            //update vertical position for falling blocks
            ///
        }
    }

    //update selector
    size_t j = static_cast<size_t>(selector->position.x);
    size_t i = static_cast<size_t>(selector->position.y);
    v_buf_tmp_selector[0] = v_buf_grid[(i * 10 + j) * 2] + selector->tr_disappear[0];
    v_buf_tmp_selector[1] = v_buf_grid[(i * 10 + j) * 2 + 1] + selector->tr_disappear[1];
}

void field::draw(grottans::engine* engine)
{
    grottans::mat2x3 scale = engine->scale;
    //drawing blocks
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            //drawing blocks
            //calculate new ver_buff_triangles
            v_buf_tmp[0] = v_buf_grid[(i * 10 + j) * 2] + gems[i][j]->tr_disappear[0];
            v_buf_tmp[1] = v_buf_grid[(i * 10 + j) * 2 + 1] + gems[i][j]->tr_disappear[1];

            //creating new vertex buffer
            gems[i][j]->v_buf = engine->create_vertex_buffer(&v_buf_tmp[0], 2);

            engine->render(*gems[i][j]->v_buf, gems[i][j]->texture,
                gems[i][j]->aspect * scale);

            engine->destroy_vertex_buffer(gems[i][j]->v_buf);
        }
    }

    //drawing selector
    selector->v_buf = engine->create_vertex_buffer(&v_buf_tmp_selector[0], 2);
    engine->render(*selector->v_buf, selector->texture, selector->aspect * scale);
    engine->destroy_vertex_buffer(selector->v_buf);
}

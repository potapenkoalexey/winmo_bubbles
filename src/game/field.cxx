#include <fstream>

#include "./field.hxx"

field::field()
{
    width = settings::FIELD_WIDTH;
    height = settings::FIELD_HEIGHT;
}

bool field::initialization(grottans::engine* engine)
{
    width = settings::FIELD_WIDTH;
    height = settings::FIELD_HEIGHT;

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
    for (size_t i = 0; i < 32; i += 2) {
        v_buf_fall[i] = tr[0];
        v_buf_fall[i + 1] = tr[1];

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

    width = 10;
    height = 10;
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j] = std::unique_ptr<block>(new block);
        }
    }

    selector = std::unique_ptr<block>(new block);

    selector->texture = tex_selector;
    selector->position = { 5.f, 5.f };

    return EXIT_SUCCESS;
}

void field::generate_on_top()
{
}

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

void field::render(grottans::engine* engine)
{
    grottans::mat2x3 scale = engine->scale;
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            v_buf_tmp[0] = v_buf_grid[(i * 10 + j) * 2] + v_buf_fall[30];
            v_buf_tmp[1] = v_buf_grid[(i * 10 + j) * 2 + 1] + v_buf_fall[31];
            gems[i][j]->v_buf = engine->create_vertex_buffer(&v_buf_tmp[0], 2);
            engine->render(*gems[i][j]->v_buf, gems[i][j]->texture, gems[i][j]->aspect * scale);
            engine->destroy_vertex_buffer(gems[i][j]->v_buf);
        }
    }
    {
        ///drawing selector
        size_t j = selector->position.x;
        size_t i = selector->position.y;
        v_buf_tmp[0] = v_buf_grid[(i * 10 + j) * 2] + v_buf_fall[30];
        v_buf_tmp[1] = v_buf_grid[(i * 10 + j) * 2 + 1] + v_buf_fall[31];
        selector->v_buf = engine->create_vertex_buffer(&v_buf_tmp[0], 2);
        engine->render(*selector->v_buf, selector->texture, selector->aspect * scale);
        engine->destroy_vertex_buffer(selector->v_buf);
    }
}

bool field::can_select(const size_t& i, const size_t& j)
{
    // i - line, j - row
    bool result = false; // if search find nothing

    // if block is black of unvisible - return false
    if (gems[i][j]->color == block::palette::black && gems[i][j]->color == block::palette::non)
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

void field::remove_selected()
{
    //    for (size_t i = 0; i < width; i++) {
    //        for (size_t j = 0; j < height; j++) {
    //            if (gems[i][j]->state == block::state::disappearing) {
    //            }
    //        }
    //    }
}

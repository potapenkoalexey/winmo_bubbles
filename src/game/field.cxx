#include <fstream>

#include "./field.hxx"

field::field()
{
    width = settings::FIELD_WIDTH;
    height = settings::FIELD_HEIGHT;
}

bool field::initialization(grottans::engine* engine)
{
    width = 10;
    height = 10;
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            gems[i][j] = std::unique_ptr<block>(new block);
        }
    }

    tex_yellow = engine->create_texture("./data/images/yellow.png");
    tex_purple = engine->create_texture("./data/images/purple.png");
    tex_green = engine->create_texture("./data/images/green.png");
    tex_black = engine->create_texture("./data/images/black.png");
    tex_blue = engine->create_texture("./data/images/blue.png");
    tex_bomb = engine->create_texture("./data/images/bomb.png");
    tex_red = engine->create_texture("./data/images/red.png");

    std::ifstream file_falling("./data/vertex_buffers/vert_buffers_for_gems.txt");
    if (!file_falling) {
        std::cerr << "can't load vert_buffers.txt\n";
        return EXIT_FAILURE;
    } else {
        file_falling >> tr[0] >> tr[1] >> tr[2] >> tr[3];
        if (!sizeof(tr[1])) {
            std::cerr << "can't create vertex buffers\n";
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

void field::remove_selected()
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            if (gems[i][j]->state == block::state::disappearing) {
            }
        }
    }
}

void field::render(grottans::engine* engine)
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            v_buf_tmp[0] = v_buf_grid[(i * 10 + j) * 2] + v_buf_fall[30];
            v_buf_tmp[1] = v_buf_grid[(i * 10 + j) * 2 + 1] + v_buf_fall[31];
            gems[i][j]->v_buf = engine->create_vertex_buffer(&v_buf_tmp[0], 2);
            engine->render(*gems[i][j]->v_buf, gems[i][j]->texture, gems[i][j]->aspect * engine->scale);
            engine->destroy_vertex_buffer(gems[i][j]->v_buf);
        }
    }
}

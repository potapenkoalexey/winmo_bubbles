#include <fstream>

#include "./progress_desk.hxx"

bool progress_desk::init(grottans::engine* engine)
{
    block_desk = std::unique_ptr<block>(new block);
    block_line = std::unique_ptr<block>(new block);

    tex_desk = engine->create_texture("./data/images/my/progress_desk.png");
    tex_line = engine->create_texture("./data/images/my/progress_line.png");

    block_desk->texture = tex_desk;
    block_line->texture = tex_line;

    std::ifstream file("./data/vertex_buffers/vert_buffer_for_progress_desk.txt");
    if (!file) {
        std::cerr << "can't load vert_buffer_for_progress_desk.txt\n";
        return EXIT_FAILURE;
    } else {
        /// tr0-1 - for desk
        /// tr2-3 - for line
        file >> tr[0] >> tr[1] >> tr[2] >> tr[3];
        if (!sizeof(tr[1])) {
            std::cerr << "can't create vertex buffer\n";
            return EXIT_FAILURE;
        }
    }
    file.close();

    ///backuping line coordinates
    tr[4] = tr[2];
    tr[5] = tr[3];

    block_desk->v_buf = engine->create_vertex_buffer(&tr[0], 2);
    block_line->v_buf = engine->create_vertex_buffer(&tr[2], 2);

    return EXIT_SUCCESS;
}

void progress_desk::draw(grottans::engine* engine)
{
    engine->render(*block_desk->v_buf, block_desk->texture, block_desk->aspect * engine->scale);
    block_line->v_buf = engine->create_vertex_buffer(&tr[2], 2);
    engine->render(*block_line->v_buf, block_line->texture, block_line->aspect * engine->scale);
}

void progress_desk::set_line_in_null()
{
    tr[2].v[1].pos.x = tr[2].v[0].pos.x;
    tr[2].v[2].pos.x = tr[2].v[2].pos.x;
    tr[3].v[1].pos.x = tr[3].v[2].pos.x;
}

void progress_desk::set_line_in_full()
{
    tr[2].v[1].pos.x = tr[4].v[1].pos.x;
    tr[2].v[2].pos.x = tr[4].v[2].pos.x;
    tr[3].v[1].pos.x = tr[5].v[1].pos.x;
}

void progress_desk::increase_progress(double points)
{
    // moving counter line on % step 0,0143
    //tr[2].v[1].pos.x += (0.0143f * points * 100 / (levels[level_number]));
    //tr[2].v[2].pos.x += (0.0143f * points * 100 / (levels[level_number]));
    //tr[3].v[1].pos.x += (0.0143f * points * 100 / (levels[level_number]));
}

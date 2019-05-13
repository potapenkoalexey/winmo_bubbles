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

    /// tr0-1 - for desk
    /// tr2-3 - for line
    std::ifstream file("./data/vertex_buffers/vert_buffer_for_progress_desk.txt");
    if (!file) {
        std::cerr << "can't load vert_buffer_for_progress_desk.txt\n";
        return EXIT_FAILURE;
    } else {
        file >> tr[0] >> tr[1] >> tr[2] >> tr[3];
        if (!sizeof(tr[1])) {
            std::cerr << "can't create vertex buffer\n";
            return EXIT_FAILURE;
        }
    }
    file.close();

    block_desk->v_buf = engine->create_vertex_buffer(&tr[0], 2);
    block_line->v_buf = engine->create_vertex_buffer(&tr[2], 2);

    return EXIT_SUCCESS;
}

void progress_desk::draw(grottans::engine* engine)
{
    engine->render(*block_desk->v_buf, block_desk->texture, block_desk->aspect * engine->scale);
    engine->render(*block_line->v_buf, block_line->texture, block_line->aspect * engine->scale);
}

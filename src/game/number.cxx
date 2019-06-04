#include "./number.hxx"

bool number::init(grottans::engine* engine)
{
    numbers_textures[0] = engine->create_texture("./data/font/0.png");
    numbers_textures[1] = engine->create_texture("./data/font/1.png");
    numbers_textures[2] = engine->create_texture("./data/font/2.png");
    numbers_textures[3] = engine->create_texture("./data/font/3.png");
    numbers_textures[4] = engine->create_texture("./data/font/4.png");
    numbers_textures[5] = engine->create_texture("./data/font/5.png");
    numbers_textures[6] = engine->create_texture("./data/font/6.png");
    numbers_textures[7] = engine->create_texture("./data/font/7.png");
    numbers_textures[8] = engine->create_texture("./data/font/8.png");
    numbers_textures[9] = engine->create_texture("./data/font/9.png");
    numbers_textures[10] = engine->create_texture("./data/font/+.png");
    if (numbers_textures[10] == nullptr) {
        std::cerr << "error: can't load numbers images" << std::endl;
        throw std::runtime_error("can't init numbers");
    }

    //set_vertexes(0.33f, -0.385f, 0.19f, 0.19f); //left number of the level_number
    //set_color({ 1.0, 1.0, 1.0, 1.0 });

    return EXIT_SUCCESS;
}

void number::draw(grottans::engine* engine)
{
    engine->render(*vertex_buffer, texture, move * engine->scale);
}

void number::set_number_and_texture(const int& t)
{
    if (t >= 0 && t < 11) {
        for (size_t i = 0; i < 11; i++) {
            if (t == i) {
                texture = numbers_textures[i];
            }
        }
    } else {
        std::cerr << "error: incorrect number in number class: " << t << std::endl;
        throw std::runtime_error("can't init number");
    }
}

void number::set_in_null()
{
    texture = numbers_textures[0];
}

void number::set_color(grottans::color col)
{
    vertex_triangles[0].v[0].c = col;
    vertex_triangles[0].v[1].c = col;
    vertex_triangles[0].v[2].c = col;

    vertex_triangles[1].v[0].c = col;
    vertex_triangles[1].v[1].c = col;
    vertex_triangles[1].v[2].c = col;
}

void number::set_vertexes(float x, float y, float w, float h)
{
    //w=h=0.08f in previous version of the game

    vertex_triangles[0].v[0].pos.x = x;
    vertex_triangles[0].v[0].pos.y = y;
    vertex_triangles[0].v[1].pos.x = x + w;
    vertex_triangles[0].v[1].pos.y = y;
    vertex_triangles[0].v[2].pos.x = x + w;
    vertex_triangles[0].v[2].pos.y = y - h;

    vertex_triangles[1].v[0].pos.x = x;
    vertex_triangles[1].v[0].pos.y = y;
    vertex_triangles[1].v[1].pos.x = x + w;
    vertex_triangles[1].v[1].pos.y = y - h;
    vertex_triangles[1].v[2].pos.x = x;
    vertex_triangles[1].v[2].pos.y = y - w;

    vertex_triangles[0].v[0].uv.x = 0.f;
    vertex_triangles[0].v[0].uv.y = 0.f;
    vertex_triangles[0].v[1].uv.x = 1.f;
    vertex_triangles[0].v[1].uv.y = 0.f;
    vertex_triangles[0].v[2].uv.x = 1.f;
    vertex_triangles[0].v[2].uv.y = 1.f;

    vertex_triangles[1].v[0].uv.x = 0.f;
    vertex_triangles[1].v[0].uv.y = 0.f;
    vertex_triangles[1].v[1].uv.x = 1.f;
    vertex_triangles[1].v[1].uv.y = 1.f;
    vertex_triangles[1].v[2].uv.x = 0.f;
    vertex_triangles[1].v[2].uv.y = 1.f;
}

void number::set_vertex_buffer(grottans::engine* engine)
{
    vertex_buffer = engine->create_vertex_buffer(&vertex_triangles[0], 2);
}

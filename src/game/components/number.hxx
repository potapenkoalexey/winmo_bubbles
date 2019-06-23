#pragma once

#include "../../../src/engine/color.hxx"
#include "../../../src/engine/engine.hxx"

class number {
public:
    bool init(grottans::engine* engine);
    void draw(grottans::engine* engine);

    void set_number_and_texture(const int&);
    void set_in_null();
    void set_color(grottans::color);
    void set_vertexes(float x, float y, float w, float h);
    void set_vertex_buffer(grottans::engine* engine);

private:
    grottans::mat2x3 move{};

    std::array<grottans::texture*, 11> numbers_textures;
    std::array<grottans::tri2, 2> vertex_triangles;

    grottans::texture* texture;
    grottans::vertex_buffer* vertex_buffer;
};

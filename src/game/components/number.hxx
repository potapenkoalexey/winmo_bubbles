#pragma once

#include "../../../src/engine/color.hxx"
#include "../../../src/engine/engine.hxx"

class number {
public:
    bool init(grottans::engine* engine);
    void draw(grottans::engine* engine);

    void set_number_and_texture(const int&);
    void set_in_null();
    void set_color(const grottans::color& color);
    void set_vertexes(const float& x, const float& y, const float& w,
        const float& h);
    void set_vertex_buffer(grottans::engine* engine);
    void cleanup();

    number();
    //number(grottans::engine* engine);
    ~number();

private:
    number(const number&) = delete;
    void operator=(const number&) = delete;

    grottans::mat2x3 move;

    std::array<grottans::texture*, 11> numbers_textures;
    std::array<grottans::tri2, 2> vertex_triangles;

    grottans::texture* texture;
    grottans::vertex_buffer* vertex_buffer;
};

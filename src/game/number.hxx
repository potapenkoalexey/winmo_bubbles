#pragma once

#include "../../src/engine/color.hxx"
#include "../../src/engine/engine.hxx"
#include "./block.hxx"

class number {
public:
    bool init(grottans::engine* engine);
    void draw(grottans::engine* engine);

    void set_number(const int&);
    void set_in_null();
    void set_color(grottans::color);
    void set_vertexes(float x, float y, float w, float h);

    grottans::vec2 position{ 0.f, 0.f };
    grottans::mat2x3 move{}; // = grottans::mat2x3::scale(1.f, 1.f);
    grottans::mat2x3 aspect{}; // = grottans::mat2x3::scale(1.f, 1.f);
    std::array<grottans::texture*, 11> numbers_textures;
    std::array<grottans::tri2, 2> vertex_triangles;

    grottans::texture* texture;
    grottans::vertex_buffer* vertex_buffer;
};

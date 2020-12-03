#pragma once

#include <array>

#include "../../engine/color.hxx"
#include "../../engine/engine.hxx"

class number final {
public:
    bool init();
    void draw();

    void set_number_and_texture(const int&);
    void set_in_null();
    void set_color(const grottans::color& color);
    void set_vertexes(const float& x, const float& y, const float& w, const float& h);
    void set_vertex_buffer();
    void cleanup();

    number(grottans::engine* e)
        : engine{ e }
        //, move{}
        , numbers_textures{}
        , texture{ nullptr }
        , vertex_buffer{ nullptr }
    {
    }
    ~number();

    number() = delete;
    number(const number&) = delete;
    const number& operator=(const number&) = delete;
    number(number&&) = delete;
    number& operator=(number&&) = delete;

private:
    grottans::engine* engine;

    grottans::mat2x3 move;

    std::array<grottans::texture*, 11> numbers_textures;
    std::array<grottans::tri2, 2> vertex_triangles;

    grottans::texture* texture;
    grottans::vertex_buffer* vertex_buffer;
};

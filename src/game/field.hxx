#pragma once

#include <iostream>
#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./settings.hxx"

class field {
public:
    field();
    field(size_t width, size_t height);
    bool initialization(grottans::engine* engine);
    void fill_clasic();
    void fill_extreme();
    void remove_selected();
    void render(grottans::engine* engine);

    void select();
    void move_falling();
    void shift_in_left();
    void generate_on_top();
    void generate_right_column();
    void swap();

    size_t width = 0;
    size_t height = 0;
    float scale = 0;

    grottans::texture* tex_yellow = nullptr;
    grottans::texture* tex_purple = nullptr;
    grottans::texture* tex_green = nullptr;
    grottans::texture* tex_black = nullptr;
    grottans::texture* tex_blue = nullptr;
    grottans::texture* tex_bomb = nullptr;
    grottans::texture* tex_red = nullptr;

    std::array<grottans::tri2, 4> tr;
    //xy-triangles for all field positions
    std::array<grottans::tri2, 200> v_buf_grid;
    //uv-triangles for disapearring animation
    std::array<grottans::tri2, 32> v_buf_fall;
    //render tmp
    std::array<grottans::tri2, 2> v_buf_tmp;

    //std::map<position, std::unique_ptr<block> > gems;
    std::unique_ptr<block> gems[10][10];
};

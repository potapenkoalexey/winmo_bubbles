#pragma once

#include <iostream>
#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./global_variables.hxx"

class field {
public:
    enum class state {
        fixed,
        swaping,
        falling,
        shifting
    };

    field();
    field(size_t width, size_t height);

    bool initialization(grottans::engine* engine);
    void fill_clasic();
    void fill_extreme();
    void render(grottans::engine* engine);
    grottans::mouse_pos get_mouse_ij(grottans::engine* engine);

    bool can_select(const size_t&, const size_t&);
    bool select_around(const size_t&, const size_t&);
    size_t selecting();
    void unselect_all();
    void undisappearing_all();

    void remove_selected();
    void move_falling();
    void shift_in_left();
    void generate_on_top();
    void generate_right_column();
    void swap();

    size_t width = 0;
    size_t height = 0;
    float scale = 0;

    state m_state = state::fixed;

    //block selector in centr of the field
    std::unique_ptr<block> selector;

    grottans::texture* tex_selector_clutch = nullptr;
    grottans::texture* tex_selector = nullptr;
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
    std::array<grottans::tri2, 32> v_buf_disappear;
    //render tmp
    std::array<grottans::tri2, 2> v_buf_tmp;

    /*std::map<position, std::unique_ptr<block> > gems;
    //std::vector<std::vector<std::unique_ptr<block> > > gems;
    //std::unique_ptr<block> gems = std::unique_ptr<block>(new block[width * height]);
    //std::unique_ptr<block>** gems;  [10][10];  */
    std::unique_ptr<block> gems[10][10];
};

#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "../../../src/engine/engine.hxx"
#include "../global_variables.hxx"
#include "./block.hxx"

struct field {

    enum class field_state {
        fixed,
        fliping,
        falling,
        shifting,
        disappearing //maybe unused because included in falling
    };

    enum class direction {
        right,
        down,
        left,
        up,
        non
    };

    enum class draw_direction {
        clockwise,
        contr_clockwise,
    };

    field(grottans::engine* e)
        : engine{ e }
    {
    }

    //field(const size_t& width, const size_t& height);
    ~field();

    field() = delete;
    field(const field&) = delete;
    const field& operator=(const field&) = delete;

    bool init();
    void fill_clasic();
    void fill_extreme();
    void associate_texture_with_gem(const size_t& i, const size_t& j);
    void draw();
    grottans::mouse_pos get_mouse_ij();

    bool can_select(const size_t&, const size_t&);
    bool select_around(const size_t&, const size_t&);
    size_t select_around_bomb(const size_t& i, const size_t& j);
    size_t selecting_to_disappearing();

    void unselect_all();
    void undisappearing_all();
    void unselect_undisappearing_all();
    void unfalling_unshifting_all();
    void unmotion_all();
    void visible_all();

    bool is_all_fixed();

    void mark_falling_blocks();
    void update_coord_falling_blocks(const size_t& i, const size_t& j, const milli_sec& delta_time);
    bool are_there_falling_blocks();
    bool is_right_row_free();
    void add_right_row();
    void add_blocks_at_the_top_of_field();
    void mark_shifting_blocks();
    void update_coord_shifting_blocks(const size_t& i, const size_t& j, const milli_sec& delta_time);

    void swap_gems(const size_t& i, const size_t& j, const size_t& m, const size_t& n);
    bool can_flip(const size_t& i, const size_t& j, direction dir);
    void update_fliping_view(const size_t& i, const size_t& j, const milli_sec& delta_time);
    void flip_gems_after_animation();

    void update_blocks_coord();

    bool is_game_over_classic();
    bool is_game_over_extreme();

    bool is_mouse_clicked_in_field(double& i /*out*/, double& j /*out*/);

    ///////////////////////////////////////////////////////////////////////////

    grottans::engine* engine;

    size_t width = 0;
    size_t height = 0;
    float scale = 0;

    field_state f_state = field_state::fixed;
    draw_direction f_draw_direction = draw_direction::clockwise;

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
    //render tmp for selector
    std::array<grottans::tri2, 2> v_buf_tmp_selector;

    std::vector<std::vector<block*> > gems; //[10][10];
};

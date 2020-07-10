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
        , width{ g_FIELD_WIDTH }
        , height{ g_FIELD_HEIGHT }
        , scale{ 0 }
        , f_state{ field_state::fixed }
        , f_draw_direction{ draw_direction::clockwise }
        , selector{ nullptr }
        , tex_selector_clutch{ nullptr }
        , tex_selector{ nullptr }
        , tex_yellow{ nullptr }
        , tex_purple{ nullptr }
        , tex_green{ nullptr }
        , tex_black{ nullptr }
        , tex_blue{ nullptr }
        , tex_bomb{ nullptr }
        , tex_red{ nullptr }

    {
    }

    ~field();

    field() = delete;
    field(const field&) = delete;
    const field& operator=(const field&) = delete;
    field(field&&) = delete;
    field& operator=(field&&) = delete;

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
    bool if_right_row_free_add_right_row();
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

    bool save_field_to_file();
    bool load_field_from_file();

    ///////////////////////////////////////////////////////////////////////////

    grottans::engine* engine;

    size_t width;
    size_t height;
    float scale;

    field_state f_state;
    draw_direction f_draw_direction;

    //block selector in centr of the field
    std::unique_ptr<block> selector;

    grottans::texture* tex_selector_clutch;
    grottans::texture* tex_selector;
    grottans::texture* tex_yellow;
    grottans::texture* tex_purple;
    grottans::texture* tex_green;
    grottans::texture* tex_black;
    grottans::texture* tex_blue;
    grottans::texture* tex_bomb;
    grottans::texture* tex_red;

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

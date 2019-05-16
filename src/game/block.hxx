#pragma once

#include "../../src/engine/engine.hxx"

struct block {

    enum class palette {
        red,
        blue,
        bomb,
        green,
        black,
        purple,
        yellow
    };

    enum class state {
        fixed,
        swaping,
        falling,
        shifting,
        disappearing
    };

    palette color = palette::black;
    state state = state::fixed;

    grottans::vec2 position{};
    //grottans::palette color = grottans::palette::non;
    bool selected = false;
    bool visible = true;
    bool motion = false;
    grottans::mat2x3 move{};
    grottans::mat2x3 aspect = grottans::mat2x3::scale(1.f, 1.f);
    grottans::texture* texture = nullptr;
    grottans::vertex_buffer* v_buf = nullptr;

    void get_random_color_from_5();
    void get_random_color_from_6();
    void get_random_color_from_7();

    size_t x0; // botton bound (include)
    size_t x1; // upper bound
    size_t y0; // left bound (include)
    size_t y1; // right bound
    void set_mouse_rectangle(size_t x0_, size_t x1_, size_t y0_, size_t y1_);
    bool is_mouse_pressed(size_t x, size_t y);
};

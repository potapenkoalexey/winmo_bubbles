#pragma once

#include "../../src/engine/engine.hxx"

struct block {

    enum class palette {
        non,
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

    bool selected = false;
    bool visible = true;
    bool motion = false;
    grottans::vec2 position{ 0, 0 };
    grottans::mat2x3 move{};
    grottans::mat2x3 aspect = grottans::mat2x3::scale(1.f, 1.f);
    grottans::texture* texture = nullptr;
    grottans::vertex_buffer* v_buf = nullptr;

    ///for animation
    float fps = 60.f;
    float current_time = 0.f;

    float get_fps() const { return fps; }
    void set_fps(float fps_value) { fps = fps_value; }
    void restart() { current_time = 0.f; }
    void set_uv_coord(
        grottans::engine& e,
        const std::array<grottans::tri2, 32>& arr_v_buf, //field::v_buf_disappear
        const float& delta_time);

    void get_random_color_from_5();
    void get_random_color_from_6();
    void get_random_color_from_7();
};

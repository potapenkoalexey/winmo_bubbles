#pragma once

#include "../../src/engine/engine.hxx"
#include "./global_variables.hxx"

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

    enum class block_state {
        fixed,
        fliping_over,
        fliping_under,
        falling,
        shifting,
        disappearing
    };

    enum class block_direction {
        right,
        down,
        left,
        up,
        non
    };

    palette color = palette::black;
    block_state state = block_state::fixed;
    block_direction flip_direction = block_direction::non;

    bool selected = false;
    bool visible = true;
    bool motion = false;
    grottans::vec2 position{ 0.f, 0.f };
    grottans::mat2x3 move = grottans::mat2x3::scale(1.f, 1.f);
    grottans::mat2x3 aspect = grottans::mat2x3::scale(1.f, 1.f);
    grottans::texture* texture = nullptr;
    grottans::vertex_buffer* v_buf = nullptr;

    ///for animation
    float fps = g_FPS;
    float current_time = 0.f;
    std::array<grottans::tri2, 2> tr_disappear;

    size_t falling_frame_index = 0;
    size_t shifting_frame_index = 0;
    size_t fliping_frame_index = 0;

    float get_fps() const { return fps; }
    void set_fps(float fps_value) { fps = fps_value; }

    void update_uv_coord(
        const std::array<grottans::tri2, 32>& arr_uv_buf,
        const milli_sec& delta_time);

    void update_fliping_view(
        const std::array<grottans::tri2, 32>& arr_uv_buf,
        const milli_sec& delta_time);

    void restore_original_parameters(const std::array<grottans::tri2, 32>& arr_uv_buf);

    void get_random_color_from_classic();
    void get_random_color_from_extreme();
    void get_random_color_from_extreme_with_bomb();

    void draw(grottans::engine*);
};

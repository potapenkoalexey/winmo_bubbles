#pragma once

#include "../../../src/engine/engine.hxx"
#include "./../global_variables.hxx"

class block {
public:
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

    palette color;
    block_state state;
    block_direction flip_direction;

    bool selected;
    bool visible;
    bool motion;
    grottans::vec2 position;
    grottans::mat2x3 move;
    grottans::mat2x3 aspect;
    grottans::texture* texture;
    grottans::vertex_buffer* v_buf;

    ///for animation
    float fps;
    float current_time;
    std::array<grottans::tri2, 2> tr_disappear;

    size_t falling_frame_index;
    size_t shifting_frame_index;
    size_t fliping_frame_index;

    ///////////////////////////////////////////////////////////////////////////

    float get_fps() const;
    void set_fps(float fps_value);

    void update_uv_coord(
        const std::array<grottans::tri2, 32>& arr_uv_buf,
        const milli_sec& delta_time);

    void restore_original_parameters(
        const std::array<grottans::tri2, 32>& arr_uv_buf);

    void get_random_color_from_classic();
    void get_random_color_from_extreme();
    void get_random_color_from_extreme_with_bomb();

    void draw(grottans::engine*);

    block();
    ~block();

private:
    block(const block&) = delete;
    void operator=(const block&) = delete;
};

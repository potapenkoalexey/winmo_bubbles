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

    void draw();

    block(grottans::engine* e)
        : color{ palette::black }
        , state{ block_state::fixed }
        , flip_direction{ block::block_direction::non }
        , selected{ false }
        , visible{ true }
        , motion{ false }
        , position{ 0.f, 0.f }
        , move{ grottans::mat2x3::scale(1.f, 1.f) }
        , aspect{ grottans::mat2x3::scale(1.f, 1.f) }
        , texture{ nullptr }
        , v_buf{ nullptr }
        , fps{ g_FPS }
        , current_time{ 0.f }
        , falling_frame_index{ 0 }
        , shifting_frame_index{ 0 }
        , fliping_frame_index{ 0 }
        , engine{ e }
    {
        move.delta.x = 0;
        move.delta.y = 0;
    }
    ~block();
    block() = delete;
    block(const block&) = delete;
    const block& operator=(const block&) = delete;

    ///////////////////////////////////////////////////////////////////////////
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

private:
    grottans::engine* engine;
};

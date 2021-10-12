#pragma once

#include <iosfwd>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "color.hxx"
#include "state.hxx"
#include "math_structures.hxx"
#include "sound_buffer.hxx"
#include "texture.hxx"
#include "vertex_buffer.hxx"

#include "file_operations.hxx"

namespace grottans {

enum class event {
    // input events
    left_pressed,
    left_released,
    right_pressed,
    right_released,
    up_pressed,
    up_released,
    down_pressed,
    down_released,
    escape_pressed,
    escape_released,
    start_pressed,
    start_released,
    button1_pressed,
    button1_released,
    button2_pressed,
    button2_released,
    mouse_motion, //move
    mouse_pressed,
    mouse_released,
    turn_off
};

std::ostream& operator<<(std::ostream& stream, const event& e);

enum class keys {
    left,
    right,
    up,
    down,
    select,
    start,
    button1,
    button2,
    mouse
};

enum class palette {
    yellow,
    purple,
    black,
    green,
    stone,
    blue,
    bomb,
    red,
    non
};

struct mouse_pos {
    mouse_pos()
        : x{ 0 }
        , y{ 0 }
    {
    }

    mouse_pos(const size_t& x_, const size_t& y_)
        : x{ x_ }
        , y{ y_ }
    {
    }

    size_t x = 0;
    size_t y = 0;
};

/// position in 2d space
struct pos {
    float x = 0.f;
    float y = 0.f;
};

/// texture position (normalized)
struct uv_pos {
    float u = 0.f;
    float v = 0.f;
};

std::istream& operator>>(std::istream& is, uv_pos&);

struct vertex {
    vertex()
        : x{ 0.f }
        , y{ 0.f }
        , tx{ 0.f }
        , ty{ 0.f }
    {
    }
    float x;
    float y;
    float tx;
    float ty;
};

std::istream& operator>>(std::istream& is, vertex&);

struct triangle {
    triangle()
    {
        v[0] = vertex();
        v[1] = vertex();
        v[2] = vertex();
    }
    vertex v[3];
};

std::istream& operator>>(std::istream& is, triangle&);

class game_state;

///////////////////////////////////////////////////////////////////////////////
class engine {
public:
    // Engine() {}

    virtual std::string initialize() = 0;
    virtual float get_time_from_init() = 0;
    virtual bool load_texture(const std::string_view) = 0;
    virtual bool input(event& e) = 0;
    virtual bool is_key_down(const keys) = 0;

    virtual void disable_mouse_moution_event() = 0;
    virtual void enable_mouse_moution_event() = 0;

    virtual void switch_to_state_and_resume(game_state*) = 0;
    virtual void change_state(game_state*) = 0;
    virtual void push_state_and_init(game_state*) = 0;
    virtual void pop_state() = 0;
    virtual void clear_states() = 0;
    //virtual void swap_last_two_states() = 0;

    virtual texture* create_texture(const std::string&) = 0;
    virtual void destroy_texture(texture*) = 0;
    virtual vertex_buffer* create_vertex_buffer(const tri2*, const size_t&) = 0;
    virtual void destroy_vertex_buffer(vertex_buffer*) = 0;

    virtual sound_buffer* create_sound_buffer(const std::string_view) = 0;
    virtual void destroy_sound_buffer(sound_buffer*) = 0;

    virtual std::stringstream load_txt_and_filter_comments(std::string_view file) = 0;

    virtual void render_triangle(const triangle&) = 0;
    virtual void render(const tri0&, const color&) = 0;
    virtual void render(const tri1&) = 0;
    virtual void render(const tri2&, texture*) = 0;
    virtual void render(const tri2&, texture*, const mat2x3&) = 0;
    virtual void render(const vertex_buffer&, texture*, const mat2x3&) = 0;

    virtual void set_window_title(const char*) = 0;
    virtual void set_window_size(const size_t& w, const size_t& h) = 0;
    virtual size_t get_window_width() = 0;
    virtual size_t get_window_height() = 0;

    virtual void swap_buffers() = 0;
    virtual void uninitialize() = 0;

    virtual bool is_mouse_clicked_in_coord(const float& lx, const float& rx, const float& ly, const float& ry) = 0;

    virtual ~engine() {}

    mat2x3 scale = grottans::mat2x3::scale(1.f, 1.f);

    mouse_pos mouse_coord;
    mouse_pos mouse_coord_pressed;
    mouse_pos mouse_coord_released;

    std::vector<game_state*> states;
    game_state* current_state;

    bool loop;
};

engine* create_engine();
void destroy_engine(engine* e);

} // end of namespace grottans

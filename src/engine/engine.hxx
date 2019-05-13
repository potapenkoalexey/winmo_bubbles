#pragma once

#include <iosfwd>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "./color.hxx"
#include "./math_structures.hxx"
#include "./sound_buffer.hxx"

extern size_t WIDTH;
extern size_t HEIGHT;
extern size_t BLOCK;

namespace grottans {

class game_state;

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
    mouse_pos();
    mouse_pos(int x, int y);
    int x = 0;
    int y = 0;
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

struct vertex {
    vertex()
        : x(0.f)
        , y(0.f)
        , tx(0.f)
        , ty(0.f)
    {
    }
    float x;
    float y;
    float tx;
    float ty;
};

std::ostream& operator<<(std::ostream& stream, const event e);
std::istream& operator>>(std::istream& is, vertex&);
std::istream& operator>>(std::istream& is, uv_pos&);

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

class texture {
public:
    virtual ~texture();
    virtual std::uint32_t get_width() const = 0;
    virtual std::uint32_t get_height() const = 0;
};

class vertex_buffer {
public:
    virtual const v2* data() const = 0;
    virtual size_t size() const = 0;
    virtual ~vertex_buffer();
};

struct membuf : public std::streambuf {
    membuf()
        : std::streambuf()
        , buf()
        , buf_size(0)
    {
    }
    membuf(std::unique_ptr<char[]> buffer, size_t size)
        : std::streambuf()
        , buf(std::move(buffer))
        , buf_size(size)
    {
        char* beg_ptr = buf.get();
        char* end_ptr = beg_ptr + buf_size;
        setg(beg_ptr, beg_ptr, end_ptr);
        setp(beg_ptr, end_ptr);
    }
    membuf(membuf&& other)
    {
        setp(nullptr, nullptr);
        setg(nullptr, nullptr, nullptr);

        other.swap(*this);

        buf = std::move(other.buf);
        buf_size = other.buf_size;

        other.buf_size = 0;
    }

    pos_type seekoff(off_type pos, std::ios_base::seekdir seek_dir,
        std::ios_base::openmode) override
    {
        // TODO implement it in correct way
        if (seek_dir == std::ios_base::beg) {
            return 0 + pos;
        } else if (seek_dir == std::ios_base::end) {
            return buf_size + pos;
        } else {
            return egptr() - gptr();
        }
    }

    char* begin() const { return eback(); }
    size_t size() const { return buf_size; }

private:
    std::unique_ptr<char[]> buf;
    size_t buf_size;
};

membuf load_file(std::string_view path);

class engine {
public:
    // Engine() {}

    virtual std::string initialize() = 0;
    virtual float get_time_from_init() = 0;
    virtual bool load_texture(std::string_view) = 0;
    virtual bool input(event& e) = 0;
    virtual bool is_key_down(const keys) = 0;

    virtual void disable_event(event& e) = 0;
    virtual void enable_event(event& e) = 0;

    virtual void change_state(game_state* state) = 0;
    virtual void push_state(game_state* state) = 0;
    virtual void pop_state() = 0;

    virtual texture* create_texture(std::string path) = 0;
    virtual void destroy_texture(texture* t) = 0;
    virtual vertex_buffer* create_vertex_buffer(const tri2*, std::size_t) = 0;
    virtual void destroy_vertex_buffer(vertex_buffer*) = 0;

    virtual sound_buffer* create_sound_buffer(std::string_view path) = 0;
    virtual void destroy_sound_buffer(sound_buffer*) = 0;

    virtual void render_triangle(const triangle&) = 0;
    virtual void render(const tri0&, const color&) = 0;
    virtual void render(const tri1&) = 0;
    virtual void render(const tri2&, texture*) = 0;
    virtual void render(const tri2&, texture*, const mat2x3& m) = 0;
    virtual void render(const vertex_buffer&, texture*, const mat2x3&) = 0;

    virtual void set_window_title(const char*) = 0;
    virtual size_t get_window_width() = 0;
    virtual size_t get_window_height() = 0;

    virtual void swap_buffers() = 0;
    virtual void uninitialize() = 0;

    virtual ~engine() {}

    mat2x3 scale = grottans::mat2x3::scale(1.f, 1.f);

    mouse_pos mouse_coord;

    std::vector<game_state*> states;

    bool mouse_moution;
    bool loop;
};

engine* create_engine();
void destroy_engine(engine* e);

class game_state {
public:
    virtual bool init(grottans::engine*) = 0;
    virtual void cleanup(grottans::engine*) = 0;

    virtual void pause(grottans::engine*) = 0;
    virtual void resume(grottans::engine*) = 0;

    virtual void handle_events(grottans::engine*) = 0;
    virtual void update(grottans::engine*) = 0;
    virtual void draw(grottans::engine*) = 0;

    void change_state(grottans::engine* engine, game_state* state)
    {
        engine->change_state(state);
    }

    virtual ~game_state();

protected:
    game_state() {}
};

} // end of namespace grottans

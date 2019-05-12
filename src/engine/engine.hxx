#pragma once

#include <iosfwd>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "./color.hxx"
#include "./sound_buffer.hxx"

extern size_t WIDTH;
extern size_t HEIGHT;
extern size_t BLOCK;

namespace grottans {

class game_state;

struct mouse_pos {
    mouse_pos();
    mouse_pos(int x, int y);
    int x = 0;
    int y = 0;
};

struct vec2 {
    vec2();
    vec2(float x, float y);
    float x = 0;
    float y = 0;
};

vec2 operator+(const vec2& l, const vec2& r);

struct mat2 {
    mat2();
    static mat2 identiry();
    static mat2 scale(float scale);
    static mat2 rotation(float thetha);
    vec2 col0;
    vec2 col1;
};

struct mat2x3 {
    mat2x3();
    static mat2x3 identiry();
    static mat2x3 scale(float scale);
    static mat2x3 scale(float sx, float sy);
    static mat2x3 rotation(float thetha);
    static mat2x3 move(const vec2& delta);
    vec2 col0;
    vec2 col1;
    vec2 delta;
};

vec2 operator*(const vec2& v, const mat2& m);
mat2 operator*(const mat2& m1, const mat2& m2);
vec2 operator*(const vec2& v, const mat2x3& m);
mat2x3 operator*(const mat2x3& m1, const mat2x3& m2);

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

/// vertex with position only
struct v0 {
    vec2 pos;
};

/// vertex with position and texture coordinate
struct v1 {
    vec2 pos;
    color c;
};
/// vertex position + color + texture coordinate
struct v2 {
    vec2 pos;
    vec2 uv;
    color c;
};

/// triangle with positions only
struct tri0 {
    tri0();
    v0 v[3];
};

/// triangle with positions and color
struct tri1 {
    tri1();
    v1 v[3];
};

/// triangle with positions color and texture coordinate
struct tri2 {
    tri2();
    v2 v[3];
};

v2 operator+(const v2& l, const v2& r);
tri2 operator+(const tri2& l, const tri2& r);

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

struct triangle {
    triangle()
    {
        v[0] = vertex();
        v[1] = vertex();
        v[2] = vertex();
    }
    vertex v[3];
};

std::ostream& operator<<(std::ostream& stream, const event e);
std::istream& operator>>(std::istream& is, vertex&);
std::istream& operator>>(std::istream& is, triangle&);
std::istream& operator>>(std::istream& is, uv_pos&);
std::istream& operator>>(std::istream& is, v0&);
std::istream& operator>>(std::istream& is, v1&);
std::istream& operator>>(std::istream& is, v2&);
std::istream& operator>>(std::istream& is, tri0&);
std::istream& operator>>(std::istream& is, tri1&);
std::istream& operator>>(std::istream& is, tri2&);

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

class game_state {
public:
    virtual void init(grottans::engine*) = 0;
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

protected:
    game_state() {}
};

engine* create_engine();
void destroy_engine(engine* e);

membuf load_file(std::string_view path);

//tri0 blend(const tri0& tl, const tri0& tr, const float a);

} // end of namespace grottans

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "picopng/picopng.hxx"
#include "include/engine.hxx"
#include "sound_buffer.hxx"

#include "SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL_opengl.h"
#include "SDL_opengl_glext.h"

#define STB_IMAGE_IMPLEMENTATION
#pragma GCC diagnostic push

// turn off the specific warning. Can also use "-Wall"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "stb/stb_image.h"
#pragma GCC diagnostic pop

static PFNGLCREATESHADERPROC glCreateShader = nullptr;
static PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
static PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
static PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
static PFNGLDELETESHADERPROC glDeleteShader = nullptr;
static PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
static PFNGLATTACHSHADERPROC glAttachShader = nullptr;
static PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;
static PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
static PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
static PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
static PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
static PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
static PFNGLUNIFORM1IPROC glUniform1i = nullptr;
static PFNGLACTIVETEXTUREPROC glActiveTexture_ = nullptr;
static PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
static PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;
static PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
static PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
static PFNGLBUFFERDATAPROC glBufferData = nullptr;
static PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;

template <typename T>
static void load_gl_func(const char* func_name, T& result)
{
    void* gl_pointer = SDL_GL_GetProcAddress(func_name);
    if (nullptr == gl_pointer) {
        throw std::runtime_error(std::string("can't load GL function") + func_name);
    }
    result = reinterpret_cast<T>(gl_pointer);
}

#ifdef GL_CHECK_FOR_DEBUG
#define OM_GL_CHECK()                                                              \
    {                                                                              \
        const GLenum err = glGetError();                                           \
        if (err != GL_NO_ERROR) {                                                  \
            switch (err) {                                                         \
            case GL_INVALID_ENUM:                                                  \
                std::cerr << "GL_INVALID_ENUM" << std::endl;                       \
                break;                                                             \
            case GL_INVALID_VALUE:                                                 \
                std::cerr << "GL_INVALID_VALUE" << std::endl;                      \
                break;                                                             \
            case GL_INVALID_OPERATION:                                             \
                std::cerr << "GL_INVALID_OPERATION" << std::endl;                  \
                break;                                                             \
            case GL_INVALID_FRAMEBUFFER_OPERATION:                                 \
                std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;      \
                break;                                                             \
            case GL_OUT_OF_MEMORY:                                                 \
                std::cerr << "GL_OUT_OF_MEMORY" << std::endl;                      \
                break;                                                             \
            }                                                                      \
            std::cerr << __FILE__ << ':' << __LINE__ << '(' << __FUNCTION__ << ')' \
                      << std::endl;                                                \
            assert(false);                                                         \
        }                                                                          \
    }
#else
#define OM_GL_CHECK() \
    {                 \
    }
#endif

namespace grottans {

///////////////////////////////////////////////////////////////////////////////
/// \brief The texture_gl_es20 class
///
class texture_gl_es20 : public texture {
public:
    explicit texture_gl_es20(const std::string& path);
    ~texture_gl_es20() override;

    void bind() const
    {
        glBindTexture(GL_TEXTURE_2D, tex_handl);
        OM_GL_CHECK();
    }

    std::uint32_t get_width() const final { return width; }
    std::uint32_t get_height() const final { return height; }

private:
    std::string file_path;
    GLuint tex_handl = 0;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief The shader_gl_es20 class
///
class shader_gl_es20 {
public:
    shader_gl_es20( //constructor
        std::string_view vertex_src, std::string_view fragment_src,
        const std::vector<std::tuple<GLuint, const GLchar*> >& attributes)
    {
        vert_shader = compile_shader(GL_VERTEX_SHADER, vertex_src);
        frag_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_src);
        if (vert_shader == 0 || frag_shader == 0) {
            throw std::runtime_error("can't compile shader");
        }
        program_id = link_shader_program(attributes);
        if (program_id == 0) {
            throw std::runtime_error("can't link shader");
        }
    }

    void use() const
    {
        glUseProgram(program_id);
        OM_GL_CHECK();
    }

    void set_uniform(std::string_view uniform_name, texture_gl_es20* texture)
    {
        assert(texture != nullptr);
        const int location = glGetUniformLocation(program_id, uniform_name.data());
        OM_GL_CHECK();
        if (location == -1) {
            std::cerr << "can't get uniform location from shader\n";
            throw std::runtime_error("can't get uniform location");
        }
        unsigned int texture_unit = 0;
        glActiveTexture_(GL_TEXTURE0 + texture_unit);
        OM_GL_CHECK();

        texture->bind();

        // http://www.khronos.org/opengles/sdk/docs/man/xhtml/glUniform.xml
        glUniform1i(location, static_cast<int>(0 + texture_unit));
        OM_GL_CHECK();
    }

    void set_uniform(std::string_view uniform_name, const color& c)
    {
        const int location = glGetUniformLocation(program_id, uniform_name.data());
        OM_GL_CHECK();
        if (location == -1) {
            std::cerr << "can't get uniform location from shader\n";
            throw std::runtime_error("can't get uniform location");
        }
        float values[4] = { c.get_r(), c.get_g(), c.get_b(), c.get_a() };
        glUniform4fv(location, 1, &values[0]);
        OM_GL_CHECK();
    }

    void set_uniform(std::string_view uniform_name, const mat2x3& m)
    {
        const int location = glGetUniformLocation(program_id, uniform_name.data());
        OM_GL_CHECK();
        if (location == -1) {
            std::cerr << "can't get uniform location from shader\n";
            throw std::runtime_error("can't get uniform location");
        }
        // OpenGL wants matrix in column major order
        // clang-format off
        float values[9] = { m.col0.x,  m.col0.y, m.delta.x,
                            m.col1.x, m.col1.y, m.delta.y,
                            0.f,      0.f,       1.f };
        // clang-format on
        glUniformMatrix3fv(location, 1, GL_FALSE, &values[0]);
        OM_GL_CHECK();
    }

private:
    GLuint compile_shader(GLenum shader_type, std::string_view src)
    {
        GLuint shader_id = glCreateShader(shader_type);
        OM_GL_CHECK();
        std::string_view vertex_shader_src = src;
        const char* source = vertex_shader_src.data();
        glShaderSource(shader_id, 1, &source, nullptr);
        OM_GL_CHECK();

        glCompileShader(shader_id);
        OM_GL_CHECK();

        GLint compiled_status = 0;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK();
        if (compiled_status == 0) {
            GLint info_len = 0;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK();
            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(shader_id, info_len, nullptr, info_chars.data()); //NULL
            OM_GL_CHECK();
            glDeleteShader(shader_id);
            OM_GL_CHECK();

            std::string shader_type_name = shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment";
            std::cerr << "Error compiling shader(vertex)\n"
                      << vertex_shader_src << "\n"
                      << info_chars.data();
            return 0;
        }
        return shader_id;
    }

    GLuint link_shader_program(
        const std::vector<std::tuple<GLuint, const GLchar*> >& attributes)
    {
        GLuint program_id_ = glCreateProgram();
        OM_GL_CHECK();
        if (0 == program_id_) {
            std::cerr << "failed to create gl program";
            throw std::runtime_error("can't link shader");
        }

        glAttachShader(program_id_, vert_shader);
        OM_GL_CHECK();
        glAttachShader(program_id_, frag_shader);
        OM_GL_CHECK();

        // bind attribute location
        for (const auto& attr : attributes) {
            GLuint loc = std::get<0>(attr);
            const GLchar* name = std::get<1>(attr);
            glBindAttribLocation(program_id_, loc, name);
            OM_GL_CHECK();
        }

        // link program after binding attribute locations
        glLinkProgram(program_id_);
        OM_GL_CHECK();
        // Check the link status
        GLint linked_status = 0;
        glGetProgramiv(program_id_, GL_LINK_STATUS, &linked_status);
        OM_GL_CHECK();
        if (linked_status == 0) {
            GLint infoLen = 0;
            glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &infoLen);
            OM_GL_CHECK();
            std::vector<char> infoLog(static_cast<size_t>(infoLen));
            glGetProgramInfoLog(program_id_, infoLen, NULL, infoLog.data());
            OM_GL_CHECK();
            std::cerr << "Error linking program:\n"
                      << infoLog.data();
            glDeleteProgram(program_id_);
            OM_GL_CHECK();
            return 0;
        }
        return program_id_;
    }

    GLuint vert_shader = 0;
    GLuint frag_shader = 0;
    GLuint program_id = 0;
};

static std::array<std::string_view, 20> event_names = {
    { /// input events
        "left_pressed", "left_released", "right_pressed", "right_released",
        "up_pressed", "up_released", "down_pressed", "down_released",
        "escape_pressed", "escape_released", "start_pressed", "start_released",
        "button1_pressed", "button1_released", "button2_pressed",
        "button2_released", "mouse_motion", "mouse_pressed", "mouse_released",
        /// virtual console events
        "turn_off" }
};

struct bind {
    bind(std::string_view s, SDL_Keycode k, event pressed, event released, keys grottans_k)
        : key(k)
        , name(s)
        , event_pressed(pressed)
        , event_released(released)
        , grottans_key(grottans_k)
    {
    }

    SDL_Keycode key;
    std::string_view name;
    event event_pressed;
    event event_released;
    grottans::keys grottans_key;
};

const std::array<bind, 9> keys{
    { bind{ "up", SDLK_UP, event::up_pressed, event::up_released, keys::up },
        bind{ "left", SDLK_LEFT, event::left_pressed, event::left_released, keys::left },
        bind{ "down", SDLK_DOWN, event::down_pressed, event::down_released, keys::down },
        bind{ "right", SDLK_RIGHT, event::right_pressed, event::right_released, keys::right },
        bind{ "button1", SDLK_LCTRL, event::button1_pressed, event::button1_released, keys::button1 },
        bind{ "button2", SDLK_SPACE, event::button2_pressed, event::button2_released, keys::button2 },
#ifdef __ANDROID__
        bind{ "escape", SDLK_AC_BACK, event::escape_pressed, event::escape_released, keys::select },
#else
        bind{ "escape", SDLK_ESCAPE, event::escape_pressed, event::escape_released, keys::select },
#endif
        bind{ "start", SDLK_RETURN, event::start_pressed, event::start_released, keys::start },
        bind{ "mouse", SDL_MOUSEBUTTONDOWN, event::mouse_pressed, event::mouse_released, keys::mouse } }
};

std::ostream& operator<<(std::ostream& stream, const event e)
{
    std::uint32_t value = static_cast<std::uint32_t>(e);
    std::uint32_t minimal = static_cast<std::uint32_t>(event::left_pressed);
    std::uint32_t maximal = static_cast<std::uint32_t>(event::turn_off);
    if (value >= minimal && value <= maximal) {
        stream << event_names[value];
        return stream;
    } else {
        throw std::runtime_error("too big event value");
    }
}

std::istream& operator>>(std::istream& is, vertex& v)
{
    is >> v.x;
    is >> v.y;

    is >> v.tx;
    is >> v.ty;

    return is;
}

std::istream& operator>>(std::istream& is, triangle& t)
{
    is >> t.v[0];
    is >> t.v[1];
    is >> t.v[2];
    return is;
}

std::istream& operator>>(std::istream& is, uv_pos& uv)
{
    is >> uv.u;
    is >> uv.v;
    return is;
}

vertex_buffer::~vertex_buffer() {}
///////////////////////////////////////////////////////////////////////////////
/// \brief The VertexBufferImpl class
///
class VertexBufferImpl : public vertex_buffer {
public:
    VertexBufferImpl(const tri2* tri, std::size_t n)
        : triangles(n)
    {
        assert(tri != nullptr);
        for (size_t i = 0; i < n; ++i) {
            triangles[i] = tri[i];
        }
    }

    ~VertexBufferImpl();

    const v2* data() const { return &triangles.data()->v[0]; }
    virtual size_t size() const { return triangles.size() * 3; }

private:
    std::vector<tri2> triangles;
};

VertexBufferImpl::~VertexBufferImpl() {}

///////////////////////////////////////////////////////////////////////////////
bool check_input(const SDL_Event& e, const bind*& result)
{
    using namespace std;

    const auto it = find_if(begin(keys), end(keys), [&](const bind& b) {
        return b.key == e.key.keysym.sym;
    });

    if (it != end(keys)) {
        result = &(*it);
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief The EngineReal class
///
class engine_impl : public engine {
public:
    std::string initialize();
    float get_time_from_init();
    bool load_texture(std::string_view);
    bool input(event& e);
    //mouse_pos mouse();

    void disable_mouse_moution_event();
    void enable_mouse_moution_event();

    bool is_key_down(const enum keys key);
    void render_triangle(const triangle&);

    texture* create_texture(const std::string& path);
    void destroy_texture(texture* t);
    vertex_buffer* create_vertex_buffer(const tri2*, const size_t&);
    void destroy_vertex_buffer(vertex_buffer*);

    sound_buffer* create_sound_buffer(std::string_view path);
    void destroy_sound_buffer(sound_buffer* sound);

    std::stringstream load_txt_and_filter_comments(std::string_view file);

    void render(const tri0&, const color&);
    void render(const tri1&);
    void render(const tri2&, texture*);
    void render(const tri2& t, texture*, const mat2x3&);
    void render(const vertex_buffer&, texture*, const mat2x3&);

    void set_window_title(const char* name);
    void set_window_size(const size_t& w, const size_t& h);
    size_t get_window_width();
    size_t get_window_height();

    void swap_buffers();
    void uninitialize();

    void switch_to_state_and_resume(game_state*);
    void push_state_and_init(game_state* state);
    void clear_states();
    void swap_last_two_states();
    void change_state(game_state* state);
    void pop_state();

    bool is_mouse_clicked_in_coord(const float& lx, const float& rx, const float& ly, const float& ry);

    ~engine_impl();
    //  mouse_pos mouse_coord;

    size_t screen_width = 0;
    size_t screen_height = 0;

private:
    static void audio_callback(void*, uint8_t*, int);
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;

    shader_gl_es20* shader00 = nullptr;
    shader_gl_es20* shader01 = nullptr;
    shader_gl_es20* shader02 = nullptr;
    shader_gl_es20* shader03 = nullptr;
    uint32_t gl_default_vbo = 0;

    SDL_AudioDeviceID audio_device;
    SDL_AudioSpec audio_device_spec;
    std::vector<sound_buffer_impl*> sounds;
};

void engine_impl::disable_mouse_moution_event()
{
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
}
void engine_impl::enable_mouse_moution_event()
{
    //SDL_EventState(SDL_SCANCODE_RETURN, SDL_ENABLE);
}

sound_buffer* engine_impl::create_sound_buffer(const std::string_view path)
{
    SDL_LockAudioDevice(audio_device); // TODO fix lock only push_back
    sound_buffer_impl* s = new sound_buffer_impl(path, audio_device, audio_device_spec);
    sounds.push_back(s);
    SDL_UnlockAudioDevice(audio_device);
    return s;
}
void engine_impl::destroy_sound_buffer(sound_buffer* sound)
{
    // TODO FIXME first remove from sounds collection
    delete sound;
}

std::stringstream engine_impl::load_txt_and_filter_comments(std::string_view file)
{
    std::stringstream out;
    std::string line;

    grottans::membuf buf = grottans::open_load_file_to_membuf(file, "rb");
    std::istream in(&buf);

    if (!in) {
        throw std::runtime_error(std::string("can't open file: ") + file.data());
    }

    while (std::getline(in, line)) {
        size_t comment_pos = line.find("//");
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        if (!line.empty()) {
            out << line << '\n';
        }
    }

    return out;
}

///////////////////////////////////////////////////////////////////////////
/// \brief EngineReal::createVertexBuffer
///
vertex_buffer* engine_impl::create_vertex_buffer(const tri2* triangles, const size_t& n)
{
    return new VertexBufferImpl(triangles, n);
}
void engine_impl::destroy_vertex_buffer(vertex_buffer* buffer) { delete buffer; }

///////////////////////////////////////////////////////////////////////////
/// \brief is_key_down
///
bool engine_impl::is_key_down(const enum keys key)
{
    const auto it = std::find_if(begin(keys), end(keys),
        [&](const bind& b) { return b.grottans_key == key; });

    if (it != end(keys)) {
        const std::uint8_t* state = SDL_GetKeyboardState(nullptr);
        int sdl_scan_code = SDL_GetScancodeFromKey(it->key);
        return state[sdl_scan_code];
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief EngineReal::input
///
bool engine_impl::input(event& e)
{
    using namespace std;
    while (true) {
        // collect all events from SDL
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event)) { //or SDL_WaitEevent
            const bind* binding = nullptr;

            //mouse
            if (sdl_event.type == SDL_MOUSEBUTTONDOWN) {
                e = grottans::event::mouse_pressed;
                this->mouse_coord_pressed.x = sdl_event.button.x;
                this->mouse_coord_pressed.y = sdl_event.button.y;
                return true;
            }

            if (sdl_event.type == SDL_MOUSEBUTTONUP) {
                e = grottans::event::mouse_released;
                this->mouse_coord_released.x = sdl_event.button.x;
                this->mouse_coord_released.y = sdl_event.button.y;
                return true;
            }

            ///can be commented
            if (sdl_event.type == SDL_MOUSEMOTION) {
                e = grottans::event::mouse_motion;
                this->mouse_coord.x = sdl_event.motion.x;
                this->mouse_coord.y = sdl_event.motion.y;
                return true;
            }

            //buttons
            if (sdl_event.type == SDL_QUIT) {
                e = grottans::event::turn_off;
                return true;
            } else if (sdl_event.type == SDL_KEYDOWN) {
                if (check_input(sdl_event, binding)) {
                    e = binding->event_pressed;
                    return true;
                }
            } else if (sdl_event.type == SDL_KEYUP) {
                if (check_input(sdl_event, binding)) {
                    e = binding->event_released;
                    return true;
                }
            }
        }
        return false;
    }
}

//mouse_pos Engine::mouse()
//{
//    mouse_pos result;
//    return result;
//}

///////////////////////////////////////////////////////////////////////////////
/// \brief EngineReal::loadTexture
///
bool engine_impl::load_texture(const std::string_view path)
{
    std::vector<unsigned char> png_file_in_memory;
    std::ifstream ifs(path.data(), std::ios_base::binary);
    if (!ifs) {
        return false;
    }
    ifs.seekg(0, std::ios_base::end);
    size_t pos_in_file = static_cast<size_t>(ifs.tellg());
    png_file_in_memory.resize(pos_in_file);
    ifs.seekg(0, std::ios_base::beg);
    if (!ifs) {
        return false;
    }

    ifs.read(reinterpret_cast<char*>(png_file_in_memory.data()), pos_in_file);
    if (!ifs.good()) {
        return false;
    }

    std::vector<unsigned char> image; //keep png
    unsigned long w = 0;
    unsigned long h = 0;
    int error = decodePNG(image, w, h, &png_file_in_memory[0],
        png_file_in_memory.size(), false);

    // if there's an error, display it
    if (error != 0) {
        std::cerr << "error: " << error << std::endl;
        return false;
    }

    GLuint tex_handl = 0;
    glGenTextures(1, &tex_handl);
    OM_GL_CHECK();
    glBindTexture(GL_TEXTURE_2D, tex_handl);
    OM_GL_CHECK();

    GLint mipmap_level = 0;
    GLint border = 0;
    glTexImage2D(GL_TEXTURE_2D, mipmap_level, GL_RGBA, w, h, border,
        GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    OM_GL_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    OM_GL_CHECK();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    OM_GL_CHECK();
    return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief EngineReal::renderTriangle
///
void engine_impl::render_triangle(const triangle& t)
{
    // vertex coordinates
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &t.v[0].x);
    OM_GL_CHECK();

    glEnableVertexAttribArray(0);
    OM_GL_CHECK();

    // texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), &t.v[0].tx);
    OM_GL_CHECK();

    glEnableVertexAttribArray(1);
    OM_GL_CHECK();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    OM_GL_CHECK();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief EngineReal::render
///
void engine_impl::render(const tri0& t, const color& c)
{
    shader00->use();
    shader00->set_uniform("u_color", c);
    // vertex coordinates
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(v0),
        &t.v[0].pos.x);
    OM_GL_CHECK();
    glEnableVertexAttribArray(0);
    OM_GL_CHECK();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    OM_GL_CHECK();
}

void engine_impl::render(const tri1& t)
{
    shader01->use();
    // positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(t.v[0]), &t.v[0].pos);
    OM_GL_CHECK();
    glEnableVertexAttribArray(0);
    OM_GL_CHECK();
    // colors
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(t.v[0]), &t.v[0].c);
    OM_GL_CHECK();
    glEnableVertexAttribArray(1);
    OM_GL_CHECK();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    OM_GL_CHECK();

    glDisableVertexAttribArray(1);
    OM_GL_CHECK();
}

void engine_impl::render(const tri2& t, texture* tex)
{
    shader02->use();
    texture_gl_es20* texture = static_cast<texture_gl_es20*>(tex);
    texture->bind();
    shader02->set_uniform("s_texture", texture);
    // positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(t.v[0]),
        &t.v[0].pos);
    OM_GL_CHECK();
    glEnableVertexAttribArray(0);
    OM_GL_CHECK();
    // colors
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(t.v[0]),
        &t.v[0].c);
    OM_GL_CHECK();
    glEnableVertexAttribArray(1);
    OM_GL_CHECK();

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(t.v[0]),
        &t.v[0].uv);
    OM_GL_CHECK();
    glEnableVertexAttribArray(2);
    OM_GL_CHECK();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    OM_GL_CHECK();

    glDisableVertexAttribArray(1);
    OM_GL_CHECK();
    glDisableVertexAttribArray(2);
    OM_GL_CHECK();
}

void engine_impl::render(const tri2& t, texture* tex, const mat2x3& m)
{
    shader03->use();
    texture_gl_es20* texture = static_cast<texture_gl_es20*>(tex);
    texture->bind();
    shader03->set_uniform("s_texture", texture);
    shader03->set_uniform("u_matrix", m);
    // positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(t.v[0]),
        &t.v[0].pos);
    OM_GL_CHECK();
    glEnableVertexAttribArray(0);
    OM_GL_CHECK();
    // colors
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(t.v[0]),
        &t.v[0].c);
    OM_GL_CHECK();
    glEnableVertexAttribArray(1);
    OM_GL_CHECK();

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(t.v[0]),
        &t.v[0].uv);
    OM_GL_CHECK();
    glEnableVertexAttribArray(2);
    OM_GL_CHECK();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    OM_GL_CHECK();

    glDisableVertexAttribArray(1);
    OM_GL_CHECK();
    glDisableVertexAttribArray(2);
    OM_GL_CHECK();
}

void engine_impl::render(const vertex_buffer& buff, texture* tex, const mat2x3& m)
{
    shader03->use();
    texture_gl_es20* texture = static_cast<texture_gl_es20*>(tex);
    texture->bind();
    shader03->set_uniform("s_texture", texture);
    shader03->set_uniform("u_matrix", m);

    assert(gl_default_vbo != 0);

    glBindBuffer(GL_ARRAY_BUFFER, gl_default_vbo);
    OM_GL_CHECK();

    const v2* t = buff.data();
    uint32_t data_size_in_bytes = static_cast<uint32_t>(buff.size() * sizeof(v2));
    glBufferData(GL_ARRAY_BUFFER, data_size_in_bytes, t, GL_DYNAMIC_DRAW);
    OM_GL_CHECK();
    glBufferSubData(GL_ARRAY_BUFFER, 0, data_size_in_bytes, t);
    OM_GL_CHECK();

    // positions
    glEnableVertexAttribArray(0);
    OM_GL_CHECK();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(v2), nullptr);
    OM_GL_CHECK();
    // colors
    glVertexAttribPointer(
        1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(v2),
        reinterpret_cast<void*>(sizeof(v2::pos) + sizeof(v2::uv)));
    OM_GL_CHECK();
    glEnableVertexAttribArray(1);
    OM_GL_CHECK();

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(v2),
        reinterpret_cast<void*>(sizeof(v2::pos)));
    OM_GL_CHECK();
    glEnableVertexAttribArray(2);
    OM_GL_CHECK();

    GLsizei num_of_vertexes = static_cast<GLsizei>(buff.size());
    glDrawArrays(GL_TRIANGLES, 0, num_of_vertexes);
    OM_GL_CHECK();

    glDisableVertexAttribArray(1);
    OM_GL_CHECK();
    glDisableVertexAttribArray(2);
    OM_GL_CHECK();
}

///////////////////////////////////////////////////////////////////////////////
void engine_impl::set_window_title(const char* name)
{
    SDL_SetWindowTitle(window, name);
}

void engine_impl::set_window_size(const size_t& w, const size_t& h)
{
    if (w <= 0 || h <= 0) {
        throw std::runtime_error(std::string("can't set the screen size: incorrect numbers"));
    }
    screen_width = w;
    screen_height = h;
    scale = grottans::mat2x3::scale(1.f, 1.f);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief texture_gl_es20::texture_gl_es20
///
texture_gl_es20::texture_gl_es20(const std::string& path)
    : file_path{ path }
{
    membuf file_contents = grottans::open_load_file_to_membuf(path, "rb");

    //stbi_set_flip_vertically_on_load(true);

    int w = 0;
    int h = 0;
    int components = 0;
    unsigned char* image = stbi_load_from_memory(reinterpret_cast<unsigned char*>(file_contents.begin()),
        file_contents.size(), &w, &h, &components, 4);

    // if there's an error, display it
    if (image == nullptr) {
        std::cerr << "error: can't load file: " << path << std::endl;
        throw std::runtime_error("can't load texture");
    }

    glGenTextures(1, &tex_handl);
    OM_GL_CHECK();
    glBindTexture(GL_TEXTURE_2D, tex_handl);
    OM_GL_CHECK();

    GLint mipmap_level = 0;
    GLint border = 0;
    GLsizei width_ = static_cast<GLsizei>(w);
    GLsizei height_ = static_cast<GLsizei>(h);
    glTexImage2D(GL_TEXTURE_2D, mipmap_level, GL_RGBA, width_, height_, border,
        GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    OM_GL_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    OM_GL_CHECK();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    OM_GL_CHECK();
}

texture_gl_es20::~texture_gl_es20()
{
    glDeleteTextures(1, &tex_handl);
    OM_GL_CHECK();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief EngineReal::getTimeFromInit
/// \return
///
float engine_impl::get_time_from_init()
{
    std::uint32_t ms_from_library_initialization = SDL_GetTicks();
    float seconds = ms_from_library_initialization * 0.001f;
    return seconds;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief EngineReal::createTexture
///
texture* engine_impl::create_texture(const std::string& path)
{
    return new texture_gl_es20(path);
}

void engine_impl::destroy_texture(texture* t) { delete t; }

///////////////////////////////////////////////////////////////////////////////
/// \brief swap_buffers
///
void engine_impl::swap_buffers()
{
    SDL_GL_SwapWindow(window);
    glClear(GL_COLOR_BUFFER_BIT);
    OM_GL_CHECK();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Engine::uninitialize
///
void engine_impl::uninitialize()
{
    //cleaning stack if the states
    while (!states.empty()) {
        states.back()->cleanup();
        states.pop_back();
    }

    SDL_GL_DeleteContext(gl_context);
    extern DECLSPEC void SDLCALL TTF_Quit(void);
    SDL_DestroyWindow(window);
    SDL_Quit();
    //destroy_engine(this);
}

///////////////////////////////////////////////////////////////////////////////
static bool already_exist = false;

engine* create_engine()
{
    if (already_exist) {
        throw std::runtime_error("engine already exist");
    }
    engine* result = new engine_impl();
    already_exist = true;
    return result;
}

void destroy_engine(engine* e)
{
    if (already_exist == false) {
        throw std::runtime_error("engine not created");
    }
    if (nullptr == e) {
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Engine::initialize
/// \return
///
std::string engine_impl::initialize()
{
    using namespace std;

    loop = true;

    stringstream serr;

    const int init_result = SDL_Init(
        SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER);
    //SDL_INIT_EVERYTHING);
    if (init_result != 0) {
        const char* err_message = SDL_GetError();
        std::cerr << "error: initialization failed: " << err_message << std::endl;
    }

    //init SDL_TTF
    extern DECLSPEC int SDLCALL TTF_Init(void);

    // get and set the desired window size
    // game screen factor = 536/480 = 1.096
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    std::cout << "screen refresh rate = " << DM.refresh_rate << std::endl;
    size_t w = static_cast<size_t>(DM.w);
    size_t h = static_cast<size_t>(DM.h);
//swap w&h if needed
//    if (w > h) {
//        std::swap(h, w);
//    }

#ifdef __unix__
  #ifndef __ANDROID__
    //  if (screen_width == 0) { //if don't set screen size with set_screen_size(w,h);
    screen_height = h / 2;
    screen_width = w / 2; //1.116
    if (w > h) {
        scale = grottans::mat2x3::scale(screen_height / (double)screen_width, 1.f);
    } else {
        scale = grottans::mat2x3::scale(1.f, screen_width / (double)screen_height);
        //scale = grottans::mat2x3::scale(1.f, 1.f);
    }
// }
  #endif
#endif
#ifdef _WIN32
    screen_height = h;
    screen_width = w; //1.116
    if (w > h) {
        scale = grottans::mat2x3::scale(screen_height / (double)screen_width, 1.f);
    } else {
        scale = grottans::mat2x3::scale(1.f, screen_width / (double)screen_height);
        //scale = grottans::mat2x3::scale(1.f, 1.f);
    }
//    screen_height = h / 2;
//    screen_width = screen_height / 1.096;
#endif
#ifdef __ANDROID__
    screen_height = h;
    screen_width = w; //1.116
    if (w > h) {
        scale = grottans::mat2x3::scale(h / (double)w, 1.f);
    } else {
        scale = grottans::mat2x3::scale(1.f, w / (double)h);
        //scale = grottans::mat2x3::scale(1.f, 1.f);
    }
#endif

// opening window
#ifdef __unix__
  #ifndef __ANDROID__
    window = SDL_CreateWindow("WinMo Bubbles",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
        ::SDL_WINDOW_OPENGL);
            // | SDL_WINDOW_BORDERLESS //without title
            // | SDL_WINDOW_FULLSCREEN); //240x268
  #endif
#endif
#ifdef _WIN32
    // app works better in fullscreen mode on Windows
    window = SDL_CreateWindow("WinMo Bubbles",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
        ::SDL_WINDOW_OPENGL
            | SDL_WINDOW_BORDERLESS //without title
            | SDL_WINDOW_FULLSCREEN); //240x268);
#endif
#ifdef __ANDROID__
    window = SDL_CreateWindow("WinMo Bubbles",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
        ::SDL_WINDOW_OPENGL);
        // | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);
#endif

    if (window == nullptr) {
        const char* err_message = SDL_GetError();
        std::cerr << "error: Create Window failed: " << err_message << std::endl;
        SDL_Quit();
    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief version GL
    ///
    int gl_major_ver = 2;
    int gl_minor_ver = 0;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);

    //create context
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr) {
        std::string msg("can't create opengl context: ");
        msg += SDL_GetError();
        serr << msg << endl;
        cout << serr.str();
    }
    //std::cout << gl_context << '\n' << std::endl;

    int result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);
    SDL_assert(result == 0);

    result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_ver);
    SDL_assert(result == 0);

    if (gl_major_ver < 2) {
        std::clog << "current context opengl version: " << gl_major_ver << '.'
                  << gl_minor_ver << '\n'
                  << "need openg version at least: 2.1\n"
                  << std::flush;
        throw std::runtime_error("opengl version too low");
    }

    try {
        load_gl_func("glCreateShader", glCreateShader);
        load_gl_func("glShaderSource", glShaderSource);
        load_gl_func("glCompileShader", glCompileShader);
        load_gl_func("glGetShaderiv", glGetShaderiv);
        load_gl_func("glGetShaderInfoLog", glGetShaderInfoLog);
        load_gl_func("glDeleteShader", glDeleteShader);
        load_gl_func("glCreateProgram", glCreateProgram);
        load_gl_func("glAttachShader", glAttachShader);
        load_gl_func("glBindAttribLocation", glBindAttribLocation);
        load_gl_func("glLinkProgram", glLinkProgram);
        load_gl_func("glGetProgramiv", glGetProgramiv);
        load_gl_func("glGetProgramInfoLog", glGetProgramInfoLog);
        load_gl_func("glDeleteProgram", glDeleteProgram);
        load_gl_func("glUseProgram", glUseProgram);
        load_gl_func("glVertexAttribPointer", glVertexAttribPointer);
        load_gl_func("glEnableVertexAttribArray", glEnableVertexAttribArray);
        load_gl_func("glDisableVertexAttribArray", glDisableVertexAttribArray);
        load_gl_func("glGetUniformLocation", glGetUniformLocation);
        load_gl_func("glUniform1i", glUniform1i);
        load_gl_func("glActiveTexture", glActiveTexture_);
        load_gl_func("glUniform4fv", glUniform4fv);
        load_gl_func("glUniformMatrix3fv", glUniformMatrix3fv);
        load_gl_func("glGenBuffers", glGenBuffers);
        load_gl_func("glBindBuffer", glBindBuffer);
        load_gl_func("glBufferData", glBufferData);
        load_gl_func("glBufferSubData", glBufferSubData);
    } catch (std::exception& ex) {
        return ex.what();
    }

    glGenBuffers(1, &gl_default_vbo);
    OM_GL_CHECK();
    glBindBuffer(GL_ARRAY_BUFFER, gl_default_vbo);
    OM_GL_CHECK();
    //uint32_t data_size_in_bytes = 0;
    GLsizeiptr data_size_in_bytes = 0;
    glBufferData(GL_ARRAY_BUFFER, data_size_in_bytes, nullptr, GL_STATIC_DRAW);
    OM_GL_CHECK();
    glBufferSubData(GL_ARRAY_BUFFER, 0, data_size_in_bytes, nullptr);
    OM_GL_CHECK();

    // clang-format off
    shader00 = new shader_gl_es20(
        R"(
                                  attribute vec2 a_position;
                                  void main()
                                  {
                                      gl_Position = vec4(a_position, 0.0, 1.0);
                                  }
                                  )",
        R"(
                                  precision mediump float;
                                  uniform vec4 u_color;
                                  void main()
                                  {
                                      gl_FragColor = u_color;
                                  }
                                  )",
                                  { { 0, "a_position" } });

    shader00->use();
    shader00->set_uniform("u_color", color(1.f, 0.f, 0.f, 1.f));

    shader01 = new shader_gl_es20(
        R"(
                attribute vec2 a_position;
                attribute vec4 a_color;
                varying vec4 v_color;
                void main()
                {
                    v_color = a_color;
                    gl_Position = vec4(a_position, 0.0, 1.0);
                }
                )",
        R"(
                precision mediump float;
                varying vec4 v_color;
                void main()
                {
                    gl_FragColor = v_color;
                }
                )",
        { { 0, "a_position" }, { 1, "a_color" } });

    shader01->use();

    shader02 = new shader_gl_es20(
        R"(
                attribute vec2 a_position;
                attribute vec2 a_tex_coord;
                attribute vec4 a_color;
                varying vec4 v_color;
                varying vec2 v_tex_coord;
                void main()
                {
                    v_tex_coord = a_tex_coord;
                    v_color = a_color;
                    gl_Position = vec4(a_position, 0.0, 1.0);
                }
                )",
        R"(
                precision mediump float;
                varying vec2 v_tex_coord;
                varying vec4 v_color;
                uniform sampler2D s_texture;
                void main()
                {
                    gl_FragColor = texture2D(s_texture, v_tex_coord) * v_color;
                }
                )",
        { { 0, "a_position" }, { 1, "a_color" }, { 2, "a_tex_coord" } });

        shader02->use();

    shader03 = new shader_gl_es20(
        R"(
                uniform mat3 u_matrix;
                attribute vec2 a_position;
                attribute vec2 a_tex_coord;
                attribute vec4 a_color;
                varying vec4 v_color;
                varying vec2 v_tex_coord;
                void main()
                {
                    v_tex_coord = a_tex_coord;                    v_color = a_color;
                    vec3 pos = vec3(a_position, 1.0) * u_matrix;
                    gl_Position = vec4(pos, 1.0);
                }
                )",
        R"(
                precision mediump float;
                varying vec2 v_tex_coord;
                varying vec4 v_color;
                uniform sampler2D s_texture;
                void main()
                {
                    gl_FragColor = texture2D(s_texture, v_tex_coord) * v_color;
                }
                )",
        { { 0, "a_position" }, { 1, "a_color" }, { 2, "a_tex_coord" } });

    shader03->use();

                // clang-format on

                glEnable(GL_BLEND);
                OM_GL_CHECK();
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                OM_GL_CHECK();

                glClearColor(0.f, 0.0, 0.f, 0.0f);
                OM_GL_CHECK();

                glViewport(0, 0, screen_width, screen_height); //
                OM_GL_CHECK();

                //#define SDL_AUDIODRIVER alsa
                // initialize audio
                audio_device_spec.freq = 48000;

                //                { AUDIO_U8, 1 }, { AUDIO_S8, 1 }, { AUDIO_U16LSB, 2 },
                //                    { AUDIO_S16LSB, 2 }, { AUDIO_U16MSB, 2 }, { AUDIO_S16MSB, 2 },
                //                    { AUDIO_S32LSB, 4 }, { AUDIO_S32MSB, 4 }, { AUDIO_F32LSB, 4 },
                //                    { AUDIO_F32MSB, 4 },

                audio_device_spec.format = AUDIO_S16LSB; //AUDIO_S16LSB;
#ifdef __ANDROID__
                audio_device_spec.channels = 2;
#else
                audio_device_spec.channels = 4;
#endif
                audio_device_spec.samples = 2048; // must be power of 2
                audio_device_spec.callback = engine_impl::audio_callback;
                audio_device_spec.userdata = this;

                // printing all and selecting audio DRIVER
                const int num_audio_drivers = SDL_GetNumAudioDrivers();
                const char* selected_audio_driver = nullptr;
                std::vector<std::string> vec_audio_drivers;

                for (int i = 0; i < num_audio_drivers; ++i) {
                    std::string tmp_audio_driver_name = SDL_GetAudioDriver(i);
                    vec_audio_drivers.push_back(tmp_audio_driver_name);
                    std::cout << "- audio_driver #:" << i << " " << tmp_audio_driver_name << '\n';
                }
                std::cout << std::flush;

                // TODO on windows 10 only DirectSound - works for me
                const char* OS = SDL_GetPlatform();
                std::cout << "-- AUDIO platform: " << OS << std::endl << std::flush;

                for (auto x : vec_audio_drivers) {
                    if (0 != SDL_AudioInit(x.c_str())) {
                        std::cout << "-- can't init SDL audio with driver: " << x << std::endl << std::flush;
                        continue;
                    } else {
                        std::cout << "- init SDL audio system with driver: " << x << std::endl << std::flush;
                    }

                    // due to unknown reason selected_audio_device become "" after exit from loop 
                    selected_audio_driver = x.c_str();

                    if (0 == strncmp(OS, "Windows", 5) && x == "winmm") {
                        break;
                    }
                    
                    if ((0 == strncmp(OS, "Linux", 5) || 0 == strncmp(OS, "Android", 7)) && 0 == strncmp(selected_audio_driver, "pulseaudio", 10)) {
                        break;
                    }
                    //else {
                        // any other OS
                        //selected_audio_driver = SDL_GetAudioDriver(1);
                        //break;
                    //}
                }

                std::cout << "- Selected driver: '" << selected_audio_driver << "' for " << OS
                          << std::endl;

                ///////////////////////////////////////////////////////////////

                // printing all and selecting audio DEVICE
                const char* default_audio_device_name = nullptr;

                const int num_audio_devices = SDL_GetNumAudioDevices(SDL_FALSE);

                if (num_audio_devices > 0) {
                    for (int i = 0; i < num_audio_devices; ++i) {
                        std::cout << "-- audio device #" << i << ": "
                                  << SDL_GetAudioDeviceName(i, SDL_FALSE) << '\n';
                    }
                }
                std::cout << std::flush;

#ifdef _WIN32   // on Windows better start from the begining of the list
               for (int i = 0; i < num_audio_devices - 1; ++i) {
#else           // on UNIX better start from the end of the list
                for (int i = num_audio_devices - 1; i >= 0; --i) {
#endif
                    // set the audio device number num_audio_devices..0
                    default_audio_device_name = SDL_GetAudioDeviceName(i, SDL_FALSE);
                    audio_device = SDL_OpenAudioDevice(default_audio_device_name, 0, &audio_device_spec,
                        nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);

                    if (audio_device == 0) {
                        std::cerr << "-- failed open audio device - " << SDL_GetError() << std::endl;
                        //throw std::runtime_error("\naudio failed");
                        continue;
                    }
                    else {
                        std::cout << "- Selected: " << default_audio_device_name
                            << '\n'
                            << "freq: " << audio_device_spec.freq << '\n'
                            << "format: "
                            << get_sound_format_name(audio_device_spec.format) << '\n'
                            << "channels: "
                            << static_cast<uint32_t>(audio_device_spec.channels) << '\n'
                            << "samples: " << audio_device_spec.samples << '\n'
                            << std::flush;

                        // unpause device
                        SDL_PauseAudioDevice(i, SDL_FALSE);
                        break;
                    }
                }

                return "";
                // clang-format on
}

void engine_impl::audio_callback(void* engine_ptr, uint8_t* stream,
    int stream_size)
{
    // no sound default
    std::fill_n(stream, stream_size, '\0');

    engine_impl* e = static_cast<engine_impl*>(engine_ptr);

    for (sound_buffer_impl* snd : e->sounds) {
        if (snd->is_playing) {
            uint32_t rest = snd->length - snd->current_index;
            uint8_t* current_buff = &snd->buffer[snd->current_index];

            if (rest <= static_cast<uint32_t>(stream_size)) {
                // copy rest to buffer
                SDL_MixAudioFormat(stream, current_buff,
                    e->audio_device_spec.format, rest,
                    SDL_MIX_MAXVOLUME);
                snd->current_index += rest;
            } else {
                SDL_MixAudioFormat(
                    stream, current_buff, e->audio_device_spec.format,
                    static_cast<uint32_t>(stream_size), SDL_MIX_MAXVOLUME);
                snd->current_index += static_cast<uint32_t>(stream_size);
            }

            if (snd->current_index == snd->length) {
                if (snd->is_looped) {
                    // start from begining
                    snd->current_index = 0;
                } else {
                    snd->is_playing = false;
                }
            }
        }
    }
}

size_t engine_impl::get_window_width()
{
    return screen_width;
}
size_t engine_impl::get_window_height()
{
    return screen_height;
}

///std::vector<game_state*> states;
void engine_impl::switch_to_state_and_resume(game_state* state)
{
    current_state->pause();
    current_state = state;
    current_state->resume();
}

void engine_impl::change_state(game_state* state)
{
    // cleanup the current state
    if (!states.empty()) {
        states.back()->cleanup();
        states.pop_back();
    }

    // store and init the new state
    states.push_back(state);
    states.back()->init(this);
}

void engine_impl::push_state_and_init(game_state* state)
{
    // store and init the new state
    states.push_back(state);
    states.back()->init(this);
}

void engine_impl::pop_state()
{
    // cleanup the current state
    if (!states.empty()) {

        states.back()->cleanup();
        states.pop_back();
    }

    // resume previous state
    if (!states.empty()) {
        states.back()->resume();
    }
}

bool engine_impl::is_mouse_clicked_in_coord(const float& lx, const float& rx, const float& ly, const float& ry)
{
    float w = screen_width;
    float h = screen_height;
    size_t m_x = mouse_coord_pressed.x;
    size_t m_y = mouse_coord_pressed.y;
    // float scale_x = engine->scale.col0.x; //0.625
    // float scale_y = engine->scale.col1.y; //1
    return ((m_y > ly * h) && (m_y < ry * h) && (m_x > lx * w) && (m_x < rx * w));
}

engine_impl::~engine_impl()
{
    delete shader00;
    delete shader01;
    delete shader02;
    delete shader03;
}

void engine_impl::clear_states()
{
    if (std::size(states)) {
        states.clear();
    }
}

void engine_impl::swap_last_two_states()
{
    if (std::size(states) < 2) {
        std::cerr << "states size < 2. could not swap";
    }

    // cleanup the current state
    if (!states.empty()) {
        states.back()->cleanup();
    }

    // store and resume the previous state
    std::iter_swap(states.rbegin() + 1, states.rbegin());

    states.back()->resume();
}

game_state::~game_state() {}

} // end of namespace grottans

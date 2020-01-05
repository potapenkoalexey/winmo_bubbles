#pragma once

#include <memory>

#include "../../../src/engine/engine.hxx"
#include "../components/block.hxx"

class select_mode_state : public grottans::game_state {
public:
    bool init(grottans::engine* e);
    void cleanup();

    void pause();
    void resume();

    void handle_events();
    void update();
    void draw();

    void handle_mouse_event(grottans::engine* engine, grottans::event& e);

    void sound_turn_on();
    void sound_turn_off();

    static game_state* instance()
    {
        return &m_select_mode_state;
    }

    select_mode_state(select_mode_state const&) = delete;
    select_mode_state& operator=(select_mode_state const&) = delete;

    ~select_mode_state();

protected:
    select_mode_state();

private:
    static select_mode_state m_select_mode_state;

    //write all elements of the screen here
    std::unique_ptr<block> block_back;
    std::unique_ptr<block> block_select;

    std::array<grottans::tri2, 6> vert_buf; // v_buf triangles
    grottans::texture* tex_back_sound_on;
    grottans::texture* tex_back_sound_off;
    grottans::vertex_buffer* v_buf_classic;
    grottans::vertex_buffer* v_buf_extreme;
    std::unique_ptr<grottans::sound_buffer> sound_on;
};

#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./settings.hxx"

class select_mode_state : public grottans::game_state {
public:
    bool init(grottans::engine*);
    void cleanup(grottans::engine*);

    void pause(grottans::engine*);
    void resume(grottans::engine*);

    void handle_events(grottans::engine*);
    void update(grottans::engine*);
    void draw(grottans::engine*);

    static game_state* instance()
    {
        return &m_select_mode_state;
    }

protected:
    select_mode_state()
    {
        block_back = nullptr;
        block_select = nullptr;
        tex_back_sound_on = nullptr;
        tex_back_sound_off = nullptr;
        v_buf_classic = nullptr;
        v_buf_extreme = nullptr;
        sound_on = nullptr;
    }

private:
    static select_mode_state m_select_mode_state;

    //write all elements of the screen here
    std::unique_ptr<block> block_back;
    std::unique_ptr<block> block_select;
    std::array<grottans::tri2, 6> tr; // v_buf triangles
    grottans::texture* tex_back_sound_on;
    grottans::texture* tex_back_sound_off;
    grottans::vertex_buffer* v_buf_classic;
    grottans::vertex_buffer* v_buf_extreme;
    grottans::sound_buffer* sound_on;
};

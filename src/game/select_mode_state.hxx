#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./settings.hxx"

class select_mode_state : public grottans::game_state {
public:
    void init(grottans::engine*);
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
        block_select = nullptr;
        sound_on = nullptr;
        width = 0;
        height = 0;
    }

private:
    static select_mode_state m_select_mode_state;

    //write all elements of the screen here
    std::unique_ptr<block> block_select;
    std::array<grottans::tri2, 2> tr; // v_buf triangles
    grottans::sound_buffer* sound_on;
    size_t width, height;
};

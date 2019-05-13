#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./settings.hxx"

class game_over_state : public grottans::game_state {
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
        return &m_game_over_state;
    }

protected:
    game_over_state()
    {
        block_back = nullptr;
        sound_game_over = nullptr;
        width = 0;
        height = 0;
    }

private:
    static game_over_state m_game_over_state;

    std::unique_ptr<block> block_back;
    std::array<grottans::tri2, 2> tr; // v_buf triangles
    grottans::sound_buffer* sound_game_over;
    size_t width, height;
};

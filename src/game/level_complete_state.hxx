#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./counter.hxx"
#include "./number.hxx"

class level_complete_state : public grottans::game_state {
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
        return &m_level_complete_state;
    }

    size_t level_number;

    std::unique_ptr<block> block_back;

    grottans::texture* tex_even;
    grottans::texture* tex_uneven;

    grottans::sound_buffer* sound_even;
    grottans::sound_buffer* sound_uneven;

    std::array<grottans::tri2, 2> tr; // v_buf triangles

    std::unique_ptr<counter> m_counter;

protected:
    level_complete_state() {}
private:
    static level_complete_state m_level_complete_state;
};

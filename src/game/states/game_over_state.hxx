#pragma once

#include <memory>

#include "../../../src/engine/engine.hxx"
#include "../components/block.hxx"
#include "../components/counter.hxx"

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

    game_over_state(game_over_state const&) = delete;
    game_over_state& operator=(game_over_state const&) = delete;

protected:
    game_over_state();

private:
    static game_over_state m_game_over_state;

    std::unique_ptr<block> block_back;

    std::array<grottans::tri2, 2> vert_buf_tr; // v_buf triangles

    std::unique_ptr<grottans::sound_buffer> sound_game_over;

    std::unique_ptr<counter> counter_final_score;
};

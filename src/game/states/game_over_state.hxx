#pragma once

#include <memory>

#include "../../../src/engine/engine.hxx"
#include "../components/block.hxx"
#include "../components/counter.hxx"

class game_over_state : public grottans::game_state {
public:
    bool init(grottans::engine* e);
    void cleanup();

    void pause();
    void resume();

    void handle_events();
    void update();
    void draw();

    static game_state* instance()
    {
        return &m_game_over_state;
    }

    ~game_over_state();

    game_over_state(game_over_state const&) = delete;
    game_over_state& operator=(game_over_state const&) = delete;

protected:
    game_over_state()
        : block_back{ nullptr }
        , sound_game_over{ nullptr }
        , counter_final_score{ nullptr }
    {
    }

private:
    static game_over_state m_game_over_state;

    std::unique_ptr<block> block_back;

    std::array<grottans::tri2, 2> vert_buf_tr; // v_buf triangles

    std::unique_ptr<grottans::sound_buffer> sound_game_over;

    std::unique_ptr<counter> counter_final_score;
};

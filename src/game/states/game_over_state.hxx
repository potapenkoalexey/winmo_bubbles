#pragma once

#include <memory>

#include "../../../src/engine/engine.hxx"
#include "../components/block.hxx"
#include "../components/counter.hxx"

class game_over_state : public grottans::game_state {
public:
    bool init(grottans::engine* e) override;
    void cleanup() final;

    void pause() override;
    void resume() override;

    void handle_events() override;
    void update() override;
    void draw() override;

    static game_state* instance()
    {
        return &m_game_over_state;
    }

    ~game_over_state() override;
    game_over_state(game_over_state const&) = delete;
    game_over_state& operator=(game_over_state const&) = delete;

protected:
    game_over_state()
        : block_back{ nullptr }
        , sound_game_over{ nullptr }
        , sound_new_max_score{ nullptr }
        , counter_final_score{ nullptr }
    {
    }

private:
    static game_over_state m_game_over_state;

    std::unique_ptr<block> block_back;

    grottans::texture* tex_game_over;
    grottans::texture* tex_game_over_new_max_score;

    std::array<grottans::tri2, 2> vert_buf_tr; // v_buf triangles

    grottans::sound_buffer* sound_game_over;
    grottans::sound_buffer* sound_new_max_score;

    std::unique_ptr<counter> counter_final_score;
};

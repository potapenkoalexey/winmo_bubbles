#pragma once

#include <memory>

#include "engine.hxx"
#include "../components/block.hxx"
#include "../components/counter.hxx"
#include "../components/number.hxx"

class level_complete_state : public grottans::game_state {
public:
    bool init(grottans::engine*) override;
    void cleanup() override;

    void pause() override;
    void resume() override;

    void handle_events() override;
    void update() override;
    void draw() override;

    static game_state* instance()
    {
        return &m_level_complete_state;
    }

    ~level_complete_state() override;
    level_complete_state(level_complete_state const&) = delete;
    level_complete_state& operator=(level_complete_state const&) = delete;

protected:
    level_complete_state()
        : block_back{ nullptr }
        , tex_even{ nullptr }
        , tex_uneven{ nullptr }
        , sound_even{ nullptr }
        , sound_uneven{ nullptr }
        , counter_level{ nullptr }
    {
    }

private:
    static level_complete_state m_level_complete_state;

    size_t level_number;

    std::unique_ptr<block> block_back;

    grottans::texture* tex_even;
    grottans::texture* tex_uneven;

    grottans::sound_buffer* sound_even;
    grottans::sound_buffer* sound_uneven;

    std::array<grottans::tri2, 2> vert_buf_tr; // v_buf triangles

    std::unique_ptr<counter> counter_level;
};

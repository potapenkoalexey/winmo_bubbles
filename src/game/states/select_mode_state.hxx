#pragma once

#include <memory>

#include "../../../src/engine/engine.hxx"
#include "../components/block.hxx"

class select_mode_state : public grottans::game_state {
public:
    bool init(grottans::engine* e) override;
    void cleanup() override;

    void pause() override;
    void resume() override;

    void handle_events() override;
    void update() override;
    void draw() override;

    void handle_mouse_event(grottans::engine* engine, grottans::event& e);

    void sound_turn_on();
    void sound_turn_off();

    static game_state* instance()
    {
        return &m_select_mode_state;
    }

    ~select_mode_state() override;
    select_mode_state(select_mode_state const&) = delete;
    select_mode_state& operator=(select_mode_state const&) = delete;

protected:
    select_mode_state()
        : block_back{ nullptr }
        , block_select{ nullptr }
        , tex_back{ nullptr }
        , v_buf_classic{ nullptr }
        , v_buf_extreme{ nullptr }
        , v_buf_sound{ nullptr }
        , sound_on{ nullptr }
    {
    }

private:
    static select_mode_state m_select_mode_state;

    //write all elements of the screen here
    std::unique_ptr<block> block_back;
    std::unique_ptr<block> block_select;
    std::unique_ptr<block> block_sound;

    std::array<grottans::tri2, 8> vert_buf; // v_buf triangles

    grottans::texture* tex_back;
    grottans::texture* tex_selector;
    grottans::texture* tex_sound_on;
    grottans::texture* tex_sound_off;

    grottans::vertex_buffer* v_buf_classic;
    grottans::vertex_buffer* v_buf_extreme;
    grottans::vertex_buffer* v_buf_sound;

    grottans::sound_buffer* sound_on;
};

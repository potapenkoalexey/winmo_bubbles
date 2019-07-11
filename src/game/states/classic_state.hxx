#pragma once

#include <memory>

#include "../../../src/engine/engine.hxx"
#include "../components/block.hxx"
#include "../components/field.hxx"
#include "../components/progress_desk.hxx"

class classic_state : public grottans::game_state {
public:
    bool init(grottans::engine*);
    void cleanup(grottans::engine*);

    void pause(grottans::engine*);
    void resume(grottans::engine*);

    void handle_events(grottans::engine*);
    void update(grottans::engine*);
    void draw(grottans::engine*);

    bool handle_mouse_event(grottans::engine* engine);
    void handle_start_released_event(grottans::engine* engine);
    void handle_left_released_event(grottans::engine* engine);
    void handle_right_released_event(grottans::engine* engine);
    void handle_up_released_event(grottans::engine* engine);
    void handle_down_released_event(grottans::engine* engine);

    static game_state* instance()
    {
        return &m_classic_state;
    }

    classic_state(classic_state const&) = delete;
    classic_state& operator=(classic_state const&) = delete;

protected:
    classic_state()
    {
        sound_fall = nullptr;
        sound_destroy_big_form = nullptr;
        progress = nullptr;
        m_counter = nullptr;
        game_field = nullptr;
    }

private:
    static classic_state m_classic_state;

    std::unique_ptr<grottans::sound_buffer> sound_fall;
    std::unique_ptr<grottans::sound_buffer> sound_destroy_big_form;

    std::unique_ptr<progress_desk> progress;

    std::unique_ptr<counter> m_counter;

    std::unique_ptr<field> game_field;
};

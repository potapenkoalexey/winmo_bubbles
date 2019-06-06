#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./field.hxx"
#include "./progress_desk.hxx"

class extreme_state : public grottans::game_state {
public:
    bool init(grottans::engine*);
    void cleanup(grottans::engine*);

    void pause(grottans::engine*);
    void resume(grottans::engine*);

    void handle_events(grottans::engine*);
    void update(grottans::engine*);
    void draw(grottans::engine*);

    bool handle_mouse_event(grottans::engine* engine);

    static game_state* instance()
    {
        return &m_extreme_state;
    }

protected:
    extreme_state() {}

private:
    static extreme_state m_extreme_state;

    std::unique_ptr<grottans::sound_buffer> sound_fall;
    std::unique_ptr<grottans::sound_buffer> sound_destroy_big_form;
    std::unique_ptr<grottans::sound_buffer> sound_flip;
    std::unique_ptr<grottans::sound_buffer> sound_cant_flip;

    std::unique_ptr<progress_desk> progress;

    std::unique_ptr<counter> m_counter;

    std::unique_ptr<field> game_field;
};

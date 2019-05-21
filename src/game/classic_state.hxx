#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./field.hxx"
#include "./progress_desk.hxx"
#include "./settings.hxx"

class classic_state : public grottans::game_state {
public:
    bool init(grottans::engine*);
    void cleanup(grottans::engine*);

    void pause(grottans::engine*);
    void resume(grottans::engine*);

    void handle_events(grottans::engine*);
    void update(grottans::engine*);
    void draw(grottans::engine*);

    void handle_mouse_event(grottans::engine* engine);

    static game_state* instance()
    {
        return &m_classic_state;
    }

protected:
    classic_state() {}

private:
    static classic_state m_classic_state;

    std::unique_ptr<progress_desk> progress;

    std::unique_ptr<field> game_field;
};

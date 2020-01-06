#pragma once

#include "./engine.hxx"

namespace grottans {

class engine;

class game_state {
public:
    virtual bool init(grottans::engine* e) = 0;
    virtual void cleanup() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void handle_events() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    virtual ~game_state();

protected:
    game_state() {}

    grottans::engine* engine;
};

} ///end of namespace

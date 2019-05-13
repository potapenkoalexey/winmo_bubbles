#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./settings.hxx"

class classic_state : public grottans::game_state {
public:
    void init(grottans::engine*);
    void cleanup(grottans::engine*);

    void pause(grottans::engine*);
    void resume(grottans::engine*);

    void handle_events(grottans::engine*);
    void update(grottans::engine*);
    void draw(grottans::engine*);

    static game_state* instance()
    {
        return &m_classic_state;
    }

protected:
    classic_state()
    {
        block_classic = nullptr;
        width = 0;
        height = 0;
    }

private:
    static classic_state m_classic_state;

    std::unique_ptr<block> block_classic;
    std::array<grottans::tri2, 2> tr; // v_buf triangles
    size_t width, height;
};

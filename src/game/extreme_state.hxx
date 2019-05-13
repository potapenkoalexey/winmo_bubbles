#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./field.hxx"
#include "./settings.hxx"

class extreme_state : public grottans::game_state {
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
        return &m_extreme_state;
    }

protected:
    extreme_state()
    {
        block_back = nullptr;
        width = 0;
        height = 0;
    }

private:
    static extreme_state m_extreme_state;

    std::unique_ptr<block> block_back;
    std::array<grottans::tri2, 2> tr; // v_buf triangles
    size_t width, height;

    std::unique_ptr<field> game_field;
};

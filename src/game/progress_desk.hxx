#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./counter.hxx"

class progress_desk {
public:
    bool init(grottans::engine* engine);
    void draw(grottans::engine* engine);

private:
    std::unique_ptr<block> block_desk;
    std::unique_ptr<block> block_line;
    std::unique_ptr<counter> emb_counter;

    grottans::texture* tex_desk;
    grottans::texture* tex_line;

    std::array<grottans::tri2, 4> tr; // v_buf triangles
};

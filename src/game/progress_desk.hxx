#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./counter.hxx"

class progress_desk {
public:
    bool init(grottans::engine* engine);
    void draw(grottans::engine* engine);

    void set_line_in_null();
    void set_line_in_full();

    void increase_progress(double);

private:
    std::unique_ptr<block> block_desk;
    std::unique_ptr<block> block_line;
    std::unique_ptr<counter> emb_counter;

    grottans::texture* tex_desk;
    grottans::texture* tex_line;

    /// v_buf triangles, 5-6'th for backup
    std::array<grottans::tri2, 6> tr;
};

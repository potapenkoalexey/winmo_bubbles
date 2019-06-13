#pragma once

#include <memory>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"
#include "./counter.hxx"
#include "./global_variables.hxx"

class progress_desk {
public:
    bool init(grottans::engine*);
    void draw(grottans::engine*);

    void set_line_in_null(grottans::engine*);
    void set_line_in_full(grottans::engine*);

    bool get_level_complete_flag();
    void set_level_complete_flag(bool);

    size_t blocks_to_points(size_t delta);

    void increase_progress(grottans::engine*, size_t points, size_t points_to_level_);
    void update_line_vertex_buffer(grottans::engine*);

    void set_dispayed_number(const size_t&);

private:
    bool level_complete_flag = false;

    std::array<size_t, 25> points_classic;

    std::array<size_t, 30> points_extreme;

    std::array<size_t, 26> levels_classic;

    std::array<size_t, 26> levels_extreme;

    std::unique_ptr<block> block_desk;
    std::unique_ptr<block> block_line;

    std::unique_ptr<counter> counter_combo_points;

    grottans::texture* tex_desk;
    grottans::texture* tex_line;

    /// v_buf triangles, 5-6'th for backup
    std::array<grottans::tri2, 6> tr;
};

#pragma once

#include <memory>

#include "../../engine/engine.hxx"
#include "../global_variables.hxx"
#include "./block.hxx"
#include "./counter.hxx"

class progress_desk {
public:
    bool init();
    void draw();

    void set_line_in_null();
    void set_line_in_full();

    bool get_level_complete_flag() const;
    void set_level_complete_flag(const bool&);

    size_t blocks_to_points(const size_t& delta);

    void increase_progress(const size_t& points, const size_t& level_number);

    int set_progress_line_in_percent(const double& percent);

    int update_progress_line_after_restore();

    void update_line_vertex_buffer();

    void set_dispayed_points(const size_t&);
    void set_displayed_score(const size_t&);

    progress_desk(grottans::engine* e)
        : engine{ e }
        , level_complete_flag{ false }
        , points_classic{}
        , points_extreme{}
        , levels_classic{}
        , levels_extreme{}
        , block_desk{ nullptr }
        , block_line{ nullptr }
        , counter_combo_points{ nullptr }
        , counter_points_to_level{ nullptr }
        , counter_score{ nullptr }
        , tex_desk{ nullptr }
        , tex_line{ nullptr }
    {
    }
    ~progress_desk();

    progress_desk() = delete;
    progress_desk(const progress_desk&) = delete;
    const progress_desk& operator=(const progress_desk&) = delete;
    progress_desk(progress_desk&&) = delete;
    progress_desk& operator=(progress_desk&&) = delete;

private:
    grottans::engine* engine;

    bool level_complete_flag;

    std::array<size_t, 25> points_classic;
    std::array<size_t, 30> points_extreme;
    std::array<size_t, 26> levels_classic;
    std::array<size_t, 26> levels_extreme;

    std::unique_ptr<block> block_desk;
    std::unique_ptr<block> block_line;

    std::unique_ptr<counter> counter_combo_points;
    std::unique_ptr<counter> counter_points_to_level;
    std::unique_ptr<counter> counter_score;

    grottans::texture* tex_desk;
    grottans::texture* tex_line;

    /// v_buf triangles, 5-6'th for backup
    std::array<grottans::tri2, 6> tr;
};

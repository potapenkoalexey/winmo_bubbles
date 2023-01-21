#include <fstream>

#include "../global_variables.hxx"
#include "progress_desk.hxx"

progress_desk::~progress_desk()
{
    delete tex_desk;
    delete tex_line;

    delete block_desk->v_buf;
    delete block_line->v_buf;
}

bool progress_desk::init()
{
#ifndef DEBUG_GAME_OVER_STATE
    points_classic = { 2, 6, 12, 20, 30, 42, 56, 72, 90,
        110, 132, 156, 182, 210, 240, 272, 306, 342,
        382, 424, 468, 514, 562, 750, 999 };
#endif
#ifdef DEBUG_GAME_OVER_STATE
    points_classic = { 2, 2, 2, 2, 2, 2, 2, 2, 2,
                      2, 2, 2, 2, 2, 2, 2, 2, 2,
                      2, 2, 2, 2, 2, 2, 2 };
#endif
    points_extreme = { 3, 8, 15, 24, 35, 48, 63, 80, 99, 120,
        143, 168, 195, 224, 255, 288, 323, 360, 399, 440,
        483, 528, 575, 624, 675, 728, 783, 840, 899, 999 };

    levels_classic = { 0, 200, 225, 250, 275, 300, 325, 350, 375,
        400, 425, 450, 475, 500, 525, 550, 575, 600,
        625, 650, 675, 700, 725, 750, 775, 800 };

    levels_extreme = { 0, 800, 1100, 1400, 1700, 2000, 2300, 2600, 2900,
        3200, 3500, 3800, 4100, 4400, 4700, 5000, 5200, 5400,
        5600, 5800, 6000, 6200, 6400, 6600, 6800, 7000 };

    block_desk = std::make_unique<block>(engine);
    block_line = std::make_unique<block>(engine);

    tex_desk = engine->create_texture("./data/images/gui/progress_desk.png");
    tex_line = engine->create_texture("./data/images/gui/progress_line.png");

    block_desk->texture = tex_desk;
    block_line->texture = tex_line;

    auto text = engine->load_txt_and_filter_comments(
        "./data/vertex_buffers/vert_buffer_for_progress_desk.txt");
    text >> tr[0] >> tr[1] >> tr[2] >> tr[3];

    /// creating vertex buffers
    block_desk->v_buf = engine->create_vertex_buffer(&tr[0], 2);
    block_line->v_buf = engine->create_vertex_buffer(&tr[2], 2);

    /// backuping line coordinates
    tr[4] = tr[2];
    tr[5] = tr[3];

    /// displays points you earned for the previous combination
    counter_combo_points = std::make_unique<counter>(engine);
    counter_combo_points->set_quantity_of_digits(3, counter::sign::sign);
    counter_combo_points->init();
    counter_combo_points->set_vertexes(-0.22f, -0.87f, 0.08f, 0.08f);
    counter_combo_points->set_color({ 1.0f, 0.5f, 0.0f, 1.0f });
    counter_combo_points->set_vertex_buffer();
    counter_combo_points->set_displayed_number(0);
    counter_combo_points->set_hide_zeros(false);

    /// displays points you need to earn to complete level
    counter_points_to_level = std::make_unique<counter>(engine);
    counter_points_to_level->set_quantity_of_digits(4, counter::sign::unsign);
    counter_points_to_level->init();
    counter_points_to_level->set_vertexes(-0.95f, -0.87f, 0.08f, 0.08f);
    counter_points_to_level->set_color({ 1.0f, 1.0f, 1.0f, 1.0f });
    counter_points_to_level->set_vertex_buffer();
    counter_points_to_level->set_displayed_number(0);
    counter_points_to_level->set_hide_zeros(true);

    /// displays game score
    counter_score = std::make_unique<counter>(engine);
    counter_score->set_quantity_of_digits(5, counter::sign::unsign);
    counter_score->init();
    counter_score->set_vertexes(0.55f, -0.87f, 0.08f, 0.08f);
    counter_score->set_color({ 1.0f, 1.0f, 1.0f, 1.0f });
    counter_score->set_vertex_buffer();
    counter_score->set_displayed_number(0);
    counter_score->set_hide_zeros(true);

    return EXIT_SUCCESS;
}

void progress_desk::update_line_vertex_buffer()
{
    /// update vertex_buffer
    engine->destroy_vertex_buffer(block_line->v_buf);
    block_line->v_buf = engine->create_vertex_buffer(&tr[2], 2);
}

void progress_desk::set_displayed_points(const size_t& number)
{
    counter_combo_points->set_displayed_number(number);

    if (g_MODE == MODE::classic){
        counter_points_to_level->set_displayed_number(levels_classic[g_LEVEL]);
    } else if (g_MODE == MODE::extreme){
        counter_points_to_level->set_displayed_number(levels_extreme[g_LEVEL]);
    }
}

void progress_desk::set_displayed_score(const size_t & score)
{
    counter_score->set_displayed_number(score);
}

void progress_desk::set_combo_points(const size_t & score)
{
    counter_combo_points->set_displayed_number(score);
}

void progress_desk::draw()
{
    block_desk->draw();
    block_line->draw();
    counter_combo_points->draw();
    counter_points_to_level->draw();
    counter_score->draw();
}

void progress_desk::set_line_in_null()
{
    tr[2].v[1].pos.x = tr[2].v[0].pos.x;
    tr[2].v[2].pos.x = tr[3].v[2].pos.x;
    tr[3].v[1].pos.x = tr[3].v[2].pos.x;
    update_line_vertex_buffer();
}

void progress_desk::set_line_in_full()
{
    tr[2].v[1].pos.x = tr[4].v[1].pos.x;
    tr[2].v[2].pos.x = tr[4].v[2].pos.x;
    tr[3].v[1].pos.x = tr[5].v[1].pos.x;
    update_line_vertex_buffer();
}

bool progress_desk::get_level_complete_flag() const
{
    return level_complete_flag;
}

void progress_desk::set_level_complete_flag(const bool& var)
{
    level_complete_flag = var;
}

size_t progress_desk::blocks_to_points(const size_t& delta)
{
    if (g_MODE == MODE::classic) {
        if (delta < 2) return 0;

        if (delta < 24)
            return points_classic[delta - 2];
        else
            return points_classic[24];
    }
    if (g_MODE == MODE::extreme) {
        if (delta < 3) return 0;

        if (delta < 29)
            return points_extreme[delta - 3];
        else
            return points_extreme[29];
    }

    return 0;
}

void progress_desk::increase_progress(
    const size_t& points,
    const size_t& level_number)
{
    float points_to_level = 0;

    if (g_MODE == MODE::classic) {
        points_to_level = levels_classic[level_number];
    } else {
        points_to_level = levels_extreme[level_number];
    }

    /// moving counter line on % step 0,0142
    tr[2].v[1].pos.x += (0.01094f * points * 100 / points_to_level);
    tr[2].v[2].pos.x += (0.01094f * points * 100 / points_to_level);
    tr[3].v[1].pos.x += (0.01094f * points * 100 / points_to_level);

    /// if line oveflow
    if (tr[3].v[1].pos.x >= tr[5].v[1].pos.x) {
        set_line_in_full();
    }

    /// updating vertex_buffer
    update_line_vertex_buffer();

    /// if overflow - set maximum
    if (g_SCORE - g_score_in_the_end_of_level >= points_to_level) {
        /// saving score
        g_score_in_the_end_of_level = g_SCORE;

        set_line_in_full();
        /// set the level complite flag
        level_complete_flag = true;
    }
}

int progress_desk::set_progress_line_in_percent(
    const double& percent)
{
    if (percent < 0 || percent > 100) {
        std::cerr << __FUNCTION__ << "-- Invalid incoming argument" << std::endl;
        EXIT_FAILURE;
    }

    /// moving counter line on % step 0,0142
    tr[2].v[1].pos.x += (0.01094f * static_cast<float>(percent));
    tr[2].v[2].pos.x += (0.01094f * static_cast<float>(percent));
    tr[3].v[1].pos.x += (0.01094f * static_cast<float>(percent));

    /// if line oveflow
    if (tr[3].v[1].pos.x >= tr[5].v[1].pos.x) {
        set_line_in_full();
    }

    /// updating vertex_buffer
    update_line_vertex_buffer();

    return EXIT_SUCCESS;
}

int progress_desk::update_progress_line_after_restore()
{
    double percent = 0;

    if (g_MODE == MODE::classic) {
        double points_to_level = levels_classic[g_LEVEL];
        percent = ((double)(g_SCORE - g_score_in_the_end_of_level)) / points_to_level * 100.0;
    } else {
        size_t points_to_level = levels_extreme[g_LEVEL];
        percent = ((double)(g_SCORE - g_score_in_the_end_of_level)) / points_to_level * 100.0;
    }

    if(percent > 100){
        std::cerr << __FUNCTION__ << "-- Invavid data. Percent = " << percent << std::endl;
        return EXIT_FAILURE;
    }

    set_progress_line_in_percent(percent);

    return EXIT_SUCCESS;
}

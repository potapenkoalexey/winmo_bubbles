#include <fstream>

#include "./global_variables.hxx"
#include "./progress_desk.hxx"

bool progress_desk::init(grottans::engine* engine)
{
    points_classic = { 2, 6, 12, 20, 30, 42, 56, 72, 90,
        110, 132, 156, 182, 210, 240, 272, 306, 342,
        382, 424, 468, 514, 562, 750, 999 };

    points_extreme = { 3, 8, 15, 24, 35, 48, 63,
        80, 99, 120, 143, 168, 195, 224, 255, 288, 323, 360, 399,
        440, 483, 528, 575, 624, 675, 728, 783, 840, 899, 999 };

    levels_classic = { 0, 200, 225, 250, 275, 300, 325,
        350, 375, 400, 425, 450, 475, 500, 525, 550, 575, 600, 625, 650, 675,
        700, 725, 750, 775, 800 };

    levels_extreme = { 0, 800, 1100, 1400, 1700, 2000,
        2300, 2600, 2900, 3200, 3500, 3800, 4100, 4400, 4700, 5000, 5200, 5400,
        5600, 5800, 6000, 6200, 6400, 6600, 6800, 7000 };

    block_desk = std::unique_ptr<block>(new block);
    block_line = std::unique_ptr<block>(new block);

    tex_desk = engine->create_texture("./data/images/my/progress_desk.png");
    tex_line = engine->create_texture("./data/images/my/progress_line.png");

    block_desk->texture = tex_desk;
    block_line->texture = tex_line;

    //    std::ifstream file("./data/vertex_buffers/vert_buffer_for_progress_desk.txt");
    //    if (!file) {
    //        std::cerr << "can't load vert_buffer_for_progress_desk.txt\n";
    //        return EXIT_FAILURE;
    //    } else {
    //        /// tr0-1 - for desk
    //        /// tr2-3 - for line
    //        file >> tr[0] >> tr[1] >> tr[2] >> tr[3];
    //        if (!sizeof(tr[1])) {
    //            std::cerr << "can't create vertex buffer\n";
    //            return EXIT_FAILURE;
    //        }
    //    }
    //    file.close();

    auto text = engine->filter_comments("./data/vertex_buffers/vert_buffer_for_progress_desk.txt");
    text >> tr[0] >> tr[1] >> tr[2] >> tr[3];

    ///creating vertex buffers
    block_desk->v_buf = engine->create_vertex_buffer(&tr[0], 2);
    block_line->v_buf = engine->create_vertex_buffer(&tr[2], 2);

    ///backuping line coordinates
    tr[4] = tr[2];
    tr[5] = tr[3];

    ///counter
    counter_combo_points = std::unique_ptr<counter>(new counter);
    counter_combo_points->set_quantity_of_digits(3, counter::sign::sign);
    counter_combo_points->init(engine);
    counter_combo_points->set_vertexes(-0.25f, -0.87f, 0.08f, 0.08f);
    counter_combo_points->set_color({ 1.0f, 0.5f, 0.0f, 1.0f });
    counter_combo_points->set_vertex_buffer(engine);
    counter_combo_points->set_displayed_number(0);
    counter_combo_points->set_hide_zeros(false);

    ///counter
    counter_points_to_level = std::unique_ptr<counter>(new counter);
    counter_points_to_level->set_quantity_of_digits(4, counter::sign::unsign);
    counter_points_to_level->init(engine);
    counter_points_to_level->set_vertexes(-0.95f, -0.87f, 0.08f, 0.08f);
    counter_points_to_level->set_color({ 1.0f, 1.0f, 1.0f, 1.0f });
    counter_points_to_level->set_vertex_buffer(engine);
    counter_points_to_level->set_displayed_number(0);
    counter_points_to_level->set_hide_zeros(true);

    return EXIT_SUCCESS;
}

void progress_desk::update_line_vertex_buffer(grottans::engine* engine)
{
    ///update vertex_buffer
    engine->destroy_vertex_buffer(block_line->v_buf);
    block_line->v_buf = engine->create_vertex_buffer(&tr[2], 2);
}

void progress_desk::set_dispayed_number(const size_t& number)
{
    counter_combo_points->set_displayed_number(number);

    if (g_MODE == MODE::classic) {
        counter_points_to_level->set_displayed_number(levels_classic[g_LEVEL]);
    } else {
        counter_points_to_level->set_displayed_number(levels_extreme[g_LEVEL]);
    }
}

void progress_desk::draw(grottans::engine* engine)
{
    block_desk->draw(engine);
    block_line->draw(engine);
    counter_combo_points->draw(engine);
    counter_points_to_level->draw(engine);
}

void progress_desk::set_line_in_null(grottans::engine* engine)
{
    tr[2].v[1].pos.x = tr[2].v[0].pos.x;
    tr[2].v[2].pos.x = tr[3].v[2].pos.x;
    tr[3].v[1].pos.x = tr[3].v[2].pos.x;
    update_line_vertex_buffer(engine);
}

void progress_desk::set_line_in_full(grottans::engine* engine)
{
    tr[2].v[1].pos.x = tr[4].v[1].pos.x;
    tr[2].v[2].pos.x = tr[4].v[2].pos.x;
    tr[3].v[1].pos.x = tr[5].v[1].pos.x;
    update_line_vertex_buffer(engine);
}

bool progress_desk::get_level_complete_flag()
{
    return level_complete_flag;
}

void progress_desk::set_level_complete_flag(bool var)
{
    level_complete_flag = var;
}

size_t progress_desk::blocks_to_points(size_t delta)
{
    if (g_MODE == MODE::classic) {
        if (delta < 24)
            return points_classic[delta - 2];
        else
            return points_classic[24];
    }
    if (g_MODE == MODE::extreme) {
        if (delta < 29)
            return points_extreme[delta - 3];
        else
            return points_extreme[29];
    }

    return 0;
}

void progress_desk::increase_progress(grottans::engine* engine, size_t points, size_t level_number)
{
    size_t points_to_level = 0;

    if (g_MODE == MODE::classic) {
        points_to_level = levels_classic[level_number];
    } else {
        points_to_level = levels_extreme[level_number];
    }

    /// moving counter line on % step 0,0142
    tr[2].v[1].pos.x += (0.01094f * points * 100 / points_to_level);
    tr[2].v[2].pos.x += (0.01094f * points * 100 / points_to_level);
    tr[3].v[1].pos.x += (0.01094f * points * 100 / points_to_level);

    ///if line oveflow
    if (tr[3].v[1].pos.x >= tr[5].v[1].pos.x) {
        set_line_in_full(engine);
    }

    ///updating vertex_buffer
    update_line_vertex_buffer(engine);

    /// if overflow - set maximum
    if (g_SCORE - g_score_in_the_end_of_level >= points_to_level) {
        ///saving score
        g_score_in_the_end_of_level = g_SCORE;

        set_line_in_full(engine);
        ///set the level complite flag
        level_complete_flag = true;
    }
}

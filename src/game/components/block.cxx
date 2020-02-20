#include <random>

#include "../../../src/engine/random_generator.hxx"
#include "./block.hxx"

static std::mt19937& mt = random_generator::getMt19937();
static std::uniform_real_distribution<double> dist_1_5(10.0, 60.0);
static std::uniform_real_distribution<double> dist_1_11(10.0, 120.0);
static std::uniform_real_distribution<double> dist_1_17(10.0, 175.0);

//block::~block()
//{
//}

float block::get_fps() const
{
    return fps;
}

void block::set_fps(const float& fps_value)
{
    fps = fps_value;
}

///////////////////////////////////////////////////////////////////////////////
void block::get_random_color_from_classic()
{
    int m = static_cast<int>(dist_1_5(mt) / 10);

    if (m == 1) {
        color = palette::yellow;
        return;
    }
    if (m == 2) {
        color = palette::green;
        return;
    }
    if (m == 3) {
        color = palette::red;
        return;
    }
    if (m == 4) {
        color = palette::blue;
        return;
    }
    if (m == 5) {
        color = palette::purple;
    }
}

void block::get_random_color_from_extreme()
{
    int m = static_cast<int>(dist_1_11(mt) / 10);

    if (m == 1 || m == 6) {
        color = palette::yellow;
        return;
    }
    if (m == 2 || m == 7) {
        color = palette::green;
        return;
    }
    if (m == 3 || m == 8) {
        color = palette::red;
        return;
    }
    if (m == 4 || m == 9) {
        color = palette::blue;
        return;
    }
    if (m == 5 || m == 10) {
        color = palette::purple;
        return;
    }
    if (m == 11) {
        color = palette::black;
    }
}

void block::get_random_color_from_extreme_with_bomb()
{
    int m = static_cast<int>(dist_1_17(mt) / 10);

    if (m == 1 || m == 6 || m == 11) {
        color = palette::yellow;
        return;
    }
    if (m == 2 || m == 7 || m == 12) {
        color = palette::green;
        return;
    }
    if (m == 3 || m == 8 || m == 13) {
        color = palette::red;
        return;
    }
    if (m == 4 || m == 9 || m == 14) {
        color = palette::blue;
        return;
    }
    if (m == 5 || m == 10 || m == 15) {
        color = palette::purple;
        return;
    }
    if (m == 16) {
        color = palette::black;
        return;
    }
    if (m == 17) {
        color = palette::bomb;
    }
}

//animation of disappearing
void block::update_uv_coord(
    const std::array<grottans::tri2, 32>& arr_uv_buf,
    const milli_sec& delta_time)
{
    if (state != block_state::disappearing)
        return;

    current_time += delta_time.count() / 1000.f;

    float one_frame_delta = 1.f / g_FPS;

    size_t how_may_frames_from_start = static_cast<size_t>(current_time / one_frame_delta);

    size_t current_frame_index = how_may_frames_from_start % g_FRAME_OF_DISAPPEARING;

    if (current_frame_index > 15) {
        std::cerr << "incorrect frame index!" << std::endl;
        restore_original_parameters(arr_uv_buf);
        return;
    }

    //assign UV-triangles of disappearing texture
    tr_disappear[0] = arr_uv_buf.at(current_frame_index * 2);
    tr_disappear[1] = arr_uv_buf.at(current_frame_index * 2 + 1);

    if (current_frame_index == 15) {
        //restore original block texture
        tr_disappear[0] = arr_uv_buf[0];
        tr_disappear[1] = arr_uv_buf[1];

        current_time = 0.f;
        state = block_state::fixed;
        //set to unvisible for drawing
        visible = false;
        selected = false;
        motion = false;
    }
}

void block::restore_original_parameters(const std::array<grottans::tri2, 32>& arr_uv_buf)
{
    move.delta.x = 0;
    move.delta.y = 0;
    tr_disappear[0] = arr_uv_buf[0];
    tr_disappear[1] = arr_uv_buf[1];
    state = block_state::fixed;
    flip_direction = block::block_direction::non;
    current_time = 0.f;
    fliping_frame_index = 0;
    falling_frame_index = 0;
    shifting_frame_index = 0;
    selected = false;
    motion = false;
}

void block::draw()
{
    engine->render(*v_buf, texture, move * aspect * engine->scale);
}

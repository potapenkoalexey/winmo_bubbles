#include <random>

#include "./block.hxx"

void block::get_random_color_from_5()
{
    static std::vector<palette> colors = {
        palette::red,
        palette::blue,
        palette::green,
        palette::purple,
        palette::yellow
    };

    static std::random_device randomDevice;
    static std::uniform_int_distribution<size_t> distribution(0, colors.size() - 1);

    size_t index = distribution(randomDevice);
    color = colors[index];
}

void block::get_random_color_from_6()
{
    static std::vector<palette> colors = {
        palette::red,
        palette::blue,
        palette::green,
        palette::purple,
        palette::yellow,
        palette::black
    };

    static std::random_device randomDevice;
    static std::uniform_int_distribution<size_t> distribution(0, colors.size() - 1);

    size_t index = distribution(randomDevice);

    color = colors[index];
}

void block::get_random_color_from_7()
{
    static std::vector<palette> colors = {
        palette::red,
        palette::blue,
        palette::green,
        palette::purple,
        palette::yellow,
        palette::black,
        palette::bomb

    };

    static std::random_device randomDevice;
    static std::uniform_int_distribution<size_t> distribution(0, colors.size() - 1);

    size_t index = distribution(randomDevice);

    color = colors[index];
}

void block::update_uv_coord( ///animation of disappearing
    const std::array<grottans::tri2, 32>& arr_uv_buf,
    const milli_sec& delta_time)
{
    if (state != block_state::disappearing) {
        return;
    }

    current_time += delta_time.count() / 1000.f;

    float one_frame_delta = 1.f / fps;

    size_t how_may_frames_from_start = static_cast<size_t>(current_time / one_frame_delta);

    size_t current_frame_index = how_may_frames_from_start % FRAME_OF_DISAPPEARING;

    ///assign UV-triangles of disappearing texture
    tr_disappear[0] = arr_uv_buf.at(current_frame_index * 2);
    tr_disappear[1] = arr_uv_buf.at(current_frame_index * 2 + 1);

    if (current_frame_index == 15) {
        tr_disappear[0] = arr_uv_buf[0];
        tr_disappear[1] = arr_uv_buf[1];
        current_time = 0.f;
        state = block_state::fixed;
    }
}

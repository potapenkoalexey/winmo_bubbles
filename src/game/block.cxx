#include <random>

#include "./block.hxx"

class random_generator {
public:
    static std::mt19937& getMt19937();

private:
    random_generator()
    {
        std::random_device rd;

        if (rd.entropy() != 0) {
            std::seed_seq seed{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
            mMt.seed(seed);
        } else {
            auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            mMt.seed(seed);
        }
    }

    ~random_generator() {}

    static random_generator& instance()
    {
        static random_generator s;
        return s;
    }
    random_generator(random_generator const&) = delete;
    random_generator& operator=(random_generator const&) = delete;

    std::mt19937 mMt;
};

std::mt19937& random_generator::getMt19937()
{
    return random_generator::instance().mMt;
}

static std::mt19937& mt = random_generator::getMt19937();
static std::uniform_real_distribution<double> dist_1_5(10.0, 60.0);
static std::uniform_real_distribution<double> dist_1_11(10.0, 120.0);
static std::uniform_real_distribution<double> dist_1_17(10.0, 175.0);

///////////////////////////////////////////////////////////////////////////////
void block::get_random_color_from_5()
{
    int m = static_cast<int>(dist_1_5(mt) / 10);

    if (m == 1) {
        color = palette::yellow;
    }
    if (m == 2) {
        color = palette::green;
    }
    if (m == 3) {
        color = palette::red;
    }
    if (m == 4) {
        color = palette::blue;
    }
    if (m == 5) {
        color = palette::purple;
    }
}

void block::get_random_color_from_6()
{
    int m = static_cast<int>(dist_1_11(mt) / 10);

    if (m == 1 || m == 6) {
        color = palette::yellow;
    }
    if (m == 2 || m == 7) {
        color = palette::green;
    }
    if (m == 3 || m == 8) {
        color = palette::red;
    }
    if (m == 4 || m == 9) {
        color = palette::blue;
    }
    if (m == 5 || m == 10) {
        color = palette::purple;
    }
    if (m == 11) {
        color = palette::black;
    }
}

void block::get_random_color_from_7()
{
    /*    static std::vector<palette> colors = {
    //        palette::red,
    //        palette::blue,
    //        palette::green,
    //        palette::purple,
    //        palette::yellow,
    //        palette::black,
    //        palette::bomb

    //    };

    //    static std::random_device randomDevice;
    //    static std::uniform_int_distribution<size_t> distribution(0, colors.size() - 1);

    //    size_t index = distribution(randomDevice);
    //    color = colors[index]; */

    int m = static_cast<int>(dist_1_17(mt) / 10);

    if (m == 1 || m == 6 || m == 11) {
        color = palette::yellow;
    }
    if (m == 2 || m == 7 || m == 12) {
        color = palette::green;
    }
    if (m == 3 || m == 8 || m == 13) {
        color = palette::red;
    }
    if (m == 4 || m == 9 || m == 14) {
        color = palette::blue;
    }
    if (m == 5 || m == 10 || m == 15) {
        color = palette::purple;
    }
    if (m == 16) {
        color = palette::black;
    }
    if (m == 17) {
        color = palette::bomb;
    }
}

///animation of disappearing
void block::update_uv_coord(const std::array<grottans::tri2, 32>& arr_uv_buf,
    const milli_sec& delta_time)
{
    if (state != block_state::disappearing)
        return;

    current_time += delta_time.count() / 1000.f;

    float one_frame_delta = 1.f / fps;

    size_t how_may_frames_from_start = static_cast<size_t>(current_time / one_frame_delta);

    size_t current_frame_index = how_may_frames_from_start % g_FRAME_OF_DISAPPEARING;

    if (current_frame_index > 15) {
        std::cerr << "incorrect frame index!" << std::endl;
    }

    ///assign UV-triangles of disappearing texture
    tr_disappear[0] = arr_uv_buf.at(current_frame_index * 2);
    tr_disappear[1] = arr_uv_buf.at(current_frame_index * 2 + 1);

    if (current_frame_index == 15) {
        ///restore original block texture
        tr_disappear[0] = arr_uv_buf[0];
        tr_disappear[1] = arr_uv_buf[1];

        current_time = 0.f;
        state = block_state::fixed;
        ///set to unvisible for drawing
        visible = false;

        //g_DISAPPEARING_END = true;
    }
}

void block::draw(grottans::engine* engine)
{
    engine->render(*v_buf, texture, move * aspect * engine->scale);
}

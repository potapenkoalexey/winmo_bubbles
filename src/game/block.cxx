#include <random>

#include "./block.hxx"
#include "./global_variables.hxx"

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
        palette::black,
        palette::green,
        palette::purple,
        palette::yellow
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
        palette::bomb,
        palette::black,
        palette::green,
        palette::purple,
        palette::yellow
    };

    static std::random_device randomDevice;
    static std::uniform_int_distribution<size_t> distribution(0, colors.size() - 1);

    size_t index = distribution(randomDevice);

    color = colors[index];
}

void block::set_uv_coord(
    grottans::engine& e,
    const std::array<grottans::tri2, 32>& arr_v_buf,
    const float& delta_time)
{
    if (state != state::disappearing) {
        std::cerr << "try to disappearing fixed block";
        return;
    }

    current_time += delta_time;

    float one_frame_delta = 1.f / fps;

    size_t how_may_frames_from_start = static_cast<size_t>(current_time / one_frame_delta);

    size_t current_frame_index = how_may_frames_from_start % FRAME_OF_DISAPPEARING;

    // ПРИСВОИТЬ НУЖНЫЕ ТРЕУГОЛЬНИКИ ДЛЯ ИСЧЕЗАНИЯ ИЗ МАССИВА 32 треуголов (есть в филде)

    //так поле рисует треугольники
    //    v_buf_tmp[0] = v_buf_grid[(i * 10 + j) * 2] + v_buf_disappear[30];
    //    v_buf_tmp[1] = v_buf_grid[(i * 10 + j) * 2 + 1] + v_buf_disappear[31];
    //    gems[i][j]->v_buf = engine->create_vertex_buffer(&v_buf_tmp[0], 2);
    //    engine->render(*gems[i][j]->v_buf, gems[i][j]->texture, gems[i][j]->aspect * scale);
    //    engine->destroy_vertex_buffer(gems[i][j]->v_buf);
}

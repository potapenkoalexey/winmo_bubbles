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

void block::set_mouse_rectangle(size_t x0_, size_t x1_, size_t y0_, size_t y1_)
{
    x0 = x0_;
    x1 = x1_;
    y0 = y0_;
    y1 = y1_;
}

bool block::is_mouse_pressed(size_t x, size_t y)
{
    return (x >= x0 && x < x1 && y >= y0 && y < y1) ? true : false;
}

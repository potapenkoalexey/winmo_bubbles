#pragma once

#include <memory>
#include <vector>

#include "../../src/engine/engine.hxx"
#include "./number.hxx"

class counter {
public:
    bool init(grottans::engine*);
    void update(size_t);
    void draw(grottans::engine*);

    void set_displayed_number(const size_t& number);
    bool is_overflow(const size_t& number);
    void set_position_and_size(float x, float y, float dx, float dy);
    void set_number_of_digit(const size_t& number);

    void increase_number_smoothly();

    void set_number(size_t);
    size_t get_number();

private:
    size_t quantity_of_digits;
    size_t number_on_screen;

    std::vector<grottans::vertex_buffer*> vertex_buffers;
    std::vector<std::unique_ptr<number> > digits;
};

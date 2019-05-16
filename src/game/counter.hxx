#pragma once

#include <vector>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"

class counter {
public:
    void set_displayed_number(size_t number);
    void set_position_and_size(float x, float y, float dx, float dy);
    void set_number_of_digit(size_t number);

private:
    size_t number_of_digit;
    std::vector<grottans::texture*> font;
    std::vector<block> digits;
};

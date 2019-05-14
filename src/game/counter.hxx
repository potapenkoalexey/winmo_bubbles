#pragma once

#include <vector>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"

class counter {
public:
    void set_number_of_digit(size_t);
    void set_number(size_t);

private:
    size_t number_of_digit;
    std::vector<block> digits;
};

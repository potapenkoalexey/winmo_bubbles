#pragma once

#include <vector>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"

struct counter {
    size_t number_of_digit;
    std::vector<block> digits;
};

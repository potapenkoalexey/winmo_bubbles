#pragma once

#include <vector>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"

class counter {
private:
    size_t number_of_digit;
    std::vector<block> digits;
};

#pragma once

#include <memory>

#include "./block.hxx"
#include "./counter.hxx"

class progress_desk {
public:
    bool init();
    void draw();

private:
    std::unique_ptr<block> desk;
    std::unique_ptr<block> line;
    std::unique_ptr<counter> emb_counter;
};

#pragma once

#include "../../src/engine/engine.hxx"

struct block {
    grottans::vec2 position{};
    grottans::palette color = grottans::palette::non;
    bool selected = false;
    bool visible = true;
    bool motion = false;
    grottans::mat2x3 move{};
    grottans::mat2x3 aspect = grottans::mat2x3::scale(1.f, 1.f);
    grottans::texture* texture = nullptr;
    grottans::vertex_buffer* v_buf = nullptr;
};

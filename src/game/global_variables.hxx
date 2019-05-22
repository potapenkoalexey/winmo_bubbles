#pragma once

#include <chrono>
#include <cstddef>

using milli_sec = std::chrono::milliseconds;

enum class MODE {
    non,
    extreme,
    classic
};

extern MODE g_MODE;

extern size_t g_LEVEL;

extern bool g_SOUND;

extern size_t g_SCORE;

extern size_t g_FIELD_WIDTH;

extern size_t g_FIELD_HEIGHT;

extern size_t FRAME_OF_DISAPPEARING;

extern milli_sec frame_delta;
extern std::chrono::duration<float> frame;

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

extern bool g_SOUND;

extern size_t g_LEVEL;

extern size_t g_POINTS;

extern size_t g_SCORE;

extern size_t g_FIELD_WIDTH;

extern size_t g_FIELD_HEIGHT;

//depends on numbers of sprites in textures of the blocks
extern size_t FRAME_OF_DISAPPEARING;

extern milli_sec frame_delta;

//offset selector movement
extern float OFFSET;

extern float FPS;

extern float FPS_falling_factor;

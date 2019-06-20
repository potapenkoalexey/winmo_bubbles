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

extern size_t g_score_in_the_end_of_level;

extern size_t g_FIELD_WIDTH;

extern size_t g_FIELD_HEIGHT;

//depends on numbers of sprites in textures of the blocks
extern size_t g_FRAME_OF_DISAPPEARING;

extern size_t g_FRAME_OF_FALLING;

extern size_t g_FRAME_OF_SHIFTING;

extern size_t g_FRAME_OF_FLIPING;

extern milli_sec g_frame_delta;

//offset selector movement
extern float g_OFFSET;

extern float g_offset_in_screen_coord;

extern float g_FPS;

extern float g_FPS_FALLING;

extern float g_FPS_SHIFTING;

extern float g_FPS_FLIPING;

#pragma once

#include <chrono>
#include <cstddef>

#define GL_CHECK_FOR_DEBUG

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
extern const size_t g_FIELD_WIDTH;
extern const size_t g_FIELD_HEIGHT;
//depends on numbers of sprites in textures of the blocks
extern const size_t g_FRAME_OF_DISAPPEARING;
extern const size_t g_FRAME_OF_FALLING;
extern const size_t g_FRAME_OF_SHIFTING;
extern const size_t g_FRAME_OF_FLIPING;
extern milli_sec g_frame_delta;
//offset selector movement
extern const float g_OFFSET;
extern const float g_offset_in_screen_coord;
extern const float g_FPS;
extern const float g_FPS_FALLING;
extern const float g_FPS_SHIFTING;
extern const float g_FPS_FLIPING;

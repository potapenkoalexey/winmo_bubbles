#pragma once

#include <chrono>
#include <cstddef>

#include "../engine/engine.hxx"

using milli_sec = std::chrono::milliseconds;

///////////////////////////////////////////////////////////////////////////////
class settings {
public:
    void initialize(grottans::engine* engine, const std::string_view& file);

    ~settings();

    using milli_sec = std::chrono::milliseconds;

    enum class MODE {
        non,
        extreme,
        classic
    };

    MODE g_MODE;
    bool g_SOUND;
    size_t g_LEVEL;
    size_t g_POINTS;
    size_t g_SCORE;
    size_t g_score_in_the_end_of_level;
    size_t g_FIELD_WIDTH;
    size_t g_FIELD_HEIGHT;
    //depends on numbers of sprites in textures of the blocks
    size_t g_FRAME_OF_DISAPPEARING;
    size_t g_FRAME_OF_FALLING;
    size_t g_FRAME_OF_SHIFTING;
    size_t g_FRAME_OF_FLIPING;
    milli_sec g_frame_delta;
    //offset selector movement
    float g_OFFSET;
    float g_offset_in_screen_coord;
    float g_FPS;
    float g_FPS_FALLING;
    float g_FPS_SHIFTING;
    float g_FPS_FLIPING;

protected:
    settings() {}
};

#pragma once

#include <chrono>
#include <cstddef>

#define GL_CHECK_FOR_DEBUG

using milli_sec = std::chrono::milliseconds;

enum class MODE {
    non,
    classic,
    extreme
};

/*! @brief Offset between block in field. Should be equal 1.*/
extern const float g_OFFSET;

/*! @brief Offset. Should be equal 2/11.*/
extern const float g_offset_in_screen_coord;

/*! @brief Startup mode. Should be ::classic or ::extreme*/
extern MODE g_MODE;

/*! @brief Debug option.*/
extern bool g_SOUND;

/*! @brief this variable is TRUE in case when player selects loading previous game*/
extern bool g_LOAD_SAVED_STATE;

/*! @brief Startup level number. Should be equal 1.*/
extern size_t g_LEVEL;

extern size_t g_POINTS;
extern size_t g_SCORE;
extern size_t g_SCORE_MAX_CLASSIC;
extern size_t g_SCORE_MAX_EXTREME;

/*! @brief updates in the end of the each level.*/
extern size_t g_score_in_the_end_of_level;

/*! @brief Number of block on the field.*/
extern const size_t g_FIELD_WIDTH;
extern const size_t g_FIELD_HEIGHT;

//depends on numbers of sprites in textures of the blocks
/*! @brief Should be equal number of sprites on block texture.*/
extern const size_t g_FRAME_OF_DISAPPEARING;

/*! @brief Parameter that mean how much intermediate position are between rows.*/
extern const size_t g_FRAME_OF_FALLING;

/*! @brief Parameter that mean how much intermediate position are between columns.*/
extern const size_t g_FRAME_OF_SHIFTING;

/*! @brief Parameter that mean how much intermediate positions are in fliping mode.*/
extern const size_t g_FRAME_OF_FLIPING;

/*! @brief Global timestamp between frames.*/
extern milli_sec g_frame_delta;

//offset selector movement
/*! @brief Global FPS. Should be equal 60.*/
extern const float g_FPS;

/*! @brief FPS in falling mode. Should be equal 140.*/
extern const float g_FPS_FALLING;

/*! @brief FPS in shifting mode. Should be equal 120.*/
extern const float g_FPS_SHIFTING;

/*! @brief FPS in fliping mode. Should be equal 60.*/
extern const float g_FPS_FLIPING;

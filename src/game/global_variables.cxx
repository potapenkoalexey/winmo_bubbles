#include "global_variables.hxx"

/*! @brief Offset between block in field. Should be equal 1.*/
const float g_OFFSET = 1.f;

/*! @brief Offset. Should be equal 2/11.*/
const float g_offset_in_screen_coord = 0.1818f;

/*! @brief Startup mode. Should be ::classic or ::extreme*/
MODE g_MODE = MODE::classic;

/*! @brief Debug option.*/
bool g_SOUND = true;

/*! @brief this variable is TRUE in case when player selects loading previous game*/
bool g_LOAD_SAVED_STATE = false;

/*! @brief Startup level number. Should be equal 1.*/
size_t g_LEVEL = 1;

size_t g_POINTS = 0;
size_t g_SCORE = 0;

/*! @brief updates in the end of the each level.*/
size_t g_score_in_the_end_of_level = 0;

/*! @brief Number of block on the field.*/
const size_t g_FIELD_WIDTH = 10;
const size_t g_FIELD_HEIGHT = 10;

/*! @brief Should be equal number of sprites on block texture.*/
const size_t g_FRAME_OF_DISAPPEARING = 16;

/*! @brief Parameter that mean how much intermediate position are between rows.*/
const size_t g_FRAME_OF_FALLING = 8;

/*! @brief Parameter that mean how much intermediate position are between columns.*/
const size_t g_FRAME_OF_SHIFTING = 8;

/*! @brief Parameter that mean how much intermediate positions are in fliping mode.*/
const size_t g_FRAME_OF_FLIPING = 6;

/*! @brief Global timestamp between frames.*/
milli_sec g_frame_delta;

/*! @brief Global FPS. Should be equal 60.*/
const float g_FPS = 60.f;

/*! @brief FPS in falling mode. Should be equal 140.*/
const float g_FPS_FALLING = 140.f;

/*! @brief FPS in shifting mode. Should be equal 120.*/
const float g_FPS_SHIFTING = 120.f;

/*! @brief FPS in fliping mode. Should be equal 60.*/
const float g_FPS_FLIPING = 60.f;

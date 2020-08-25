#include "global_variables.hxx"

const float g_OFFSET = 1.f;

const float g_offset_in_screen_coord = 0.1818f;

MODE g_MODE = MODE::classic;

bool g_SOUND = true;

bool g_LOAD_SAVED_STATE = false;

size_t g_LEVEL = 1;

size_t g_POINTS = 0;
size_t g_SCORE = 0;
size_t g_SCORE_MAX_CLASSIC = 0;
size_t g_SCORE_MAX_EXTREME = 0;

size_t g_score_in_the_end_of_level = 0;

const size_t g_FIELD_WIDTH = 10;
const size_t g_FIELD_HEIGHT = 10;

const size_t g_FRAME_OF_DISAPPEARING = 16;

const size_t g_FRAME_OF_FALLING = 8;

const size_t g_FRAME_OF_SHIFTING = 8;

const size_t g_FRAME_OF_FLIPING = 6;

milli_sec g_frame_delta;

const float g_FPS = 60.f;

const float g_FPS_FALLING = 140.f;

const float g_FPS_SHIFTING = 120.f;

const float g_FPS_FLIPING = 60.f;

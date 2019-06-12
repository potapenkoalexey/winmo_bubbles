#include "global_variables.hxx"

MODE g_MODE = MODE::classic;

bool g_SOUND = true;

size_t g_LEVEL = 1;

size_t g_POINTS = 0;

size_t g_SCORE = 0;

size_t g_score_in_the_end_of_level = 0;

size_t g_FIELD_WIDTH = 10;

size_t g_FIELD_HEIGHT = 10;

size_t g_FRAME_OF_DISAPPEARING = 16;

size_t g_FRAME_OF_FALLING = 16;

size_t g_FRAME_OF_FLIPING = 6;

milli_sec g_frame_delta;

float g_OFFSET = 1.f;

///original 0.18f, in range 0.19...0.20 take a rebound of the blocks
float g_offset_in_screen_coord = 0.18f;

float g_FPS = 60.f;

float g_FPS_falling_factor = 4.f;

float g_FPS_shifting_factor = 4.f;

float g_FPS_fliping_factor = 1.f;

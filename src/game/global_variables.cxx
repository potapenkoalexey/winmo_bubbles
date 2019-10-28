#include "global_variables.hxx"

float g_OFFSET = 1.f;
float g_offset_in_screen_coord = 0.18f;
MODE g_MODE = MODE::classic;
bool g_SOUND = true;
size_t g_LEVEL = 1;
size_t g_POINTS = 0;
size_t g_SCORE = 0;
size_t g_score_in_the_end_of_level = 0;
size_t g_FIELD_WIDTH = 10;
size_t g_FIELD_HEIGHT = 10;
size_t g_FRAME_OF_DISAPPEARING = 16; // == number of sprites on block texture
size_t g_FRAME_OF_FALLING = 8;
size_t g_FRAME_OF_SHIFTING = 8;
size_t g_FRAME_OF_FLIPING = 6;
milli_sec g_frame_delta;
float g_FPS = 60.f;
float g_FPS_FALLING = 140.f;
float g_FPS_SHIFTING = 120.f;
float g_FPS_FLIPING = 60.f;

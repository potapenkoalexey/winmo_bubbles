#include "global_variables.hxx"

MODE g_MODE = MODE::classic;

bool g_SOUND = true;

size_t g_LEVEL = 1;

size_t g_POINTS = 0;

size_t g_SCORE = 0;

size_t g_FIELD_WIDTH = 10;

size_t g_FIELD_HEIGHT = 10;

size_t g_FRAME_OF_DISAPPEARING = 16;

milli_sec g_frame_delta;

float g_OFFSET = 1.f;

float g_FPS = 60.f;

float g_FPS_falling_factor = 4.f;

float g_FPS_shifting_factor = 4.f;

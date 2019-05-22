#include "global_variables.hxx"

MODE g_MODE = MODE::classic;

size_t g_LEVEL = 1;

bool g_SOUND = true;

size_t g_SCORE = 0;

size_t g_FIELD_WIDTH = 10;

size_t g_FIELD_HEIGHT = 10;

size_t FRAME_OF_DISAPPEARING = 16;

milli_sec frame_delta;

std::chrono::duration<float> frame;

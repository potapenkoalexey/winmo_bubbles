#include "global_variables.hxx"

MODE g_MODE = MODE::classic;

bool g_SOUND = true;

//bool g_DISAPPEARING_END = false;

size_t g_LEVEL = 1;

size_t g_POINTS = 0;

size_t g_SCORE = 0;

size_t g_FIELD_WIDTH = 10;

size_t g_FIELD_HEIGHT = 10;

size_t FRAME_OF_DISAPPEARING = 16;

milli_sec frame_delta;

float OFFSET = 1.f;

float FPS = 60.f;

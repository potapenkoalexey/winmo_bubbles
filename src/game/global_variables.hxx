#pragma once

#include <cstddef>

enum class MODE {
    non,
    extreme,
    classic
};

extern MODE g_MODE;

extern size_t g_LEVEL;

extern bool g_SOUND;

extern size_t g_SCORE;

extern size_t g_FIELD_WIDTH;

extern size_t g_FIELD_HEIGHT;

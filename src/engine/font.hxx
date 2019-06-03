#pragma once

#include "SDL2/SDL_ttf.h"

namespace grottans {
class font {
public:
    font(const char* filename, int size);
    ~font();

    TTF_Font* get_ttf() const;

private:
    TTF_Font* m_font;
};
}

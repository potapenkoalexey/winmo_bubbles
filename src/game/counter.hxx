#pragma once

#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include <SDL2/SDL_ttf.h>

#include "../../src/engine/engine.hxx"
#include "./block.hxx"

class counter {
public:
    void set_displayed_number(size_t);
    void set_position_and_size(float x, float y, float dx, float dy);
    void set_number_of_digit(size_t);

    void set_number(size_t);
    size_t get_number();

    bool init(grottans::engine*);
    void update(size_t);
    void draw(grottans::engine*);

private:
    size_t number_of_digit;
    size_t number;
    std::vector<grottans::texture*> font;
    std::vector<block> digits;

    TTF_Font* ttf_font;
};

class digit {
public:
    void set_number(size_t);
    size_t get_number();

    bool init();
    void draw(grottans::engine*);

private:
    size_t number = 0;
};

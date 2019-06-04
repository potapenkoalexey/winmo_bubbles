#pragma once

#include <string>

#include "./font.hxx"

namespace grottans {
class text_item {
public:
    text_item(const char* text, TTF_Font* font, SDL_Renderer* renderer);
    ~text_item();

    void set_text(const char* text);
    SDL_Texture* get_texture() const;

    int get_width() const;
    int get_height() const;

private:
    SDL_Texture* m_texture;
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;
    std::string m_text;
    int m_width;
    int m_height;
};
}

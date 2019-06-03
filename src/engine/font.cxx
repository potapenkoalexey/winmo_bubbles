#include <stdexcept>

#include "./font.hxx"

namespace grottans {
font::font(const char* filename, int size)
    : m_font(TTF_OpenFont(filename, size))
{
    if (!m_font) {
        throw std::runtime_error(SDL_GetError());
    }
}

font::~font()
{
    if (m_font) {
        TTF_CloseFont(m_font);
    }
}

TTF_Font* font::get_ttf() const
{
    return m_font;
}
}

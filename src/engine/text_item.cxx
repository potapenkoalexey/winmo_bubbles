#include "text_item.hxx"

namespace {
SDL_Texture* generate_texture(const char* text, TTF_Font* font, SDL_Renderer* renderer)
{
    const SDL_Color color = { 225, 225, 225, 255 };
    const auto surface = TTF_RenderText_Blended(font, text, color);
    const auto texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
}

namespace grottans {
text_item::text_item(const char* text, TTF_Font* font, SDL_Renderer* renderer)
    : m_texture(nullptr)
    , m_renderer(renderer)
    , m_font(font)
    , m_width(-1)
    , m_height(-1)
{
    set_text(text);
}

text_item::~text_item()
{
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

void text_item::set_text(const char* text)
{
    if (m_text != text) {
        m_text = text;

        if (m_texture) {
            SDL_DestroyTexture(m_texture);
        }

        m_texture = generate_texture(text, m_font, m_renderer);
        SDL_QueryTexture(m_texture, nullptr, nullptr, &m_width, &m_height);
    }
}

SDL_Texture* text_item::get_texture() const
{
    return m_texture;
}

int text_item::get_width() const
{
    return m_width;
}

int text_item::get_height() const
{
    return m_height;
}
}

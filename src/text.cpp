#include "text.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <stdexcept>

Text::Text(SDL_Renderer *renderer, TTF_Font *font)
    : m_renderer(renderer), m_font(font), m_texture(nullptr),
      m_color{255, 255, 255, 255}, m_width(0), m_height(0) {
  if (!m_renderer || !m_font) {
    throw std::runtime_error("Invalid renderer or font");
  }
}

Text::~Text() { destroyTexture(); }

void Text::setText(const std::string& text, SDL_Color color){
    if (m_text == text &&
        m_color.r == color.r &&
        m_color.g == color.g &&
        m_color.b == color.b &&
        m_color.a == color.a) {
        return; // No change needed
    }

    m_text = text;
    m_color = color;
    createTexture();
}
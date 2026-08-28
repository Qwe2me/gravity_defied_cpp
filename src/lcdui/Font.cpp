#include "Font.h"

#include <stdexcept>
#include <fstream>
#include <vector>

SDL_RWops* Font::ttfRwOps = nullptr;  // ← ЭТО СТРОКА ДОБАВЛЕНА!

Font::Font(FontStyle style, FontSize pointSize)
{
    if (!ttfRwOps) {
        std::string fontPaths[] = {
            "assets/FontSansSerif.ttf",
            "FontSansSerif.ttf"
        };

        SDL_RWops* raw = nullptr;
        for (const auto& path : fontPaths) {
            raw = SDL_RWFromFile(path.c_str(), "rb");
            if (raw) {
                break;
            }
        }

        if (!raw) {
            throw std::runtime_error("Failed to load font: FontSansSerif.ttf");
        }

        ttfRwOps = raw;
    }

    int realSize = getRealFontSize(pointSize);
    TTF_Font* font = TTF_OpenFontRW(ttfRwOps, SDL_TRUE, realSize);
    if (!font) {
        throw std::runtime_error(TTF_GetError());
    }
    TTF_SetFontStyle(font, style);
    this->ttfFont = font;
    this->height = realSize;
}

Font::~Font()
{
    TTF_CloseFont(ttfFont);
}

int Font::getBaselinePosition() const
{
    return height;
}

int Font::getHeight() const
{
    return height;
}

TTF_Font* Font::getTtfFont() const
{
    return ttfFont;
}

int Font::charWidth(char c)
{
    return stringWidth(std::string(1, c));
}

int Font::stringWidth(const std::string& s)
{
    int width, height;
    if (TTF_SizeText(ttfFont, s.c_str(), &width, &height) == -1) {
        throw std::runtime_error(TTF_GetError());
    }
    return width;
}

int Font::substringWidth(const std::string& string, int offset, int len)
{
    return stringWidth(string.substr(offset, len));
}

int Font::getRealFontSize(FontSize size)
{
    switch (size) {
    case SIZE_LARGE:
        return 32;
    case SIZE_MEDIUM:
        return 16;
    case SIZE_SMALL:
        return 12;
    default:
        return 16;
    }
}
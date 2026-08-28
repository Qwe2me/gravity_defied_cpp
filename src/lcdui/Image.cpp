#include "Image.h"

#include <stdexcept>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

// Удаляем CMRC_DECLARE(assets);
// Удаляем #include <cmrc/cmrc.hpp>

Image::Image(int width, int height)
{
    // В SDL 1.2 используем SDL_CreateRGBSurface с масками для 16-bit
    SDL_Surface* surf = SDL_CreateRGBSurface(
        SDL_SWSURFACE,
        width, height,
        16,
        0xF800, 0x07E0, 0x001F, 0
    );
    if (!surf) {
        throw std::runtime_error(SDL_GetError());
    }
    this->surface = surf;
}

Image::Image(const std::string& path)
{
    // Загружаем изображение из файла на SD-карте
    std::string fullPath = "assets/" + path;
    SDL_Surface* surf = IMG_Load(fullPath.c_str());
    
    if (!surf) {
        // Пробуем без папки assets
        surf = IMG_Load(path.c_str());
    }
    
    if (!surf) {
        throw std::runtime_error("Failed to load image: " + path);
    }
    
    // Конвертируем в 16-bit формат (как в Image(int, int))
    SDL_PixelFormat fmt;
    fmt.BitsPerPixel = 16;
    fmt.BytesPerPixel = 2;
    fmt.Rmask = 0xF800;
    fmt.Gmask = 0x07E0;
    fmt.Bmask = 0x001F;
    fmt.Rshift = 11;
    fmt.Gshift = 5;
    fmt.Bshift = 0;
    fmt.Rloss = 3;
    fmt.Gloss = 2;
    fmt.Bloss = 3;
    fmt.colorkey = 0;
    fmt.alpha = 0;
    
    SDL_Surface* surf_conv = SDL_ConvertSurface(surf, &fmt, SDL_SWSURFACE);
    SDL_FreeSurface(surf);
    
    if (!surf_conv) {
        throw std::runtime_error("Failed to convert image: " + path);
    }
    
    this->surface = surf_conv;
}

Image::~Image()
{
    if (this->surface) {
        SDL_FreeSurface(this->surface);
    }
}

int Image::getWidth() const
{
    return this->surface ? this->surface->w : 0;
}

int Image::getHeight() const
{
    return this->surface ? this->surface->h : 0;
}

SDL_Surface* Image::getSurface() const
{
    return this->surface;
}

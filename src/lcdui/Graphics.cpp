#include "Graphics.h"
#include <memory>
#include <SDL_ttf.h>

Graphics::Graphics(SDL_Surface* surface)
{
    this->surface = surface;
    this->currentColor = { 0, 0, 0 };
    this->font = nullptr;
}

void Graphics::drawString(const std::string& s, int x, int y, int anchor)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font->getTtfFont(), s.c_str(), currentColor);
    if (!surfaceMessage) {
        throw std::runtime_error(TTF_GetError());
    }

    int width = surfaceMessage->w;
    int height = surfaceMessage->h;

    x = getAnchorX(x, width, anchor);
    y = getAnchorY(y, height, anchor);
    SDL_Rect dstRect = { (Sint16)x, (Sint16)y, (Uint16)width, (Uint16)height };

    SDL_BlitSurface(surfaceMessage, nullptr, this->surface, &dstRect);
    SDL_FreeSurface(surfaceMessage);
}

void Graphics::setColor(int r, int g, int b)
{
    currentColor.r = r;
    currentColor.g = g;
    currentColor.b = b;
}

void Graphics::setFont(std::shared_ptr<Font> font)
{
    this->font = font;
}

std::shared_ptr<Font> Graphics::getFont() const
{
    return font;
}

void Graphics::setClip(int x, int y, int w, int h)
{
    SDL_Rect clipRect = { (Sint16)x, (Sint16)y, (Uint16)w, (Uint16)h };
    SDL_SetClipRect(surface, &clipRect);
}

void Graphics::drawChar(char c, int x, int y, int anchor)
{
    drawString(std::string(1, c), x, y, anchor);
}

void Graphics::fillRect(int x, int y, int w, int h)
{
    SDL_Rect rect = { (Sint16)x, (Sint16)y, (Uint16)w, (Uint16)h };
    Uint32 color = SDL_MapRGB(surface->format, currentColor.r, currentColor.g, currentColor.b);
    SDL_FillRect(surface, &rect, color);
}

void Graphics::drawArc(int x, int y, int width, int heigth, int startAngle, int arcAngle)
{
    int xradius = width / 2, yradius = heigth / 2;
    x += xradius;
    y += yradius;
    if (xradius == 0 && yradius == 0) {
        return;
    }

    for (int angle = startAngle; angle < startAngle + arcAngle; angle++) {
        drawLine(x + int(xradius * cos(angle * PI_CONV)),
            y - int(yradius * sin(angle * PI_CONV)),
            x + int(xradius * cos((angle + 1) * PI_CONV)),
            y - int(yradius * sin((angle + 1) * PI_CONV)));
    }
}

int to_360(int ang)
{
    if (ang >= 0 && ang <= 360) {
        return ang;
    }
    if (ang < 0) {
        ang += 360;
    }
    return ang;
}

void Graphics::fillArc(int x, int y, int w, int h, int startAngle, int arcAngle)
{
    int endAngle = startAngle + arcAngle;
    double a = w / 2.0, b = h / 2.0;
    double e = sqrt(1.0 - (b * b) / (a * a));
    for (int _y = y - b; _y < y + b; _y++) {
        for (int _x = x - a; _x < x + a; _x++) {
            double ang = atan2(-(_y - y), _x - x);
            double rad = b * b / (1 - e * e * cos(ang) * cos(ang));
            double dist = ((_x - x) * (_x - x) + (_y - y) * (_y - y));

            int ang2 = to_360(ang / PI_CONV);

            if (ang2 >= to_360(startAngle)
                && ang2 <= to_360(endAngle)
                && dist <= rad) {
                _putpixel(_x, _y);
            }

            if (endAngle > 360) {
                if (ang2 < endAngle % 360 && dist <= rad) {
                    _putpixel(_x, _y);
                }
            }
        }
    }
}

void Graphics::_putpixel(int x, int y)
{
    Uint32 color = SDL_MapRGB(surface->format, currentColor.r, currentColor.g, currentColor.b);
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    if (bpp == 2) {
        *(Uint16*)p = (Uint16)color;
    } else if (bpp == 3) {
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (color >> 16) & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = color & 0xff;
        } else {
            p[0] = color & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = (color >> 16) & 0xff;
        }
    } else if (bpp == 4) {
        *(Uint32*)p = color;
    }
}

void Graphics::drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        _putpixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void Graphics::drawImage(Image* const image, int x, int y, int anchor)
{
    SDL_Surface* imgSurface = image->getSurface();
    x = getAnchorX(x, imgSurface->w, anchor);
    y = getAnchorY(y, imgSurface->h, anchor);
    SDL_Rect dstRect = { (Sint16)x, (Sint16)y, (Uint16)imgSurface->w, (Uint16)imgSurface->h };
    SDL_BlitSurface(imgSurface, nullptr, this->surface, &dstRect);
}

int Graphics::getAnchorX(int x, int size, int anchor)
{
    if ((anchor & LEFT) != 0) {
        return x;
    }
    if ((anchor & RIGHT) != 0) {
        return x - size;
    }
    if ((anchor & HCENTER) != 0) {
        return x - size / 2;
    }
    throw std::runtime_error("unknown xanchor = " + std::to_string(anchor));
}

int Graphics::getAnchorY(int y, int size, int anchor)
{
    if ((anchor & TOP) != 0) {
        return y;
    }
    if ((anchor & BOTTOM) != 0) {
        return y - size;
    }
    if ((anchor & VCENTER) != 0) {
        return y - size / 2;
    }
    throw std::runtime_error("unknown yanchor = " + std::to_string(anchor));
}
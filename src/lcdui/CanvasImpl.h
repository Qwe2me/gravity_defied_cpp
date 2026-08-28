#pragma once

#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Canvas;

class CanvasImpl {
private:
    Canvas* canvas;

    SDL_Surface* screen;
    SDL_Surface* window;

    const int width = 320;
    const int height = 240;

    static int convertKeyCharToKeyCode(SDLKey keyCode);

public:
    CanvasImpl(Canvas* canvas);
    ~CanvasImpl();

    void repaint();
    int getWidth();
    int getHeight();

    SDL_Surface* getRenderer();
    void processEvents();
    void setWindowTitle(const std::string& title);
};
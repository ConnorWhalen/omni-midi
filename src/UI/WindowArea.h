#ifndef OMNI_WINDOWAREA_H
#define OMNI_WINDOWAREA_H

#include <SDL2/SDL.h>

class WindowArea {
public:
    static SDL_Rect* ConstructSDLRect(int w, int h, int x, int y) {
        auto rect = new SDL_Rect;
        rect->w = w;
        rect->h = h;
        rect->x = x;
        rect->y = y;
        return rect;
    }

    virtual void Destroy() = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;
};

#endif

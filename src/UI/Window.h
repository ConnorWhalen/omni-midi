#ifndef OMNI_WINDOW_H
#define OMNI_WINDOW_H

#include <vector>

#include <SDL2/SDL.h>

#include "WindowArea.h"

constexpr int GAME_WINDOW_WIDTH = 800;
constexpr int GAME_WINDOW_HEIGHT = 480;
constexpr int GAME_WIDTH = 800;
constexpr int GAME_HEIGHT = 480;


class Window {
public:
    Window(int width, int height, bool isFullscreen);
    void Render();
    void Destroy();
    SDL_Renderer* GetRenderer() {return renderer;};
    void AddWindowArea(WindowArea* windowArea);
protected:
    int width;
    int height;
    SDL_Rect rect;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* bgTexture;
    std::vector<WindowArea*>* windowAreas;
};

#endif

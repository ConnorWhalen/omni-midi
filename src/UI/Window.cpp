#include "Window.h"

#include <SDL2_image/SDL_image.h>

Window::Window(int width, int height, bool isFullscreen) {
    this->window = SDL_CreateWindow("SDL WINDOW", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                                    SDL_WINDOW_ALLOW_HIGHDPI);
    if(this->window == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
        this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    }
    this->width = width;
    this->height = height;
    this->rect = (SDL_Rect) {0, 0, width, height};
    this->windowAreas = new std::vector<WindowArea*>();

    if (isFullscreen) {
        SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN);
    }
}

void Window::setBg(std::string bgFilename) {
    SDL_Surface* loadedSurface = IMG_Load(bgFilename.c_str());
    this->bgTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
}

void Window::Render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, nullptr, &rect);
    for (WindowArea* w : *windowAreas) {
        w->Render(renderer);
    }
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
}

void Window::Destroy() {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    for (WindowArea* w : *windowAreas) {
        w->Destroy();
    }
}

void Window::AddWindowArea(WindowArea* windowArea) {
    this->windowAreas->push_back(windowArea);
}

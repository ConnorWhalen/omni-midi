#include "Harp.h"

#include <SDL2_image/SDL_image.h>


Harp::Harp(SDL_Rect rect_, SDL_Renderer* renderer, const std::string& imagePath, std::vector<Input*>* inputs) {
    SDL_Surface* loadedSurface = IMG_Load(imagePath.c_str());
    this->texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    this->renderer = renderer;
    // this->rect = (SDL_Rect) { rect_.x*2, rect_.y*2, rect_.w*2, rect_.h*2 };
    this->rect = (SDL_Rect) { rect_.x, rect_.y, rect_.w, rect_.h };
    this->inputs = inputs;
}

void Harp::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Harp::Destroy() {
    SDL_DestroyTexture(texture);
}
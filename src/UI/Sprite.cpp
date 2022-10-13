#include "Sprite.h"

#include <SDL2_image/SDL_image.h>

Sprite::Sprite(SDL_Rect rect_, SDL_Renderer* renderer, const std::string& imagePath) {
    SDL_Surface* loadedSurface = IMG_Load(imagePath.c_str());
    this->texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    // Everything is halved on MacOS for some reason. Do this instead to fix it
    // this->rect = (SDL_Rect) { rect_.x*2, rect_.y*2, rect_.w*2, rect_.h*2 };
    this->rect = (SDL_Rect) { rect_.x, rect_.y, rect_.w, rect_.h };
}

void Sprite::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Sprite::Destroy() {
    SDL_DestroyTexture(texture);
}

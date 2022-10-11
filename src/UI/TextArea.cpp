#include "TextArea.h"

#include <sstream>

TextArea::TextArea(const std::string fontFile) {
    this->offsetX = 0;
    this->offsetY = 0;
    this->message = nullptr;

    if (TTF_Init() == -1) {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    this->font = font = TTF_OpenFont(fontFile.c_str(), FONT_SIZE);
    if (font == nullptr) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}

void TextArea::SetText(const std::string text, const SDL_Color color, SDL_Renderer* renderer) {
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), color);

    rect.w = surfaceMessage->w;
    rect.h = surfaceMessage->h;

    if (message) {
        SDL_DestroyTexture(message);
    }
    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_FreeSurface(surfaceMessage);
}

void TextArea::Offset(const int offsetX, const int offsetY) {
    this->offsetX = offsetX;
    this->offsetY = offsetY;
}

void TextArea::Clear() {
    if (message) {
        SDL_DestroyTexture(message);
    }
}

void TextArea::Render(SDL_Renderer* renderer) {
    if (message) {
        rect.x = offsetX - rect.w/2;
        rect.y = offsetY - rect.h/2;
        SDL_RenderCopy(renderer, message, nullptr, &rect);
        SDL_RenderCopy(renderer, message, nullptr, &rect);
    }
}

void TextArea::Destroy() {
    Clear();
    TTF_CloseFont(this->font);
}


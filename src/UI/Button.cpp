#include "Button.h"

#include <SDL2_image/SDL_image.h>

Button::Button(SDL_Rect buttonRect_, SDL_Renderer* renderer,
               const std::string& onButtonImagePath, const std::string& offButtonImagePath,
               std::vector<Input*>* inputs, ButtonIndex buttonIndex,
               const std::string fontFile, const std::string top_title, const std::string bottom_title,
               const bool hasLight) {
    SDL_Surface* loadedSurface = IMG_Load(onButtonImagePath.c_str());
    this->onTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    loadedSurface = IMG_Load(offButtonImagePath.c_str());
    this->offTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    this->renderer = renderer;
    // this->buttonRect = (SDL_Rect) { buttonRect_.x*2, buttonRect_.y*2, buttonRect_.w*2, buttonRect_.h*2 };
    this->buttonRect = (SDL_Rect) { buttonRect_.x, buttonRect_.y, buttonRect_.w, buttonRect_.h };
    this->inputs = inputs;
    this->buttonIndex = buttonIndex;

    this->top_text_area = new TextArea(fontFile);
    this->bottom_text_area = new TextArea(fontFile);
    this->top_text_area->Offset(buttonRect.x + buttonRect.w/2, buttonRect.y - buttonRect.h/8);
    this->bottom_text_area->Offset(buttonRect.x + buttonRect.w/2, buttonRect.y + buttonRect.h + buttonRect.h/6);
    SetTopTitle(top_title);
    SetBottomTitle(bottom_title);

    this->lightOn = false;
    this->lightRect = (SDL_Rect) {
        buttonRect.x + buttonRect.w/2 - 6,
        buttonRect.y + buttonRect.h + buttonRect.h/6,
        12,
        12
    };
    if (hasLight) {
        loadedSurface = IMG_Load("sprites/omni-button-light on.png");
        this->lightOnTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);

        loadedSurface = IMG_Load("sprites/omni-button-light off.png");
        this->lightOffTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }   
    else {
        this->lightOnTexture = nullptr;
        this->lightOffTexture = nullptr;
    }
}

void Button::SetTopTitle(const std::string text) {
    top_title = text;
    top_text_area->Clear();
    if (text != "") {
        top_text_area->SetText(top_title, {255, 255, 255}, renderer);
    }
}

void Button::SetBottomTitle(const std::string text) {
    bottom_title = text;
    bottom_text_area->Clear();
    if (text != "") {
        bottom_text_area->SetText(bottom_title, {255, 255, 255}, renderer);
    }
}

void Button::SetLight(const bool lightOn_) {
    lightOn = lightOn_;
}

void Button::Render(SDL_Renderer* renderer) {
    if (top_title != "") {
        top_text_area->Render(renderer);
    }
    if (bottom_title != "") {
        bottom_text_area->Render(renderer);
    } 

    SDL_Texture* buttonTexture = offTexture;
    for (Input *input : *inputs) {
        if (input->Read().buttonData & (1uLL << buttonIndex)) {
            buttonTexture = onTexture;
        }
    }
    SDL_RenderCopy(renderer, buttonTexture, nullptr, &buttonRect);

    if (lightOnTexture != nullptr && lightOffTexture != nullptr) {
        SDL_Texture* lightTexture = lightOffTexture;
        if (lightOn) {
            lightTexture = lightOnTexture;
        }
        SDL_RenderCopy(renderer, lightTexture, nullptr, &lightRect);
    }
}

void Button::Destroy() {
    top_text_area->Destroy();
    bottom_text_area->Destroy();

    SDL_DestroyTexture(onTexture);
    SDL_DestroyTexture(offTexture);
}

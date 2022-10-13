#ifndef OMNI_SPRITE_H
#define OMNI_SPRITE_H

#include <string>

#include "WindowArea.h"

class Sprite : public WindowArea {
public:
    Sprite(SDL_Rect buttonRect, SDL_Renderer* renderer, const std::string& imagePath);
protected:
    void Destroy() override;
    void Render(SDL_Renderer* renderer) override;
private:
    SDL_Texture* texture;
    SDL_Rect rect;
};

#endif

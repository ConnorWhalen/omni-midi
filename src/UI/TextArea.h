#ifndef OMNI_TEXTAREA_H
#define OMNI_TEXTAREA_H

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "WindowArea.h"

constexpr int FONT_SIZE = 12;

class TextArea : public WindowArea {
public:
    TextArea(const std::string fontFile);
    void SetText(const std::string text, const SDL_Color color, SDL_Renderer* renderer);
    void Offset(const int offsetX, const int offsetY);
    void Clear();
    void Destroy() override;
    void Render(SDL_Renderer* renderer) override;
private:
    SDL_Texture* message;
    SDL_Rect rect;
    int offsetX;
    int offsetY;
    TTF_Font* font;
};

#endif

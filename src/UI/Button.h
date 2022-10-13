#ifndef OMNI_BUTTON_H
#define OMNI_BUTTON_H

#include <string>
#include <vector>

#include "../input/Input.h"
#include "TextArea.h"
#include "WindowArea.h"

constexpr int BUTTON_SIZE_X = 48;
constexpr int BUTTON_SIZE_Y = 48;
constexpr int BUTTON_PADDING_X = 48;
constexpr int BUTTON_PADDING_Y = 48;
constexpr int BUTTON_SPACING_X = 0;
constexpr int BUTTON_SPACING_Y = 16;

constexpr int GH_FRET_SIZE_X = 105;
constexpr int GH_FRET_SIZE_Y = 155;
constexpr int GH_FRET_PADDING_X = 40;
constexpr int GH_FRET_PADDING_Y = 245;
constexpr int GH_FRET_SPACING_X = 10;

constexpr int GH_BUTTON_SIZE_X = 150;
constexpr int GH_BUTTON_SIZE_Y = 100;
constexpr int GH_BUTTON_PADDING_X = 625;
constexpr int GH_BUTTON_PADDING_Y = 30;
constexpr int GH_BUTTON_SPACING_Y = 10;

class Button : public WindowArea {
public:
    Button(SDL_Rect buttonRect, SDL_Renderer* renderer,
           const std::string& onButtonImagePath, const std::string& offButtonImagePath,
           std::vector<Input*>* inputs, ButtonIndex buttonIndex,
           const std::string fontFile, const std::string top_title = "", const std::string bottom_title = "",
           const bool hasLight = false);
    void SetTopTitle(const std::string text);
    void SetBottomTitle(const std::string text);
    void SetLight(bool lightOn);
protected:
    void Destroy() override;
    void Render(SDL_Renderer* renderer) override;
private:
    SDL_Renderer* renderer;
    SDL_Texture* offTexture;
    SDL_Texture* onTexture;
    SDL_Rect buttonRect;
    std::vector<Input*>* inputs;
    ButtonIndex buttonIndex;
    std::string top_title;
    std::string bottom_title;
    TextArea *top_text_area;
    TextArea *bottom_text_area;
    SDL_Texture* lightOffTexture;
    SDL_Texture* lightOnTexture;
    SDL_Rect lightRect;
    bool lightOn;
};

#endif

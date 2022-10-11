#ifndef OMNI_HARP_H
#define OMNI_HARP_H

#include <string>
#include <vector>

#include "../input/Input.h"
#include "WindowArea.h"

constexpr int HARP_SIZE_X = 64;
constexpr int HARP_SIZE_Y = 384;

class Harp : public WindowArea {
public:
    Harp(SDL_Rect rect, SDL_Renderer* renderer, const std::string& imagePath, std::vector<Input*>* inputs);
protected:
    void Destroy() override;
    void Render(SDL_Renderer* renderer) override;
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect rect;
    std::vector<Input*>* inputs;
};

#endif

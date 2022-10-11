#ifndef OMNI_TOUCHINPUT_H
#define OMNI_TOUCHINPUT_H

#include "../UI/Window.h"
#include "Input.h"

constexpr int FINGER_LIMIT = 10;

struct TouchRect {
    float top;
    float left;
    float height;
    float width;
};

class TouchInput : public Input {
public:
    static TouchRect SDLRectToTouchRect(SDL_Rect sdl) {
        return TouchRect {
            (float) sdl.y/GAME_HEIGHT,
            (float) sdl.x/GAME_WIDTH,
            (float) sdl.h/GAME_HEIGHT,
            (float) sdl.w/GAME_WIDTH
        };
    }

    TouchInput(SDL_Rect buttonRects[], SDL_Rect harpRect);
    InputData Read() const override;
    void Apply(SDL_Event e) override;

    long fingerIdMap[FINGER_LIMIT];
    float fingers[FINGER_LIMIT*2];
private:
    static bool Intersecting(TouchRect, float x, float y);
    int GetFingerIndex(long fingerId, float x, float y);
    void UpdateCurrent();

    InputData current;

    TouchRect rects[TOTAL_BUTTONS];
    TouchRect harpRect;

    int lastZero;
};

#endif

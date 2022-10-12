#include "TouchInput.h"

bool TouchInput::Intersecting(TouchRect rect, float x, float y) {
    return (y > rect.top  && y < rect.top+rect.height &&
            x > rect.left && x < rect.left+rect.width);
}

TouchInput::TouchInput() {
    this->lastZero = 0;
    this->current.buttonData = 0;
    this->current.harpData = -1;
}

void TouchInput::Configure(SDL_Rect buttonRects[], SDL_Rect harpRect) {
    for (int i = 0; i < TOTAL_BUTTONS; i++) {
        this->rects[i] = TouchInput::SDLRectToTouchRect(buttonRects[i]);
    }
    this->harpRect = TouchInput::SDLRectToTouchRect(harpRect);
}

InputData TouchInput::Read() const {
    return this->current;
}

int TouchInput::GetFingerIndex(const long fingerId, float x, float y) {
    int index_to_use = -1;
    for (int i = 0; i < 10; i++) {
        if (fingerIdMap[i] == fingerId ||
                (x-fingers[i*2] < 0.05 &&
                 x-fingers[i*2] > -0.05 &&
                 y-fingers[i*2+1] < 0.05 &&
                 y-fingers[i*2+1] > -0.05)) {
            index_to_use = i;
        }
    }
    if (index_to_use == -1) {
        fingerIdMap[lastZero] = fingerId;
        index_to_use = lastZero++;
        if (lastZero == FINGER_LIMIT) lastZero = 0;
    }
    return index_to_use;
}

void TouchInput::UpdateCurrent() {
    this->current.buttonData = 0;
    this->current.harpData = -1;
    for (int i = 0; i < FINGER_LIMIT; i++) {
        for (int j = 0; j < TOTAL_BUTTONS; j++) {
            if (Intersecting(rects[j], fingers[i*2], fingers[i*2+1])) {
                this->current.buttonData |= (1uLL << j);
            }
        }
        if (Intersecting(harpRect, fingers[i*2], fingers[i*2+1])) {
            float harpPoint = (harpRect.top + harpRect.height) - fingers[i*2+1];
            this->current.harpData = harpPoint * (float)HARP_RANGE/harpRect.height; // normalize
        }
    }
}

void TouchInput::Apply(const SDL_Event e) {
    int index_to_use = this->GetFingerIndex(e.tfinger.fingerId, e.tfinger.x, e.tfinger.y);
    if (e.type == SDL_FINGERDOWN) {
        fingers[index_to_use*2] = e.tfinger.x;
        fingers[index_to_use*2+1] = e.tfinger.y;
    } else if (e.type == SDL_FINGERUP) {
        fingers[index_to_use*2] = 0;
        fingers[index_to_use*2+1] = 0;
    } else if (e.type == SDL_FINGERMOTION) {
        fingers[index_to_use*2] = e.tfinger.x;
        fingers[index_to_use*2+1] = e.tfinger.y;
    }
    this->UpdateCurrent();
}

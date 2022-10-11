#include "KeyboardInput.h"

KeyboardInput::KeyboardInput() {
    this->current.buttonData = 0;
    this->current.harpData = -1;
}

InputData KeyboardInput::Read() const {
    return this->current;
}

void KeyboardInput::Apply(const SDL_Event e) {
    if (e.type == SDL_KEYDOWN) {
        for (int i = 0; i < TOTAL_BUTTONS; i++) {
            if (e.key.keysym.sym == SDL_GetKeyFromName(BUTTON_CODES[i])) {
                this->current.buttonData |= (1uLL << i);
            }
        }
    } else if (e.type == SDL_KEYUP) {
        for (int i = 0; i < TOTAL_BUTTONS; i++) {
            if (e.key.keysym.sym == SDL_GetKeyFromName(BUTTON_CODES[i])) {
                this->current.buttonData &= ~(1uLL << i);
            }
        }
    }
}

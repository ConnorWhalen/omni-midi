#ifndef OMNI_KEYBOARDINPUT_H
#define OMNI_KEYBOARDINPUT_H

#include <string>

#include "Input.h"

const char* const BUTTON_CODES[] {
    // chord buttons
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]",
    "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "\\",
    // mute  hold chords  octave +  octave -  c volume +  c volume -  h volume +  h volume -
       "M",  ",",         "/",      ".",      "X",        "Z",        "V",        "C",
    // mirror to keep 64 total
                                            "9", "10", "-", "=",
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]",
    "A", "S", "D", "F",
};

class KeyboardInput : public Input {
public:
    KeyboardInput();
    InputData Read() const override;
    void Apply(SDL_Event e) override;
private:
    InputData current;
};

#endif

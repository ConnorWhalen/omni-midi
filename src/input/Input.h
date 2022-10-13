#ifndef OMNI_INPUT_H
#define OMNI_INPUT_H

#include <SDL2/SDL.h>

typedef uint64_t ButtonData; // max 64 buttons
typedef unsigned char ButtonIndex;
typedef char HarpData;
struct InputData {
    // buttonData is a state of all buttons.
    // for ButtonIndex n with value m and buttonData i
    // m == i & (1 << n)
    // i == (m << n) for all n and their m
    ButtonData buttonData;

    // harpData is the y position being touched on the harp normalized to 0-36.
    // 0 is the bottom of the harp and 31 is the top.
    // value is -1 if the harp is not being played.
    HarpData harpData;
};

constexpr int TOTAL_BUTTONS = 64;

// omni
constexpr int CHORD_BUTTONS_ROWS = 3;
constexpr int CHORD_BUTTONS_COLS = 12;
constexpr int TOTAL_CHORD_BUTTONS = CHORD_BUTTONS_ROWS*CHORD_BUTTONS_COLS;

constexpr int MUTE_BUTTON_INDEX = TOTAL_CHORD_BUTTONS;
constexpr int HOLD_CHORD_BUTTON_INDEX = MUTE_BUTTON_INDEX+1;
constexpr int CHORD_OCTAVE_UP_BUTTON_INDEX = HOLD_CHORD_BUTTON_INDEX+1;
constexpr int CHORD_OCTAVE_DOWN_BUTTON_INDEX = CHORD_OCTAVE_UP_BUTTON_INDEX+1;
constexpr int CHORD_VOLUME_UP_BUTTON_INDEX = CHORD_OCTAVE_DOWN_BUTTON_INDEX+1;
constexpr int CHORD_VOLUME_DOWN_BUTTON_INDEX = CHORD_VOLUME_UP_BUTTON_INDEX+1;
constexpr int HARP_VOLUME_UP_BUTTON_INDEX = CHORD_VOLUME_DOWN_BUTTON_INDEX+1;
constexpr int HARP_VOLUME_DOWN_BUTTON_INDEX = HARP_VOLUME_UP_BUTTON_INDEX+1;

constexpr char HARP_RANGE = 36;

// gh
constexpr int GH_FRET_BUTTONS = 6;
constexpr int GH_CONTROL_BUTTONS = 4;

constexpr int GH_PURPLE_FRET_INDEX = 0;
constexpr int GH_GREEN_FRET_INDEX = GH_PURPLE_FRET_INDEX + 1;
constexpr int GH_RED_FRET_INDEX = GH_GREEN_FRET_INDEX + 1;
constexpr int GH_YELLOW_FRET_INDEX = GH_RED_FRET_INDEX + 1;
constexpr int GH_BLUE_FRET_INDEX = GH_YELLOW_FRET_INDEX + 1;
constexpr int GH_ORANGE_FRET_INDEX = GH_BLUE_FRET_INDEX + 1;

constexpr int GH_HOLD_BUTTON_INDEX = GH_ORANGE_FRET_INDEX + 1;
constexpr int GH_HOPO_BUTTON_INDEX = GH_HOLD_BUTTON_INDEX + 1;
constexpr int GH_TAP_BUTTON_INDEX = GH_HOPO_BUTTON_INDEX + 1;
constexpr int GH_STAR_BUTTON_INDEX = GH_TAP_BUTTON_INDEX + 1;


class Input {
public:
    virtual InputData Read() const = 0;
    virtual void Apply(SDL_Event e) = 0;
};

#endif

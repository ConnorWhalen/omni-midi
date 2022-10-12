#include "OmniDevice.h"

#include "../controller/OmniController.h"
#include "../UI/Button.h"
#include "../UI/Harp.h"
#include "../UI/TextArea.h"

OmniDevice::OmniDevice(Window* window, std::vector<Input*>* inputs, TouchInput &touchInput, const int midiPort, const std::string fontFile) {
    SDL_Rect buttonRects[TOTAL_BUTTONS];
    for (int i = 0; i < CHORD_BUTTONS_ROWS; i++) {
        for (int j = 0; j < CHORD_BUTTONS_COLS; j++) {
            buttonRects[i*CHORD_BUTTONS_COLS + j] = (SDL_Rect) {
                BUTTON_PADDING_X + j*(BUTTON_SIZE_X + BUTTON_SPACING_X),
                BUTTON_PADDING_Y + i*(BUTTON_SIZE_Y + BUTTON_SPACING_Y),
                BUTTON_SIZE_X,
                BUTTON_SIZE_Y
            };
        }
    }
    buttonRects[MUTE_BUTTON_INDEX] = (SDL_Rect) {
        576,
        336,
        BUTTON_SIZE_X,
        BUTTON_SIZE_Y
    };
    buttonRects[HOLD_CHORD_BUTTON_INDEX] = (SDL_Rect) {
        480,
        336,
        BUTTON_SIZE_X,
        BUTTON_SIZE_Y
    };
    buttonRects[CHORD_OCTAVE_UP_BUTTON_INDEX] = (SDL_Rect) {
        336,
        311,
        BUTTON_SIZE_X,
        BUTTON_SIZE_Y
    };
    buttonRects[CHORD_OCTAVE_DOWN_BUTTON_INDEX] = (SDL_Rect) {
        336,
        359,
        BUTTON_SIZE_X,
        BUTTON_SIZE_Y
    };
    buttonRects[CHORD_VOLUME_UP_BUTTON_INDEX] = (SDL_Rect) {
        56,
        311,
        BUTTON_SIZE_X,
        BUTTON_SIZE_Y
    };
    buttonRects[CHORD_VOLUME_DOWN_BUTTON_INDEX] = (SDL_Rect) {
        56,
        359,
        BUTTON_SIZE_X,
        BUTTON_SIZE_Y
    };
    buttonRects[HARP_VOLUME_UP_BUTTON_INDEX] = (SDL_Rect) {
        184,
        311,
        BUTTON_SIZE_X,
        BUTTON_SIZE_Y
    };
    buttonRects[HARP_VOLUME_DOWN_BUTTON_INDEX] = (SDL_Rect) {
        184,
        359,
        BUTTON_SIZE_X,
        BUTTON_SIZE_Y
    };

    auto harpRect = (SDL_Rect) {
        700,
        48,
        HARP_SIZE_X,
        HARP_SIZE_Y
    };

    touchInput.Configure(buttonRects, harpRect);

    auto majorLabel = new TextArea(fontFile);
    auto minorLabel = new TextArea(fontFile);
    auto sevenLabel = new TextArea(fontFile);
    majorLabel->Offset(24, 72);
    minorLabel->Offset(24, 134);
    sevenLabel->Offset(24, 196);
    majorLabel->SetText("Maj", {255, 255, 255}, window->GetRenderer());
    minorLabel->SetText("Min", {255, 255, 255}, window->GetRenderer());
    sevenLabel->SetText("7", {255, 255, 255}, window->GetRenderer());
    window->AddWindowArea(majorLabel);
    window->AddWindowArea(minorLabel);
    window->AddWindowArea(sevenLabel);

    Button *buttons[TOTAL_BUTTONS];
    for (int i = 0; i < CHORD_BUTTONS_ROWS; i++) {
        for (int j = 0; j < CHORD_BUTTONS_COLS; j++) {
            buttons[i*CHORD_BUTTONS_COLS + j] = new Button(
                buttonRects[i*CHORD_BUTTONS_COLS + j],
                window->GetRenderer(),
                "sprites/omni-button-on.png",
                "sprites/omni-button-off.png",
                inputs,
                i*CHORD_BUTTONS_COLS + j,
                fontFile
            );
            window->AddWindowArea(buttons[i*CHORD_BUTTONS_COLS + j]);
        }
    }

    buttons[MUTE_BUTTON_INDEX] = new Button(
        buttonRects[MUTE_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/omni-button-on.png",
        "sprites/omni-button-off.png",
        inputs,
        MUTE_BUTTON_INDEX,
        fontFile,
        "Mute"
    );
    window->AddWindowArea(buttons[MUTE_BUTTON_INDEX]);

    buttons[HOLD_CHORD_BUTTON_INDEX] = new Button(
        buttonRects[HOLD_CHORD_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/omni-button-on.png",
        "sprites/omni-button-off.png",
        inputs,
        HOLD_CHORD_BUTTON_INDEX,
        fontFile,
        "Hold Chords",
        "",
        true
    );
    window->AddWindowArea(buttons[HOLD_CHORD_BUTTON_INDEX]);

    buttons[CHORD_OCTAVE_UP_BUTTON_INDEX] = new Button(
        buttonRects[CHORD_OCTAVE_UP_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/omni-button-up-on.png",
        "sprites/omni-button-up-off.png",
        inputs,
        CHORD_OCTAVE_UP_BUTTON_INDEX,
        fontFile,
        "Chord Octave"
    );
    window->AddWindowArea(buttons[CHORD_OCTAVE_UP_BUTTON_INDEX]);

    buttons[CHORD_OCTAVE_DOWN_BUTTON_INDEX] = new Button(
        buttonRects[CHORD_OCTAVE_DOWN_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/omni-button-down-on.png",
        "sprites/omni-button-down-off.png",
        inputs,
        CHORD_OCTAVE_DOWN_BUTTON_INDEX,
        fontFile,
        "",
        "0"
    );
    window->AddWindowArea(buttons[CHORD_OCTAVE_DOWN_BUTTON_INDEX]);

    buttons[CHORD_VOLUME_UP_BUTTON_INDEX] = new Button(
        buttonRects[CHORD_VOLUME_UP_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/omni-button-up-on.png",
        "sprites/omni-button-up-off.png",
        inputs,
        CHORD_VOLUME_UP_BUTTON_INDEX,
        fontFile,
        "Chord Volume"
    );
    window->AddWindowArea(buttons[CHORD_VOLUME_UP_BUTTON_INDEX]);

    buttons[CHORD_VOLUME_DOWN_BUTTON_INDEX] = new Button(
        buttonRects[CHORD_VOLUME_DOWN_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/omni-button-down-on.png",
        "sprites/omni-button-down-off.png",
        inputs,
        CHORD_VOLUME_DOWN_BUTTON_INDEX,
        fontFile,
        "",
        "100"
    );
    window->AddWindowArea(buttons[CHORD_VOLUME_DOWN_BUTTON_INDEX]);

    buttons[HARP_VOLUME_UP_BUTTON_INDEX] = new Button(
        buttonRects[HARP_VOLUME_UP_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/omni-button-up-on.png",
        "sprites/omni-button-up-off.png",
        inputs,
        HARP_VOLUME_UP_BUTTON_INDEX,
        fontFile,
        "Harp Volume"
    );
    window->AddWindowArea(buttons[HARP_VOLUME_UP_BUTTON_INDEX]);

    buttons[HARP_VOLUME_DOWN_BUTTON_INDEX] = new Button(
        buttonRects[HARP_VOLUME_DOWN_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/omni-button-down-on.png",
        "sprites/omni-button-down-off.png",
        inputs,
        HARP_VOLUME_DOWN_BUTTON_INDEX,
        fontFile,
        "",
        "100"
    );
    window->AddWindowArea(buttons[HARP_VOLUME_DOWN_BUTTON_INDEX]);

    auto harp = new Harp(
        harpRect,
        window->GetRenderer(),
        "sprites/omni-harp.png",
        inputs
    );
    window->AddWindowArea(harp);

    this->controller = new OmniController(inputs, buttons, midiPort);
}

void OmniDevice::Destroy() {
	controller->Destroy();
}

#include "GhDevice.h"

#include "../controller/GhController.h"
#include "../UI/Button.h"
#include "../UI/Sprite.h"

GhDevice::GhDevice(Window* window, std::vector<Input*>* inputs, TouchInput &touchInput, const int midiPort, const std::string fontFile) {
    SDL_Rect buttonRects[TOTAL_BUTTONS];
    buttonRects[GH_PURPLE_FRET_INDEX] = (SDL_Rect) {
        50,
        60,
        520,
        100
    };
    for (int i = 1; i < GH_FRET_BUTTONS; i++) {
        buttonRects[i] = (SDL_Rect) {
            GH_FRET_PADDING_X + (i-1)*(GH_FRET_SIZE_X + GH_FRET_SPACING_X),
            GH_FRET_PADDING_Y,
            GH_FRET_SIZE_X,
            GH_FRET_SIZE_Y
        };
    }
    for (int i = 0; i < GH_CONTROL_BUTTONS; i++) {
        buttonRects[i + GH_FRET_BUTTONS] = (SDL_Rect) {
            GH_BUTTON_PADDING_X,
            GH_BUTTON_PADDING_Y + i*(GH_BUTTON_SIZE_Y + GH_BUTTON_SPACING_Y),
            GH_BUTTON_SIZE_X,
            GH_BUTTON_SIZE_Y
        };
    }

    touchInput.Configure(buttonRects, (SDL_Rect) {0, 0, 0, 0});

    Button *buttons[TOTAL_BUTTONS];

    buttons[GH_PURPLE_FRET_INDEX] = new Button(
        buttonRects[GH_PURPLE_FRET_INDEX],
        window->GetRenderer(),
        "sprites/gh-purple-on.png",
        "sprites/gh-purple-off.png",
        inputs,
        GH_PURPLE_FRET_INDEX,
        fontFile
    );
    window->AddWindowArea(buttons[GH_PURPLE_FRET_INDEX]);

    buttons[GH_GREEN_FRET_INDEX] = new Button(
        buttonRects[GH_GREEN_FRET_INDEX],
        window->GetRenderer(),
        "sprites/gh-green-on.png",
        "sprites/gh-green-off.png",
        inputs,
        GH_GREEN_FRET_INDEX,
        fontFile
    );
    window->AddWindowArea(buttons[GH_GREEN_FRET_INDEX]);

    buttons[GH_RED_FRET_INDEX] = new Button(
        buttonRects[GH_RED_FRET_INDEX],
        window->GetRenderer(),
        "sprites/gh-red-on.png",
        "sprites/gh-red-off.png",
        inputs,
        GH_RED_FRET_INDEX,
        fontFile
    );
    window->AddWindowArea(buttons[GH_RED_FRET_INDEX]);

    buttons[GH_YELLOW_FRET_INDEX] = new Button(
        buttonRects[GH_YELLOW_FRET_INDEX],
        window->GetRenderer(),
        "sprites/gh-yellow-on.png",
        "sprites/gh-yellow-off.png",
        inputs,
        GH_YELLOW_FRET_INDEX,
        fontFile
    );
    window->AddWindowArea(buttons[GH_YELLOW_FRET_INDEX]);

    buttons[GH_BLUE_FRET_INDEX] = new Button(
        buttonRects[GH_BLUE_FRET_INDEX],
        window->GetRenderer(),
        "sprites/gh-blue-on.png",
        "sprites/gh-blue-off.png",
        inputs,
        GH_BLUE_FRET_INDEX,
        fontFile
    );
    window->AddWindowArea(buttons[GH_BLUE_FRET_INDEX]);

    buttons[GH_ORANGE_FRET_INDEX] = new Button(
        buttonRects[GH_ORANGE_FRET_INDEX],
        window->GetRenderer(),
        "sprites/gh-orange-on.png",
        "sprites/gh-orange-off.png",
        inputs,
        GH_ORANGE_FRET_INDEX,
        fontFile
    );
    window->AddWindowArea(buttons[GH_ORANGE_FRET_INDEX]);

    buttons[GH_HOLD_BUTTON_INDEX] = new Button(
        buttonRects[GH_HOLD_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/gh-grey-on.png",
        "sprites/gh-grey-off.png",
        inputs,
        GH_HOLD_BUTTON_INDEX,
        fontFile
    );
    window->AddWindowArea(buttons[GH_HOLD_BUTTON_INDEX]);

    buttons[GH_HOPO_BUTTON_INDEX] = new Button(
        buttonRects[GH_HOPO_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/gh-grey-on.png",
        "sprites/gh-grey-off.png",
        inputs,
        GH_HOPO_BUTTON_INDEX,
        fontFile
    );
    window->AddWindowArea(buttons[GH_HOPO_BUTTON_INDEX]);

    buttons[GH_TAP_BUTTON_INDEX] = new Button(
        buttonRects[GH_TAP_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/gh-grey-on.png",
        "sprites/gh-grey-off.png",
        inputs,
        GH_TAP_BUTTON_INDEX,
        fontFile
    );
    window->AddWindowArea(buttons[GH_TAP_BUTTON_INDEX]);

    buttons[GH_STAR_BUTTON_INDEX] = new Button(
        buttonRects[GH_STAR_BUTTON_INDEX],
        window->GetRenderer(),
        "sprites/gh-grey-on.png",
        "sprites/gh-grey-off.png",
        inputs,
        GH_STAR_BUTTON_INDEX,
        fontFile
    );
    window->AddWindowArea(buttons[GH_STAR_BUTTON_INDEX]);


    Sprite *holdTailSprite = new Sprite(
        (SDL_Rect) {
            700-36,
            30+300+30+50-24,
            96,
            48
        },
        window->GetRenderer(),
        "sprites/gh-tail.png"
    );
    window->AddWindowArea(holdTailSprite);

    Sprite *holdNoteSprite = new Sprite(
        (SDL_Rect) {
            700-48,
            30+300+30+50-24,
            48,
            48
        },
        window->GetRenderer(),
        "sprites/gh-note.png"
    );
    window->AddWindowArea(holdNoteSprite);

    Sprite *hopoSprite = new Sprite(
        (SDL_Rect) {
            700-24,
            30+200+20+50-24,
            48,
            48
        },
        window->GetRenderer(),
        "sprites/gh-hopo.png"
    );
    window->AddWindowArea(hopoSprite);

    Sprite *tapSprite = new Sprite(
        (SDL_Rect) {
            700-24,
            30+100+10+50-24,
            48,
            48
        },
        window->GetRenderer(),
        "sprites/gh-tap.png"
    );
    window->AddWindowArea(tapSprite);

    Sprite *starSprite = new Sprite(
        (SDL_Rect) {
            700-32,
            30+50-24,
            64,
            48
        },
        window->GetRenderer(),
        "sprites/gh-star.png"
    );
    window->AddWindowArea(starSprite);


    this->controller = new GhController(inputs, buttons, midiPort);
}

void GhDevice::Destroy() {
    controller->Destroy();
}

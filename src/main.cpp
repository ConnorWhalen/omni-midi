#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "Config.h"
#include "controller/ChordController.h"
#include "input/KeyboardInput.h"
#include "input/TouchInput.h"
#include "UI/Button.h"
#include "UI/Harp.h"
#include "UI/TextArea.h"
#include "UI/Window.h"

Window* window;
ChordController* chordController = nullptr;

bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}

void close(){
    if (window != nullptr) {
        window->Destroy();
    }
    if (chordController != nullptr) {
        chordController->Destroy();
    }


    SDL_Quit();
}

int main(int argc, char *argv[]) {
    std::string rom_file;
    std::string config_file;
    for (int i = 1; i < argc; i++ ){
        if (!strcmp(argv[i], "--config")) {
            config_file = argv[++i];
        }
    }

    Config config(config_file);
    bool isFullscreen = (bool) stoi(config.Get("fullscreen", "0"));
    int midiPort = stoi(config.Get("midiport", "0"));
    std::string fontFile = config.Get("font", "/System/Library/Fonts/Supplemental/Courier New.ttf");

    SDL_Color Red = {255, 80, 80};
    SDL_Color White = {255, 255, 255};
    if(!init()) {
        printf("Failed to initialize SDL. Exiting!\n");
        return -1;
    }

    window = new Window(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, isFullscreen);

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

    KeyboardInput keyboardInput;
    TouchInput touchInput(buttonRects, harpRect);
    auto inputs = new std::vector<Input*>();
    inputs->push_back(&keyboardInput);
    inputs->push_back(&touchInput);

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

    chordController = new ChordController(inputs, buttons, midiPort);

    bool quit = false;
    SDL_Event inputEvent;
    int fps = 60;
    int frameDelay = 1000 / fps;
    int frameStart;
    int frameTime;
    while (!quit) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&inputEvent) != 0) {
            if (inputEvent.type == SDL_QUIT ||
               (inputEvent.type == SDL_KEYUP && inputEvent.key.keysym.sym == SDLK_ESCAPE)) {
                quit = true;
            }
            for (Input *input : *inputs) {
                input->Apply(inputEvent);
            }
        }

        chordController->Update();

        window->Render();
        window->Render();

        frameTime = SDL_GetTicks() - frameStart;
        // This keeps us from displaying more frames than 60/Second
        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    close();

    return 0;
}

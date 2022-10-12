#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "Config.h"
#include "device/OmniDevice.h"
#include "input/KeyboardInput.h"
#include "input/TouchInput.h"
#include "UI/Button.h"
#include "UI/Harp.h"
#include "UI/TextArea.h"
#include "UI/Window.h"

Window* window;
Device* device = nullptr;

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
    if (device != nullptr) {
        device->Destroy();
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

    KeyboardInput keyboardInput;
    TouchInput touchInput;
    auto inputs = new std::vector<Input*>();
    inputs->push_back(&keyboardInput);
    inputs->push_back(&touchInput);

    device = new OmniDevice(window, inputs, touchInput, midiPort, fontFile);

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

        device->Update();

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

#include "GhController.h"

#include <algorithm>

GhController::GhController(std::vector<Input*>* inputs, Button *buttons[], const int midiPort) : inputs(inputs) {
    this->heldNotes = new std::vector<MidiNote>();
    this->midiController = new MidiControl(midiPort);

    for (int i = 0; i < TOTAL_BUTTONS; i++) {
        this->buttons[i] = buttons[i];
    }

    midiController->SetVolume(100);
}

void GhController::Update() {
    InputData currentInput = (InputData) {0, (char) -1};
    for (Input* input : *inputs) {
        InputData inputData = input->Read();
        currentInput.buttonData |= inputData.buttonData;
    }

    for (ButtonIndex i = 0; i < TOTAL_BUTTONS; i++) {
        bool currentState = (currentInput.buttonData & (1uLL << i));
        bool previousState = (previousInput.buttonData & (1uLL << i));
        if (currentState && !previousState) {
            ButtonPressed(i, currentInput.buttonData);
        } else if (!currentState and previousState) {
            ButtonReleased(i, currentInput.buttonData);
        }
    }

    previousInput = currentInput;
}

void GhController::ButtonPressed(ButtonIndex buttonIndex, const ButtonData currentInput) {
    if (buttonIndex < GH_FRET_BUTTONS) {
        // Play note buttonIndex
    }
    else if (buttonIndex == GH_HOLD_BUTTON_INDEX) {
        // Hold mode
    }
    else if (buttonIndex == GH_HOPO_BUTTON_INDEX) {
        // HOPO
    }
    else if (buttonIndex == GH_TAP_BUTTON_INDEX) {
        // Tap
    }
    else if (buttonIndex == GH_STAR_BUTTON_INDEX) {
        // Star power
    }
}

void GhController::ButtonReleased(ButtonIndex buttonIndex, const ButtonData currentInput) {
    if (buttonIndex < GH_FRET_BUTTONS) {
        // Release note buttonIndex
    }
}

void GhController::Destroy() {
    for (MidiNote midiNote : *heldNotes) {
        midiController->ReleaseNote(midiNote.pitch);
    }
}

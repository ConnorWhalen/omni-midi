#include "GhController.h"

#include <algorithm>

GhController::GhController(std::vector<Input*>* inputs, Button *buttons[], const int midiPort) : inputs(inputs) {
    this->heldNotes = new std::vector<MidiNote>();
    this->midiController = new MidiControl(midiPort);
    this->holdMode = false;
    this->hopoMode = false;
    this->tapMode = false;

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

    int current_time = SDL_GetTicks();
    for (int i = heldNotes->size()-1; i >= 0; i--) {
        if ((heldNotes->at(i).pitch == GH_HOPO_PITCH || heldNotes->at(i).pitch == GH_TAP_PITCH)
            && heldNotes->at(i).timestamp < current_time + GH_TEMP_NOTE_DURATION) {
            midiController->ReleaseNote(heldNotes->at(i).pitch);
            heldNotes->erase(heldNotes->begin() + i);
        }
    }

    previousInput = currentInput;
}

void GhController::ButtonPressed(ButtonIndex buttonIndex, const ButtonData currentInput) {
    if (buttonIndex < GH_FRET_BUTTONS) {
        int midiPitch = GH_BASE_FRET_PITCH + (2*buttonIndex);
        if (holdMode) midiPitch++;

        int timestamp = SDL_GetTicks();
        midiController->PlayNote(midiPitch);
        heldNotes->push_back((MidiNote) { midiPitch, timestamp });
        if (hopoMode) {
            midiController->PlayNote(GH_HOPO_PITCH);
            heldNotes->push_back((MidiNote) { GH_HOPO_PITCH, timestamp });
        }
        if (tapMode) {
            midiController->PlayNote(GH_TAP_PITCH);
            heldNotes->push_back((MidiNote) { GH_TAP_PITCH, timestamp });
        }
    }
    else if (buttonIndex == GH_HOLD_BUTTON_INDEX) {
        holdMode = true;
    }
    else if (buttonIndex == GH_HOPO_BUTTON_INDEX) {
        hopoMode = true;
    }
    else if (buttonIndex == GH_TAP_BUTTON_INDEX) {
        tapMode = true;
    }
    else if (buttonIndex == GH_STAR_BUTTON_INDEX) {
        auto starNoteIt = std::find(heldNotes->begin(), heldNotes->end(), (MidiNote) { GH_STAR_PITCH, 0 });
        if (starNoteIt != heldNotes->end()) {
            midiController->ReleaseNote(GH_STAR_PITCH);
            heldNotes->erase(starNoteIt);
        }
        else {
            midiController->PlayNote(GH_STAR_PITCH);
            heldNotes->push_back((MidiNote) { GH_STAR_PITCH, 0 });
        }
    }
}

void GhController::ButtonReleased(ButtonIndex buttonIndex, const ButtonData currentInput) {
    if (buttonIndex < GH_FRET_BUTTONS) {
        for (int i = 0; i < 2; i++) {
            int midiPitch = GH_BASE_FRET_PITCH + (2*buttonIndex) + i;
            auto heldNoteIt = std::find(heldNotes->begin(), heldNotes->end(), (MidiNote) { midiPitch, 0 });
            if (heldNoteIt != heldNotes->end()) {
                midiController->ReleaseNote(midiPitch);
                heldNotes->erase(heldNoteIt);
            }
        }
    }
    else if (buttonIndex == GH_HOLD_BUTTON_INDEX) {
        holdMode = false;
    }
    else if (buttonIndex == GH_HOPO_BUTTON_INDEX) {
        hopoMode = false;
    }
    else if (buttonIndex == GH_TAP_BUTTON_INDEX) {
        tapMode = false;
    }
}

void GhController::Destroy() {
    for (MidiNote midiNote : *heldNotes) {
        midiController->ReleaseNote(midiNote.pitch);
    }
}

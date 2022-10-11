#include "ChordController.h"

#include <algorithm>

ChordController::ChordController(std::vector<Input*>* inputs, Button *buttons[], const int midiPort) : inputs(inputs) {
    this->heldChordNotes = new std::vector<int>();
    this->heldHarpNotes = new std::vector<MidiNote>();
    this->midiController = new MidiController(midiPort);

    for (int i = 0; i < TOTAL_BUTTONS; i++) {
        this->buttons[i] = buttons[i];
    }

    for (int i = 0; i < CHORD_BUTTONS_COLS; i++) {
        buttons[i]->SetTopTitle(CHORD_TITLES[i]);
    }

    midiController->SetVolume(chordVolumeLevel);
    midiController->SetVolume(harpVolumeLevel, 1);
    buttons[CHORD_VOLUME_DOWN_BUTTON_INDEX]->SetBottomTitle(std::to_string(chordVolumeLevel));
    buttons[HARP_VOLUME_DOWN_BUTTON_INDEX]->SetBottomTitle(std::to_string(harpVolumeLevel));
}

void ChordController::Update() {
    InputData currentInput = (InputData) {0, (char) -1};
    for (Input* input : *inputs) {
        InputData inputData = input->Read();
        currentInput.buttonData |= inputData.buttonData;
        if (inputData.harpData != -1) {
            currentInput.harpData = inputData.harpData;
        }
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
    for (int i = heldHarpNotes->size()-1; i >= 0; i--) {
        if (heldHarpNotes->at(i).timestamp < current_time + HARP_NOTE_DURATION) {
            // printf("RELEASE NOTE: %d\n", heldHarpNotes->at(i));
            midiController->ReleaseNote(heldHarpNotes->at(i).pitch, 1);
            heldHarpNotes->erase(heldHarpNotes->begin() + i);
        }
    }

    if (currentInput.harpData != -1 && currentInput.harpData != 255) {
        HarpPressed(currentInput.harpData);
    }

    previousInput = currentInput;
}

void ChordController::ButtonPressed(ButtonIndex buttonIndex, const ButtonData currentInput) {
    if (buttonIndex < TOTAL_CHORD_BUTTONS) {
        int button_chord_index = buttonIndex % CHORD_BUTTONS_COLS;
        unsigned char chord_type = 0;

        for (int i = 0; i < CHORD_BUTTONS_ROWS; i++) {
            if (currentInput & (1uLL << (i*CHORD_BUTTONS_COLS + button_chord_index))) {
                chord_type += (1 << i);
            }
        }

        // Don't emit if another chord is being held
        // if (button_chord_index != chordIndex && heldNotes->size() > 0) {
        //     return;
        // }

        PlayChord(button_chord_index, chord_type);
    }
    else if (buttonIndex == MUTE_BUTTON_INDEX) {
        PlayChord(0, 0);
        harpChordType = 0;
    }
    else if (buttonIndex == CHORD_OCTAVE_UP_BUTTON_INDEX) {
        if (octave_offset < 3) {
            octave_offset += 1;
            buttons[CHORD_OCTAVE_DOWN_BUTTON_INDEX]->SetBottomTitle(std::to_string(octave_offset));
        }
    }
    else if (buttonIndex == CHORD_OCTAVE_DOWN_BUTTON_INDEX) {
        if (octave_offset > -2) {
            octave_offset -= 1;
            buttons[CHORD_OCTAVE_DOWN_BUTTON_INDEX]->SetBottomTitle(std::to_string(octave_offset));
        }
    }
    else if (buttonIndex == CHORD_VOLUME_UP_BUTTON_INDEX) {
        if (chordVolumeLevel < 127) {
            chordVolumeLevel += 10;
            if (chordVolumeLevel > 127) chordVolumeLevel = 127;
            midiController->SetVolume(chordVolumeLevel);
            buttons[CHORD_VOLUME_DOWN_BUTTON_INDEX]->SetBottomTitle(std::to_string(chordVolumeLevel));
        }
    }
    else if (buttonIndex == CHORD_VOLUME_DOWN_BUTTON_INDEX) {
        if (chordVolumeLevel > 0) {
            chordVolumeLevel -= 10;
            if (chordVolumeLevel < 0) chordVolumeLevel = 0;
            midiController->SetVolume(chordVolumeLevel);
            buttons[CHORD_VOLUME_DOWN_BUTTON_INDEX]->SetBottomTitle(std::to_string(chordVolumeLevel));
        }
    }
    else if (buttonIndex == HARP_VOLUME_UP_BUTTON_INDEX) {
        if (harpVolumeLevel < 127) {
            harpVolumeLevel += 10;
            if (harpVolumeLevel > 127) harpVolumeLevel = 127;
            midiController->SetVolume(harpVolumeLevel, 1);
            buttons[HARP_VOLUME_DOWN_BUTTON_INDEX]->SetBottomTitle(std::to_string(harpVolumeLevel));
        }
    }
    else if (buttonIndex == HARP_VOLUME_DOWN_BUTTON_INDEX) {
        if (harpVolumeLevel > 0) {
            harpVolumeLevel -= 10;
            if (harpVolumeLevel < 0) harpVolumeLevel = 0;
            midiController->SetVolume(harpVolumeLevel, 1);
            buttons[HARP_VOLUME_DOWN_BUTTON_INDEX]->SetBottomTitle(std::to_string(harpVolumeLevel));
        }
    }
    else if (buttonIndex == HOLD_CHORD_BUTTON_INDEX) {
        hold_chord_mode = !hold_chord_mode;
        if (hold_chord_mode) {
            buttons[HOLD_CHORD_BUTTON_INDEX]->SetLight(true);
        }
        else {
            buttons[HOLD_CHORD_BUTTON_INDEX]->SetLight(false);
        }
    }
}

void ChordController::ButtonReleased(ButtonIndex buttonIndex, const ButtonData currentInput) {
    if (buttonIndex < TOTAL_CHORD_BUTTONS && !hold_chord_mode) {
        int button_chord_index = buttonIndex % CHORD_BUTTONS_COLS;
        unsigned char chord_type = 0;

        for (int i = 0; i < CHORD_BUTTONS_ROWS; i++) {
            if (currentInput & (1uLL << (i*CHORD_BUTTONS_COLS + button_chord_index))) {
                chord_type += (1 << i);
            }
        }

        // Don't release if another chord is being held
        if (button_chord_index != chordIndex) {
            return;
        }

        PlayChord(button_chord_index, chord_type);
    }
}

void ChordController::HarpPressed(const HarpData currentInput) {
    // printf("HARP! %d\n", chordIndex);
    if (harpChordType == 0) return;

    unsigned char chordNotes = CHORD_NOTES;
    if (CHORD_MAP[harpChordType*CHORD_NOTES + CHORD_NOTES-1] == -1) {
        chordNotes = CHORD_NOTES-1;
    }

    unsigned char harpNotes = chordNotes * 3 + 1; // 3 octaves plus high note
    char harpNote = (float)(currentInput * harpNotes/* + 0.5*/) / HARP_RANGE;
    char previousHarpNote = (float)(previousInput.harpData * harpNotes/* + 0.5*/) / HARP_RANGE;

    // printf("HARP NOTE: %d\n", harpNote);
    if (previousHarpNote != -1 && harpNote == previousHarpNote) return;

    unsigned char harpOctave = 0;
    while (harpNote >= chordNotes) {
        harpOctave += 1;
        harpNote -= chordNotes;
    }
    int midiPitch = MIDI_MAP[chordIndex] + CHORD_MAP[harpChordType*CHORD_NOTES + harpNote] + harpOctave*12;
    int timestamp = SDL_GetTicks();
    midiController->PlayNote(midiPitch, 1);
    heldHarpNotes->push_back((MidiNote) {midiPitch, timestamp});
    // printf("MIDI PITCH: %d\n", midiPitch);
}

void ChordController::PlayChord(int chordIndex, unsigned char chordType) {
    // Get chord notes
    // printf("PLAY CHORD: %d, %x\n", chordIndex, chordType);
    this->chordIndex = chordIndex;
    if (chordType != 0) this->harpChordType = chordType;
    std::vector<int> chordNotes;
    for (int i = 0; i < CHORD_NOTES; i++) {
        int chord_offset = CHORD_MAP[chordType*CHORD_NOTES + i];
        if (chord_offset >= 0) {
            int midiPitch = MIDI_MAP[chordIndex] + chord_offset + octave_offset*12;
            chordNotes.push_back(midiPitch);
        }
    }

    // Release notes not in chord
    for (int i = heldChordNotes->size()-1; i >= 0; i--) {
        if (std::find(chordNotes.begin(), chordNotes.end(), heldChordNotes->at(i)) == chordNotes.end()) {
            // printf("RELEASE NOTE: %d\n", heldChordNotes->at(i));
            midiController->ReleaseNote(heldChordNotes->at(i));
            heldChordNotes->erase(heldChordNotes->begin() + i);
        }
    }

    // Play notes not already being played
    for (int chord_note : chordNotes) {
        if (std::find(heldChordNotes->begin(), heldChordNotes->end(), chord_note) == heldChordNotes->end()) {
            // printf("PLAY NOTE: %d\n", chord_note);
            midiController->PlayNote(chord_note);
            heldChordNotes->push_back(chord_note);
        }
    }
    // printf("HELD NOTES: ");
    // for (int note : *heldChordNotes) {
    //     printf("%d, ", note);
    // }
    // printf("\n");
}

void ChordController::Destroy() {
    for (int note : *heldChordNotes) {
        midiController->ReleaseNote(note);
    }
    for (MidiNote midiNote : *heldHarpNotes) {
        midiController->ReleaseNote(midiNote.pitch, 1);
    }
}

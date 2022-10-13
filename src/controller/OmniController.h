#ifndef OMNI_OMNICONTROLLER_H
#define OMNI_OMNICONTROLLER_H

#include <vector>

#include "Controller.h"

#include "../input/Input.h"
#include "../UI/Button.h"
#include "MidiControl.h"

constexpr int MIDI_MAP[] {
//  Db  Ab  Eb  Bb  F   C   G   D   A   E   B   F#
    49, 56, 51, 58, 53, 48, 55, 50, 57, 52, 59, 54
};

constexpr const char* CHORD_TITLES[] {
    "Db", "Ab", "Eb", "Bb", "F", "C", "G", "D", "A", "E", "B", "F#"
};

constexpr int CHORD_MAP[] {
    // 000 None
    -1, -1, -1, -1,
    // 001 Major
    0, 4, 7, -1,
    // 010 Minor
    0, 3, 7, -1,
    // 011 Augmented
    0, 4, 8, -1,
    // 100 Dominant 7
    0, 4, 7, 10,
    // 101 Major 7
    0, 4, 7, 11,
    // 110 Minor 7
    0, 3, 7, 10,
    // 111 Diminished
    0, 3, 6, -1
};
constexpr int CHORD_TYPES = 8;
constexpr int CHORD_NOTES = 4;

constexpr int HARP_NOTE_DURATION = 60;

class OmniController : public Controller {
public:
    OmniController(std::vector<Input*>* inputs, Button *buttons[], const int midiPort);
    virtual void Update() override;
    virtual void Destroy() override;
private:
    void ButtonPressed(ButtonIndex buttonIndex, const ButtonData currentInput);
    void ButtonReleased(ButtonIndex buttonIndex, const ButtonData currentInput);
    void HarpPressed(const HarpData currentInput);
    void PlayChord(int chordIndex, unsigned char chordType);

    std::vector<Input*>* inputs;
    InputData previousInput = (InputData) {0, (char) -1};
    std::vector<int>* heldChordNotes;
    std::vector<MidiNote>* heldHarpNotes;
    int chordIndex = 0;
    unsigned char harpChordType = 0;
    int octave_offset = 0;
    int chordVolumeLevel = 100;
    int harpVolumeLevel = 100;
    MidiControl* midiController;
    Button *buttons[TOTAL_BUTTONS];
    bool hold_chord_mode = false;
};

#endif

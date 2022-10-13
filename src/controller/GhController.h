#ifndef OMNI_GHCONTROLLER_H
#define OMNI_GHCONTROLLER_H

#include <vector>

#include "Controller.h"

#include "../input/Input.h"
#include "../UI/Button.h"
#include "MidiControl.h"

constexpr int GH_BASE_FRET_PITCH = 36;
constexpr int GH_HOPO_PITCH = 48;
constexpr int GH_TAP_PITCH = 49;
constexpr int GH_STAR_PITCH = 50;

constexpr int GH_TEMP_NOTE_DURATION = 60;

class GhController : public Controller {
public:
    GhController(std::vector<Input*>* inputs, Button *buttons[], const int midiPort);
    virtual void Update() override;
    virtual void Destroy() override;
private:
    void ButtonPressed(ButtonIndex buttonIndex, const ButtonData currentInput);
    void ButtonReleased(ButtonIndex buttonIndex, const ButtonData currentInput);

    std::vector<Input*>* inputs;
    InputData previousInput = (InputData) {0, (char) -1};
    MidiControl* midiController;
    Button *buttons[TOTAL_BUTTONS];
    std::vector<MidiNote>* heldNotes;
    bool holdMode;
    bool hopoMode;
    bool tapMode;
};

#endif

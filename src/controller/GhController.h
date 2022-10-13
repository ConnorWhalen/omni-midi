#ifndef OMNI_GHCONTROLLER_H
#define OMNI_GHCONTROLLER_H

#include <vector>

#include "Controller.h"

#include "../input/Input.h"
#include "../UI/Button.h"
#include "MidiControl.h"

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
    std::vector<MidiNote>* heldNotes;
    MidiControl* midiController;
    Button *buttons[TOTAL_BUTTONS];
};

#endif

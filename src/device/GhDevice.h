#ifndef OMNI_GHDEVICE_H
#define OMNI_GHDEVICE_H

#include <string>

#include <SDL2/SDL.h>

#include "Device.h"
#include "../input/TouchInput.h"
#include "../UI/Window.h"

class GhDevice : public Device {
public:
    GhDevice(Window* window, std::vector<Input*>* inputs, TouchInput &touchInput, const int midiPort, const std::string fontFile);
    virtual void Destroy() override;
};

#endif

#ifndef OMNI_DEVICE_H
#define OMNI_DEVICE_H

#include "../controller/Controller.h"

class Device {
public:
    virtual void Update() { controller->Update(); };
    virtual void Destroy() = 0;
protected:
    Controller* controller;
};

#endif

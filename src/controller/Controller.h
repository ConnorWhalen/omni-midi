#ifndef OMNI_CONTROLLER_H
#define OMNI_CONTROLLER_H

class Controller {
public:
    virtual void Update() = 0;
    virtual void Destroy() = 0;
};

#endif

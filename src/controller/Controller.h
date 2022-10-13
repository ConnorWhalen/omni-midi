#ifndef OMNI_CONTROLLER_H
#define OMNI_CONTROLLER_H

struct MidiNote {
    int pitch;
    int timestamp;
};

inline bool operator==(const MidiNote& lhs, const MidiNote& rhs) { return lhs.pitch == rhs.pitch; }

class Controller {
public:
    virtual void Update() = 0;
    virtual void Destroy() = 0;
};

#endif

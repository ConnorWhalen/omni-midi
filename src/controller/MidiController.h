#ifndef OMNI_MIDICONTROLLER_H
#define OMNI_MIDICONTROLLER_H

#include "../../rtmidi-5.0.0/RtMidi.h"

constexpr unsigned char MIDI_NOTE_ON = 0x90;
constexpr unsigned char MIDI_NOTE_OFF = 0x80;
constexpr unsigned char MIDI_CONTROL_EVENT = 0xB0;
constexpr unsigned char MIDI_CONTROL_VOLUME = 0x07;
constexpr unsigned char MIDI_VELOCITY = 0x40;

class MidiController {
public:
    MidiController(const int port);
    void PlayNote(unsigned char pitch, unsigned char channel = 0);
    void ReleaseNote(unsigned char pitch, unsigned char channel = 0);
    void SetVolume(unsigned char level, unsigned char channel = 0);
private:
    RtMidiOut* midi_out;
};

#endif

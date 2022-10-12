#include "MidiControl.h"

#include <vector>

MidiControl::MidiControl(const int port) {
    midi_out = new RtMidiOut();
    midi_out->openPort(port);
}

void MidiControl::PlayNote(unsigned char pitch, unsigned char channel) {
    std::vector<unsigned char> message;
    message.push_back(MIDI_NOTE_ON + channel);
    message.push_back(pitch);
    message.push_back(MIDI_VELOCITY);
    midi_out->sendMessage(&message);
}

void MidiControl::ReleaseNote(unsigned char pitch, unsigned char channel) {
    std::vector<unsigned char> message;
    message.push_back(MIDI_NOTE_OFF + channel);
    message.push_back(pitch);
    message.push_back(MIDI_VELOCITY);
    midi_out->sendMessage(&message);
}

void MidiControl::SetVolume(unsigned char level, unsigned char channel) {
    std::vector<unsigned char> message;
    message.push_back(MIDI_CONTROL_EVENT + channel);
    message.push_back(MIDI_CONTROL_VOLUME);
    message.push_back(level);
    midi_out->sendMessage(&message);
}
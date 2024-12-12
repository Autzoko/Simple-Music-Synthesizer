#ifndef MIDI_HANDLER_H
#define MIDI_HANDLER_H

#include <RtMidi.h>
#include <functional>
#include <memory>

class MIDIHandler {
public:
    MIDIHandler();
    ~MIDIHandler();

    void start();
    void stop();

    void setNoteOnCallback(const std::function<void(int, double)>& callback);
    void setNoteOffCallback(const std::function<void(int)>& callback);

private:
    void processMidiMessage(double deltaTime, std::vector<unsigned char>* message);

    std::unique_ptr<RtMidiIn> midiIn;
    std::function<void(int, double)> noteOnCallback;
    std::function<void(int)> noteOffCallback;
};

#endif
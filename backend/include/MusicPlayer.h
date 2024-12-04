#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "Note.h"
#include <vector>
#include <mutex>
#include <functional>

class MusicPlayer {
public:
    MusicPlayer(unsigned int sampleRate = 44100, unsigned int polyphony = 16);

    void noteOn(double frequency, double duration);
    void noteOff(double frequency);
    void generateAudio(std::vector<double>& outputBuffer, unsigned int numFrames);

    void setSynthesizer(std::function<Synthesizer*()> factory);

private:
    unsigned int sampleRate;
    unsigned int maxPolyphony;
    std::vector<Note> activeNotes;
    std::mutex noteMutex;
    std::function<Synthesizer*()> synthesizerFactory;
};

#endif
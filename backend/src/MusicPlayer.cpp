#include "MusicPlayer.h"
#include <algorithm>

MusicPlayer::MusicPlayer(unsigned int sampleRate, unsigned int polyphony)
    : sampleRate(sampleRate), maxPolyphony(polyphony), synthesizerFactory(nullptr) {}

void MusicPlayer::setSynthesizer(std::function<Synthesizer*()> factory) {
    synthesizerFactory = factory;
} 

void MusicPlayer::noteOn(double frequency, double duration) {
    std::lock_guard<std::mutex> lock(noteMutex);

    if (activeNotes.size() >= maxPolyphony) {
        activeNotes.erase(activeNotes.begin());
    }

    if (synthesizerFactory) {
        Synthesizer* synth = synthesizerFactory();
        activeNotes.emplace_back(frequency, 0.0, duration, synth);
    }
}

void MusicPlayer::noteOff(double frequency) {
    std::lock_guard<std::mutex> lock(noteMutex);

    for (auto& note : activeNotes) {
        if (note.frequency == frequency && note.active) {
            note.active = false;
            break;
        }
    }
}

#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include "portaudio.h"
#include "Synthesizer.h"
#include <memory>
#include <vector>

class AudioOutput {
public:
    AudioOutput(Synthesizer& synth, unsigned int sampleRate = 44100, unsigned int bufferSize = 512);
    ~AudioOutput();

    void start();
    void stop();

private:
    Synthesizer& synthesizer;
    unsigned int sampleRate;
    unsigned int bufferSize;
    PaStream* stream;

    static int audioCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
};

#endif
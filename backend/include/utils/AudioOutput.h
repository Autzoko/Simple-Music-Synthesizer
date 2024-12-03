#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <vector>
#include "RtAudio.h"

class AudioOutput {
public:
    AudioOutput(unsigned int sampleRate = 44100, unsigned int bufferFrames = 256);
    ~AudioOutput();

    void play(const std::vector<double>& data);
    void stop();

private:
    RtAudio dac;
    unsigned int sampleRate;
    unsigned int bufferFrames;
    std::vector<double> buffer;
    unsigned int bufferPosition;
    bool isPlaying;

    static int audioCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime,
        RtAudioStreamStatus status, void* userData);
};

#endif
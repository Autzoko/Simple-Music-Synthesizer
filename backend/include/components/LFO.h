#ifndef LFO_H
#define LFO_H

#include "Oscillator.h"

class LFO : public Oscillator {
public:
    LFO(WaveformType waveform = WaveformType::SINE, double frequency = 5.0, double amplitude = 1.0, unsigned int sampleRate=44100);
};

#endif //LFO_H
#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <vector>
#include "Oscillator.h"
#include "EnvelopeGenerator.h"
#include "Filter.h"
#include "Delay.h"
#include "Mixer.h"

class Synthesizer {
public:
    Synthesizer(unsigned int sampleRate = 44100);

    std::vector<double> noteOn(double frequency, double duration);
    void addOscillator(Oscillator* oscillator);
    void setEnvelope(EnvelopeGenerator* envelope);
    void setFilter(Filter* filter);
    void addEffect(DelayEffect* effect);

private:
    unsigned int sampleRate;
    std::vector<Oscillator*> oscillators;
    EnvelopeGenerator* envelope;
    Filter* filter;
    std::vector<DelayEffect*> effects;
    Mixer mixer;
};

#endif
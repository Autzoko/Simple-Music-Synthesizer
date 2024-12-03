#ifndef REVERB_H
#define REVERB_H

#include <vector>

class ReverbEffect {
public:
    ReverbEffect(double decay = 0.5, double mix = 0.5, unsigned int sampleRate = 44100);

    void apply(std::vector<double>& samples);

    void setDecay(double decay);
    void setMix(double mix);

private:
    unsigned int sampleRate;
    double decay;
    double mix;
    std::vector<double> delayBuffer;
    size_t delayIndex = 0;
};

#endif
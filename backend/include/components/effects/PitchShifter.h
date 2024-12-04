#ifndef PITCH_SHIFTER_H
#define PITCH_SHIFTER_H

#include <vector>
#include <cmath>
#include "Effect.h"

class PitchShifter : public Effect{
public:
    PitchShifter(unsigned int sampleRate, double pitchFactor = 1.0);

    void apply(std::vector<double>& samples) override;

    void setPitchFactor(double pitchFactor);

private:
    unsigned int sampleRate;
    double pitchFactor;
    std::vector<double> delayBuffer;
    size_t writeIndex = 0;
};

#endif
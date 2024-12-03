#ifndef CHORUS_H
#define CHORUS_H

#include "Effect.h"
#include <vector>
#include <cmath>

class ChorusEffect : public Effct {
public:
    ChorusEffect(double depth = 0.002, double rate = 1.5, unsigned int sampleRate = 44100);

    void apply(std::vector<double>& samples) override;

    void setDepth(double depth);
    void setRate(double rate);

private:
    unsigned int sampleRate;
    double depth;
    double rate;
    double lfoPhase = 0.0;
    std::vector<double> delayBuffer;
    size_t delayIndex = 0;
};

#endif //CHORUS_H
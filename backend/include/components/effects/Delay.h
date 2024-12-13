#ifndef DELAY_H
#define DELAY_H

#include "Effect.h"
#include <vector>

class DelayEffect : public Effect {
public:
    DelayEffect(double delayTime = 0.5, double feedback = 0.5, double mix = 0.5, unsigned int sampleRate = 44100);
    void apply(std::vector<double>& data) override;
    void setDelayTime(double delayTime);
    void setFeedback(double feedback);
    void setMix(double mix);

private:
    double delayTime;
    double feedback;
    double mix;
    unsigned int sampleRate;
    std::vector<double> delayBuffer;
    unsigned int delaySamples;
    unsigned int writeIndex;
};

#endif //DELAY_H
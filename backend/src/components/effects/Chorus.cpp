#include "Chorus.h"

ChorusEffect::ChorusEffect(double depth, double rate, unsigned int sampleRate)
    : depth(depth), rate(rate), sampleRate(sampleRate) {
        delayBuffer.resize(sampleRate, 0.0);
}

void ChorusEffect::apply(std::vector<double>& samples) {
    for (size_t i = 0; i < samples.size(); ++i) {
        double modDelay = depth * sampleRate * (0.5 + 0.5 * sin(2.0 * M_PI * lfoPhase));
        size_t _delayIndex = (this->delayIndex + delayBuffer.size() - static_cast<size_t>(modDelay)) % delayBuffer.size();

        double delayedSample = delayBuffer[_delayIndex];
        delayBuffer[this->delayIndex] = samples[i];
        samples[i] += delayedSample * 0.5;

        this->delayIndex = (this->delayIndex + 1) % delayBuffer.size();
        lfoPhase += rate / sampleRate;
        if (lfoPhase >= 1.0) lfoPhase -= 1.0;
    }
}

void ChorusEffect::setDepth(double depth) {
    this->depth = depth;
}

void ChorusEffect::setRate(double rate) {
    this->rate = rate;
}
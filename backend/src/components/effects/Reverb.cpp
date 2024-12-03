#include "Reverb.h"

ReverbEffect::ReverbEffect(double decay, double mix, unsigned int sampleRate)
    : decay(decay), mix(mix), sampleRate(sampleRate) {
        delayBuffer.resize(sampleRate * 2, 0.0);
}

void ReverbEffect::apply(std::vector<double>& samples) {
    for (size_t i = 0; i < samples.size(); ++i) {
        double delayedSample = delayBuffer[delayIndex];
        delayBuffer[delayIndex] = samples[i] + decay * delayedSample;
        samples[i] = (1.0 - mix) * samples[i] + mix * delayedSample;

        delayIndex = (delayIndex + 1) % delayBuffer.size();
    }
}

void ReverbEffect::setDecay(double decay) {
    this->decay = decay;
}

void ReverbEffect::setMix(double mix) {
    this->mix = mix;
}
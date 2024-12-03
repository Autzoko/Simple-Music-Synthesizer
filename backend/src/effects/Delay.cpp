#include "Delay.h"

DelayEffect::DelayEffect(double delayTime, double feedback, double mix, unsigned int sampleRate)
    : delayTime(delayTime), feedback(feedback), mix(mix), sampleRate(sampleRate), writeIndex(0) {
        delaySamples = static_cast<unsigned int>(delayTime * sampleRate);
        delayBuffer.resize(delaySamples, 0.0);
}

void DelayEffect::setParameters(double delayTime, double feedback, double mix) {
    this->delayTime = delayTime;
    this->feedback = feedback;
    this->mix = mix;
    delaySamples = static_cast<unsigned int>(delayTime * sampleRate);
    delayBuffer.resize(delaySamples, 0.0);
}

void DelayEffect::apply(std::vector<double>& data) {
    for (auto& sample : data) {
        double delayedSample = delayBuffer[writeIndex];
        double newSample = sample + delayedSample * feedback;
        delayBuffer[writeIndex] = newSample;
        sample = sample * (1.0 - mix) + delayedSample * mix;
        writeIndex = (writeIndex + 1) % delaySamples;
    }
}
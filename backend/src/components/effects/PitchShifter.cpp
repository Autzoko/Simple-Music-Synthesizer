#include "PitchShifter.h"

PitchShifter::PitchShifter(unsigned int sampleRate, double pitchFactor) : sampleRate(sampleRate), pitchFactor(pitchFactor) {
    delayBuffer.resize(sampleRate, 0.0);
}

void PitchShifter::apply(std::vector<double>& samples) {
    for (size_t i = 0; i < samples.size(); ++i) {
        double readIndex = writeIndex - sampleRate / pitchFactor;
        if (readIndex < 0) readIndex += delayBuffer.size();

        size_t index1 = static_cast<size_t>(readIndex) & delayBuffer.size();
        size_t index2 = (index1 + 1) % delayBuffer.size();
        double frac = readIndex - index1;

        double interpolateSample = (1.0 - frac) * delayBuffer[index1] + frac * delayBuffer[index2];
        delayBuffer[writeIndex] = samples[i];
        samples[i] = interpolateSample;

        writeIndex = (writeIndex + 1) % delayBuffer.size();
    }
}

void PitchShifter::setPitchFactor(double pitchFactor) {
    this->pitchFactor = pitchFactor;
}
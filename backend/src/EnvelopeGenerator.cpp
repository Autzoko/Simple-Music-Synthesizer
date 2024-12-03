#include "EnvelopeGenerator.h"

EnvelopeGenerator::EnvelopeGenerator(double attack, double decay, double sustain, double release, unsigned int sampleRate)
    : attack(attack), decay(decay), sustain(sustain), release(release), sampleRate(sampleRate) {}

std::vector<double> EnvelopeGenerator::generate(double duration) {
    unsigned int totalSamples = static_cast<unsigned int>(sampleRate * duration);
    std::vector<double> envelope(totalSamples, 0.0);

    unsigned int attackSamples = static_cast<unsigned int>(attack * sampleRate);
    unsigned int decaySamples = static_cast<unsigned int>(decay * sampleRate);
    unsigned int releaseSamples = static_cast<unsigned int>(release * sampleRate);
    unsigned int sustainSamples = totalSamples - attackSamples - decaySamples - releaseSamples;

    if (sustainSamples < 0) {
        sustainSamples = 0;
    }

    for (unsigned int i = 0; i < attackSamples && i < totalSamples; ++i) {
        envelope[i] = static_cast<double>(i) / attackSamples;
    }
    for (unsigned int i = attackSamples; i < attackSamples + decaySamples && i < totalSamples; ++i) {
        envelope[i] = 1.0 - ((1.0 - sustain) * (static_cast<double>(i - attackSamples) / decaySamples));
    }
    for (unsigned int i = attackSamples + decaySamples; i < attackSamples + decaySamples + releaseSamples && i < totalSamples; ++i) {
        envelope[i] = sustain;
    }
    for (unsigned int i = totalSamples - releaseSamples; i < totalSamples; ++i) {
        envelope[i] = sustain * (1.0 - static_cast<double>(i - (totalSamples - releaseSamples)) / releaseSamples);
    }

    return envelope;
}

void EnvelopeGenerator::setADSR(double attack, double decay, double sustain, double release) {
    this->attack = attack;
    this->decay = decay;
    this->sustain = sustain;
    this->release = release;
}
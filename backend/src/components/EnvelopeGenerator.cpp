#include "EnvelopeGenerator.h"

EnvelopeGenerator::EnvelopeGenerator(double attack, double decay, double sustain, double release, unsigned int sampleRate)
    : attack(attack), decay(decay), sustain(sustain), release(release), sampleRate(sampleRate), state(State::Idle), currentLevel(0.0), currentSample(0) {
        calculateSegmentLength();
}

void EnvelopeGenerator::calculateSegmentLength() {
    attackSamples = static_cast<unsigned int>(attack * sampleRate);
    decaySamples = static_cast<unsigned int>(decay * sampleRate);
    releaseSamples = static_cast<unsigned int>(release * sampleRate);
}


void EnvelopeGenerator::setAttack(double attack) {
    this->attack = attack;
    calculateSegmentLength();
}

void EnvelopeGenerator::setDecay(double decay) {
    this->decay = decay;
    calculateSegmentLength();
}

void EnvelopeGenerator::setSustain(double sustain) {
    this->sustain = sustain;
    calculateSegmentLength();
}

void EnvelopeGenerator::setRelease(double release) {
    this->release = release;
    calculateSegmentLength();
}

void EnvelopeGenerator::trigger() {
    state = State::Attack;
    currentLevel = 0.0;
    currentSample = 0;
}

void EnvelopeGenerator::doRelease() {
    state = State::Release;
    currentSample = 0;
}

bool EnvelopeGenerator::isFinished() const {
    return state == State::Idle;
}

std::vector<double> EnvelopeGenerator::generate(double duration) {
    size_t numSamples = static_cast<size_t>(duration * sampleRate);
    std::vector<double> envelope(numSamples);

    for (size_t i = 0; i < numSamples; ++i) {
        switch (state) {
            case State::Idle:
                envelope[i] = 0.0;
                break;
            case State::Attack:
                currentLevel += 1.0 / attackSamples;
                if (currentSample++ >= attackSamples) {
                    currentLevel = 1.0;
                    state = State::Decay;
                    currentSample = 0;
                }
                envelope[i] = currentLevel;
                break;
            case State::Decay:
                currentLevel -= (1.0 - sustain) / decaySamples;
                if (currentSample++ >= decaySamples) {
                    currentLevel = sustain;
                    state = State::Sustain;
                }
                envelope[i] = currentLevel;
                break;
            case State::Sustain:
                envelope[i] = sustain;
                break;
            case State::Release:
                currentLevel -= sustain / releaseSamples;
                if (currentSample++ >= releaseSamples) {
                    currentLevel = 0.0;
                    state = State::Idle;
                }
                envelope[i] = std::max(currentLevel, 0.0);
                break;
        }
    }
    return envelope;
}

#ifndef ENVELOPEGENERATOR_H
#define ENVELOPEGENERATOR_H

#include <vector>

class EnvelopeGenerator {
public:
    EnvelopeGenerator(double attack = 0.01, double decay = 0.1, double sustain = 0.7, double release = 0.1, unsigned int sampleRate = 44100);
    std::vector<double> generate(double duration);

    void setADSR(double attack, double decay, double sustain, double release);

    void trigger();
    void doRelease();
    bool isFinished() const;

private:
    double attack;
    double decay;
    double sustain;
    double release;
    unsigned int sampleRate;

    enum class State {Idle, Attack, Decay, Sustain, Release} state;
    double currentLevel;
    unsigned int currentSample;
    unsigned int attackSamples;
    unsigned int decaySamples;
    unsigned int releaseSamples;

    void calculateSegmentLength();
};

#endif //ENVELOPEGENERATOR_H
#include "Tremolo.h"

TremoloEffect::TremoloEffect(double depth, double rate, unsigned int sampleRate)
    : depth(depth), rate(rate), sampleRate(sampleRate) {}


void TremoloEffect::apply(std::vector<double>& samples) {
    for (size_t i = 0; i < samples.size(); ++i) {
        double modulation = 1.0 - depth * 0.5 * (1.0 + sin(2.0 * M_PI * lfoPhase));
        samples[i] *= modulation;

        lfoPhase += rate / sampleRate;
        if (lfoPhase >= 1.0) lfoPhase -= 1.0;
    }
}

void TremoloEffect::setDepth(double depth) {
    this->depth = depth;
}

void TremoloEffect::setRate(double rate) {
    this->rate = rate;
}
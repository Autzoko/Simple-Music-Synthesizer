#include "Synthesizer.h"

Synthesizer::Synthesizer(unsigned int sampleRate) : sampleRate(sampleRate), envelope(nullptr), filter(nullptr) {}

void Synthesizer::addOscillator(Oscillator* oscillator) {
    oscillators.push_back(oscillator);
}

void Synthesizer::setEnvelope(EnvelopeGenerator* envelope) {
    this->envelope = envelope;
}

void Synthesizer::setFilter(Filter* filter) {
    this->filter = filter;
}

void Synthesizer::addEffect(DelayEffect* effect) {
    effects.push_back(effect);
}

std::vector<double> Synthesizer::noteOn(double frequency, double duration) {
    std::vector<std::vector<double>> signals;

    for (auto osc : oscillators) {
        osc->setFrequency(frequency);
        signals.push_back(osc->generate(duration));
    }

    for (const auto& signal : signals) {
        mixer.addChannel(signal);
    }
    std::vector<double> mixedSignal = mixer.mix();

    if (envelope) {
        std::vector<double> env = envelope->generate(duration);
        for (size_t i = 0; i < mixedSignal.size(); ++i) {
            mixedSignal[i] *= env[i];
        }
    }

    if (filter) {
        filter->apply(mixedSignal);
    }

    for (auto effect : effects) {
        effect->apply(mixedSignal);
    }

    return mixedSignal;
}
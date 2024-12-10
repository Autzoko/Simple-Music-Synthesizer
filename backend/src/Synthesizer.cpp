#include "Synthesizer.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>

Synthesizer::Voice::Voice(unsigned int numOscillators, std::shared_ptr<EnvelopeGenerator> env, unsigned int sampleRate)
    : envelope(std::move(env)), active(false), mainFrequency(0.0) {
        for (unsigned int i = 0; i < numOscillators; ++i) {
            oscillators.emplace_back(WaveformType::SINE, 440.0, 1.0, 1.0, 0.0);
        }
}

Synthesizer::Synthesizer(unsigned int sampleRate) : sampleRate(sampleRate), filter(nullptr) {
    for (unsigned int i = 0; i < 128; ++i) {
        voices.emplace_back(4, std::make_shared<EnvelopeGenerator>(0.01, 0.1, 0.7, 0.2, sampleRate), sampleRate);
    }
}

void Synthesizer::assignFrequencies(Voice& voice, const Note& note) {
    size_t numOscillators = voice.oscillators.size();

    for (size_t i = 0; i < numOscillators; ++i) {
        double frequency = (i == 0) ? note.getMainFrequency() : note.getSubFrequencies(i - 1) * note.getMainFrequency() * (1.0 + voice.oscillators[i].detune);
        voice.oscillators[i].oscillator->setFrequency(frequency);
    }
}

void Synthesizer::noteOn(const Note& note, double velocity) {
    std::lock_guard<std::mutex> lock(synthMutex);

    for (auto& voice : voices) {
        if (!voice.active) {
            assignFrequencies(voice, note);
            for (auto& oscConfig : voice.oscillators) {
                oscConfig.oscillator->setAmplitude(velocity * oscConfig.weight);
            }
            voice.envelope->trigger();
            voice.mainFrequency = note.getMainFrequency();
            voice.active = true;
            return;
        }
    }
}

void Synthesizer::noteOff(const Note& note) {
    std::lock_guard<std::mutex> lock(synthMutex);

    for (auto& voice : voices) {
        if (voice.active && voice.mainFrequency == note.getMainFrequency()) {
            voice.envelope->doRelease();
            return;
        }
    }
}

void Synthesizer::generateAudio(std::vector<double>& outputBuffer, unsigned int numFrames) {
    std::lock_guard<std::mutex> lock(synthMutex);

    outputBuffer.assign(numFrames, 0.0);

    for (auto& voice : voices) {
        if (!voice.active) continue;

        std::vector<double> voiceBuffer(numFrames, 0.0);
        std::vector<double> envelopeSamples = voice.envelope->generate(static_cast<double>(numFrames) / sampleRate);

        for (auto& oscConfig : voice.oscillators) {
            std::vector<double> oscSamples = oscConfig.oscillator->generate(numFrames);
            for (size_t i = 0; i < numFrames; ++i) {
                voiceBuffer[i] += oscSamples[i] * oscConfig.weight * envelopeSamples[i];
                //printf("%f : %f\n", oscSamples[i], envelopeSamples[i]);
            }
        }
        
        for (size_t i = 0; i < numFrames; ++i) {
            outputBuffer[i] += voiceBuffer[i];
        }

        if (voice.envelope->isFinished()) voice.active = false;
    }

    for (auto& effect : effects) {
        effect->apply(outputBuffer);
    }

    if (filter) {
        filter->apply(outputBuffer);
    }
}

void Synthesizer::setOscillatorWaveform(size_t oscillatorIndex, WaveformType waveform) {
    std::lock_guard<std::mutex> lock(synthMutex);

    for (auto& voice : voices) {
        voice.oscillators[oscillatorIndex].oscillator->setWaveform(waveform);
    }
}

void Synthesizer::setOscillatorWeight(size_t oscillatorIndex, double weight) {
    std::lock_guard<std::mutex> lock(synthMutex);

    for (auto& voice : voices) {
        voice.oscillators[oscillatorIndex].weight = weight;
    }
}

void Synthesizer::setOscillatorDetune(size_t oscillatorIndex, double detune) {
    std::lock_guard<std::mutex> lock(synthMutex);

    for (auto& voice : voices) {
        voice.oscillators[oscillatorIndex].detune = detune;
    }
}

Oscillator& Synthesizer::getOscillator(size_t oscillatorIndex) {
    if (oscillatorIndex >= voices[0].oscillators.size()) {
        throw std::out_of_range("Invalid oscillator index");
    }
    return *voices[0].oscillators[oscillatorIndex].oscillator;
}

const Oscillator& Synthesizer::getOscillator(size_t oscillatorIndex) const {
    if (oscillatorIndex >= voices[0].oscillators.size()) {
        throw std::out_of_range("Invalid oscillator index");
    }
    return *voices[0].oscillators[oscillatorIndex].oscillator;
}

void Synthesizer::setEnvelope(std::shared_ptr<EnvelopeGenerator> envelope) {
    for (auto& voice : voices) {
        voice.envelope = envelope;
    }
}

std::shared_ptr<EnvelopeGenerator> Synthesizer::getEnvelope() {
    if (!voices.empty()) {
        return voices[0].envelope;
    }
    return nullptr;
}

void Synthesizer::setFilter(Filter* filter) {
    this->filter = filter;
}

Filter* Synthesizer::getFilter() {
    return filter;
}

void Synthesizer::addEffect(std::shared_ptr<Effect> effect) {
    effects.push_back(effect);
}

void Synthesizer::removeEffect(size_t index) {
    std::lock_guard<std::mutex> lock(synthMutex);
    if (index < effects.size()) {
        effects.erase(effects.begin() + index);
    }
}

std::shared_ptr<Effect> Synthesizer::getEffect(size_t index) {
    if (index < effects.size()) {
        return effects[index];
    }
}
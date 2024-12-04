#include "Synthesizer.h"
#include <algorithm>
#include <cmath>
#include <cstring>

Synthesizer::Voice::Voice(unsigned int numOscillators, EnvelopeGenerator* env, unsigned int sampleRate)
    : envelope(env), active(false), mainFrequency(0.0) {
        for (unsigned int i = 0; i < numOscillators; ++i) {
            oscillators.emplace_back(WaveformType::SINE, 1.0, 440.0, 1.0, 0.0);
        }
}

Synthesizer::Voice::~Voice() {
    delete envelope;
}

Synthesizer::Synthesizer(unsigned int sampleRate) : sampleRate(sampleRate), filter(nullptr) {
    for (unsigned int i = 0; i < 128; ++i) {
        voices.emplace_back(4, new EnvelopeGenerator(), sampleRate);
    }
}

void Synthesizer::assignFrequencies(Voice& voice, const Note& note) {
    size_t numOscillators = voice.oscillators.size();
    size_t numSubFrequencies = note.getSubFrequencies().size();

    for (size_t i = 0; i < numOscillators; ++i) {
        double frequency = (i == 0) ? note.getMainFrequency() : note.getSubFrequencies()[i - 1] * (1.0 + voice.oscillators[i].detune);
        voice.oscillators[i].oscillator.setFrequency(frequency);
    }
}

void Synthesizer::noteOn(const Note& note, double velocity) {
    std::lock_guard<std::mutex> lock(synthMutex);

    for (auto& voice : voices) {
        if (!voice.active) {
            assignFrequencies(voice, note);
            for (auto& oscConfig : voice.oscillators) {
                oscConfig.oscillator.setAmplitude(velocity * oscConfig.weight);
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
            auto oscSamples = oscConfig.oscillator.generate(static_cast<double>(numFrames) / sampleRate);
            for (size_t i = 0; i < numFrames; ++i) {
                voiceBuffer[i] += oscSamples[i] * oscConfig.weight * envelopeSamples[i];
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
        voice.oscillators[oscillatorIndex].oscillator.setWaveform(waveform);
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

void Synthesizer::setEnvelope(EnvelopeGenerator* envelope) {
    for (auto& voice : voices) {
        voice.envelope = envelope;
    }
}

void Synthesizer::setFilter(Filter* filter) {
    this->filter = filter;
}







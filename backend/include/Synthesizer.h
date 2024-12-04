#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include "Oscillator.h"
#include "EnvelopeGenerator.h"
#include "Filter.h"
#include "Effect.h"
#include "Note.h"

#include <vector>
#include <memory>
#include <mutex>

class Synthesizer {
public:
    Synthesizer(unsigned int sampleRate = 44100);

    void noteOn(const Note& note, double velocity = 1.0);
    void noteOff(const Note& note);

    void generateAudio(std::vector<double>& outputBuffer, unsigned int numFrames);

    void setOscillatorWaveform(size_t oscillatorIndex, WaveformType wavform);
    void setOscillatorWeight(size_t oscillatorIndex, double weight);
    void setOscillatorDetune(size_t oscillatorIndex, double detune);
    void setEnvelope(EnvelopeGenerator* envelope);
    void setFilter(Filter* filter);
    void addEffect(std::shared_ptr<Effect> effect);

private:
    struct Voice {
        struct OscillatorConfig {
            Oscillator oscillator;
            double weight;
            double detune;
            OscillatorConfig(WaveformType waveform, double amplitude, double frequency, double w = 1.0, double d = 0.0) : oscillator(oscillator), weight(w), detune(d) {}
        };

        std::vector<OscillatorConfig> oscillators;
        EnvelopeGenerator* envelope;
        bool active;
        double mainFrequency;

        Voice(unsigned int numOscillators, EnvelopeGenerator* env, unsigned int sampleRate);
        ~Voice();
    };

    unsigned int sampleRate;
    std::vector<Voice> voices;
    std::vector<std::shared_ptr<Effect>> effects;
    Filter* filter;
    std::mutex synthMutex;

    void assignFrequencies(Voice& voice, const Note& note);
};

#endif
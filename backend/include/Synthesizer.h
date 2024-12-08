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
    void setEnvelope(std::shared_ptr<EnvelopeGenerator> envelope);
    void setFilter(Filter* filter);
    void addEffect(std::shared_ptr<Effect> effect);

    Filter* getFilter();
    std::shared_ptr<Effect> getEffect(unsigned int index);



private:
    struct Voice {
        struct OscillatorConfig {
            std::shared_ptr<Oscillator> oscillator;
            double weight;
            double detune;
            OscillatorConfig(WaveformType waveform, double frequency, double amplitude, double w = 1.0, double d = 0.0, unsigned int sampleRate = 44100)
                : oscillator(std::make_shared<Oscillator>(waveform, frequency, amplitude, 0.0, sampleRate)),
                weight(w), detune(d) {}
        };

        std::vector<OscillatorConfig> oscillators;
        std::shared_ptr<EnvelopeGenerator> envelope;
        bool active;
        double mainFrequency;

        Voice(unsigned int numOscillators, std::shared_ptr<EnvelopeGenerator> env, unsigned int sampleRate);
    };

    unsigned int sampleRate;
    std::vector<Voice> voices;
    std::vector<std::shared_ptr<Effect>> effects;
    Filter* filter;
    std::mutex synthMutex;

    void assignFrequencies(Voice& voice, const Note& note);
};

class NewSynthesizer {
public:
    NewSynthesizer(const string noteMapFile, unsigned int sampleRate = 44100);
    ~NewSynthesizer();

    std::vector<Oscillator*> initializeOscillators();
    EnvelopeGenerator* initializeEnvelope();
    Filter* initializeFilter();
    void initializeEffectChain();

    void addEffect(Effect* effect);

    void noteOn(const Note& note, double velocity = 1.0);
    void noteOff(const Note& note);

    void generateAudio(std::vector<double>& outputBuffer, unsigned int numFrames);

private:
    unsigned int sampleRate;
    std::unordered_map<unsigned int, std::vector<double>> noteFrequencyMap;
    std::vector<Oscillator*> oscillators;
    EnvelopeGenerator* envelope;
    Filter* filter;
    std::vector<Effect*> effectChain;
    std::mutex synthMutex;

    void setFrequencyMap(); 
};

#endif
#include <iostream>
#include <vector>
#include "Synthesizer.h"
#include "AudioOutput.h"

int main() {
    unsigned int sampleRate = 44100;
    double duration = 2.0;

    Synthesizer synth(sampleRate);
    
    Oscillator* osc1 = new Oscillator(
        WaveformType::SAWTOOTH,
        440.0,
        1.0,
        0.0,
        sampleRate
    );
    synth.addOscillator(osc1);

    EnvelopeGenerator* env = new EnvelopeGenerator(
        0.01,
        0.1,
        0.8,
        0.5,
        sampleRate
    );
    synth.setEnvelope(env);

    Filter* filter = new Filter(
        FilterType::LOWPASS,
        1000.0,
        0.7071,
        sampleRate
    );
    synth.setFilter(filter);

    DelayEffect* delay = new DelayEffect(
        0.5,
        0.5,
        0.3
    );
    synth.addEffect(delay);

    std::vector<double> samples = synth.noteOn(440.0, duration);

    AudioOutput audioOutput(sampleRate);
    audioOutput.play(samples);

    delete osc1;
    delete env;
    delete filter;
    delete delay;

    return 0;
}
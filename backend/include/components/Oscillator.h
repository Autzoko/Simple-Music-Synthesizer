#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <vector>

enum class WaveformType {
    SINE,
    SQUARE,
    SAWTOOTH,
    TRIANGLE,
    NOISE
};

class Oscillator {
public:
    Oscillator(WaveformType waveform = WaveformType::SINE, double frequency = 440.0, double amplitude = 1.0, double phase = 0.0, unsigned int sampleRate = 44100);
    std::vector<double> generate(unsigned int numSamples);

    void setFrequency(double frequency);
    void setAmplitude(double amplitude);
    void setPhase(double phase);
    void setWaveform(WaveformType waveform);

private:
    WaveformType waveform;
    double frequency;
    double amplitude;
    double phase;
    unsigned int sampleRate;
};

#endif //OSCILLATOR_H
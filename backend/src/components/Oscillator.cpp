#include "Oscillator.h"
#include <cmath>
#include <random>

Oscillator::Oscillator(WaveformType waveform, double frequency, double amplitude, double phase, unsigned int sampleRate)
    : waveform(waveform), frequency(frequency), amplitude(amplitude), phase(phase), sampleRate(sampleRate) {}


std::vector<double> Oscillator::generate(unsigned int numSamples) {
    unsigned int totalSamples = numSamples;

    std::vector<double> wave(totalSamples);

    for (unsigned int i = 0; i < totalSamples; ++i) {
        double t = static_cast<double>(i) / sampleRate;
        double value = 0.0;

        switch (waveform) {
            case WaveformType::SINE:
                value = amplitude * sin(2.0 * M_PI * frequency * t + phase);
                break;
            
            case WaveformType::SQUARE:
                value = amplitude * (sin(2.0 * M_PI * frequency * t + phase) >= 0 ? 1.0 : -1.0);
                break;

            case WaveformType::SAWTOOTH:
                value = amplitude * (2.0 * (frequency * t - floor(0.5 + frequency * t)));
                break;

            case WaveformType::TRIANGLE:
                value = amplitude * (2.0 * fabs(2.0 * (frequency * t - floor(frequency * t + 0.5))) - 1.0);
                break;

            case WaveformType::NOISE:
                std::default_random_engine generator;
                std::uniform_real_distribution<double> distribution(-1.0, 1.0);
                value = amplitude * distribution(generator);
                break;
        }
        //printf("%f\n", value);
        wave[i] = value;
    }
    return wave;
}

void Oscillator::setFrequency(double frequency) {
    this->frequency = frequency;
}

void Oscillator::setAmplitude(double amplitude) {
    this->amplitude = amplitude;
}

void Oscillator::setPhase(double phase) {
    this->phase = phase;
}

void Oscillator::setWaveform(WaveformType waveform) {
    this->waveform = waveform;
}
#include "LFO.h"

LFO::LFO(WaveformType waveform, double frequency, double amplitude, unsigned int sampleRate)
    : Oscillator(waveform, frequency, amplitude, 0.0, sampleRate) {}
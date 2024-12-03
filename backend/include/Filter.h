#ifndef FILTER_H
#define FILTER_H

#include <vector>

enum class FilterType {
    LOWPASS,
    HIGHPASS,
    BANDPASS
};

class Filter {
public:
    Filter(FilterType type = FilterType::LOWPASS, double cutoff = 1000.0, double resonance = 1.0, unsigned int sampleRate = 44100);
    void apply(std::vector<double>& data);
    void setParameters(FilterType type, double cutoff, double resonance);

private:
    FilterType type;
    double cutoff;
    double resonance;
    unsigned int sampleRate;

    double a0, a1, a2, b1, b2;
    double z1, z2;

    void calculateCoefficients();
};

#endif //FILTER_H
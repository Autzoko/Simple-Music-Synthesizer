#include "Filter.h"
#include <cmath>

Filter::Filter(FilterType type, double cutoff, double resonance, unsigned int sampleRate)
    : type(type), cutoff(cutoff), resonance(resonance), sampleRate(sampleRate), z1(0.0), z2(0.0) {
        calculateCoefficients();
}


void Filter::setFilterType(FilterType type) {
    this->type = type;
    calculateCoefficients();
}

void Filter::setCutoff(double cutoff) {
    this->cutoff = cutoff;
    calculateCoefficients();
}

void Filter::setResonance(double resonance) {
    this->resonance = resonance;
    calculateCoefficients();
}

void Filter::calculateCoefficients() {
    double omega = 2.0 * M_PI * cutoff / sampleRate;
    double alpha = sin(omega) / (2.0 * resonance);

    switch (type) {
        case FilterType::LOWPASS:
            a0 = (1.0 - cos(omega)) / 2.0;
            a1 = 1.0 - cos(omega);
            a2 = a0;
            b1 = -2.0 * cos(omega);
            b2 = 1.0 - alpha;
            break;

        case FilterType::HIGHPASS:
            a0 = (1.0 + cos(omega)) / 2.0;
            a1 = -(1.0 + cos(omega));
            a2 = a0;
            b1 = -2.0 * cos(omega);
            b2 = 1.0 - alpha;
            break;

        case FilterType::BANDPASS:
            a0 = alpha;
            a1 = 0.0;
            a2 = -alpha;
            b1 = -2.0 * cos(omega);
            b2 = 1.0 - alpha;
            break;
    }

    double a_sum = 1.0 + alpha;
    a0 /= a_sum;
    a1 /= a_sum;
    a2 /= a_sum;
    b1 /= a_sum;
    b2 /= a_sum;
}

void Filter::apply(std::vector<double>& data) {
    for (auto& sample : data) {
        double out = a0 * sample + a1 * z1 + a2 * z2 - b1 * z1 - b2 * z2;
        z2 = z1;
        z1 = sample;
        sample = out;
    }
}
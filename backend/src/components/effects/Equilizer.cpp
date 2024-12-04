#include "Equilizer.h"
#include <cmath>

Equilizer::Equilizer(unsigned int sampleRate, const std::vector<Band>& bands) : sampleRate(sampleRate), bands(bands) {}

void Equilizer::apply(std::vector<double>& samples) {
    for (auto& sample : samples) {
        for (const auto& band : bands) {
            sample = applyBand(sample, band);
        }
    }
}

void Equilizer::setBandGain(size_t bandIndex, double gain) {
    if (bandIndex > bands.size()) return;
    bands[bandIndex].gain = gain;
}

double Equilizer::applyBand(double sample, const Band& band) {
    double omega = 2.0 * M_PI * band.frequency / sampleRate;
    double alpha = sin(omega) / (2.0 * band.bandwidth);
    double A = pow(10, band.gain / 40.0);

    double b0 = 1 + alpha * A;
    double b1 = -2 * cos(omega);
    double b2 = 1 - alpha * A;
    double a0 = 1 + alpha / A;
    double a1 = -2 * cos(omega);
    double a2 = 1 - alpha / A;

    static double x1 = 0, x2 = 0, y1 = 0, y2 = 0;

    double y0 = (b0 / a0) * sample + (b1 / a0) * x1 + (b2 / a0) * x2 - (a1 / a0) * y1 - (a2 / a0) * y2;

    x2 = x1;
    x1 = sample;
    y2 = y1;
    y1 = y0;

    return y0;
}
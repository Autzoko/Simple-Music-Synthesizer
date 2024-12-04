#ifndef EQUILIZER_H
#define EQUILIZER_H

#include "Effect.h"
#include <vector>

class Equilizer : public Effect {
public:
    struct Band {
        double frequency;
        double gain;
        double bandwidth;

        Band(double freq, double g = 0.0, double bw = 100.0) : frequency(freq), gain(g), bandwidth(bw) {}
    };

    Equilizer(unsigned int sampleRate, const std::vector<Band>& bands);

    void apply(std::vector<double>& samples) override;

    void setBandGain(size_t bandIndex, double gain);

private:
    unsigned int sampleRate;
    std::vector<Band> bands;

    double applyBand(double sample, const Band& band);
};

#endif
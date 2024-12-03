#ifndef TREMOLO_H
#define TREMOLO_H

#include <vector>
#include <cmath>

class TremoloEffect {
public:
    TremoloEffect(double depth = 0.5, double rate = 5.0, unsigned int sampleRate = 44100);

    void apply(std::vector<double>& samples);

    void setDepth(double depth);
    void setRate(double rate);

private:
    unsigned int sampleRate;
    double depth;
    double rate;
    double lfoPhase = 0.0;
};

#endif
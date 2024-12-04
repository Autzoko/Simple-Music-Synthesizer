#ifndef DISTORTION_H
#define DISTORTION_H

#include <vector>
#include "Effect.h"

class Distortion : public Effect{
public:
    Distortion(double threshold = 0.5);

    void apply(std::vector<double>& samples) override;

    void setThreshold(double threshold);

private:
    double threshold;
};

#endif
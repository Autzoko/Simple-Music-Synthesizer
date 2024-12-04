#include "Distortion.h"
#include <algorithm>

Distortion::Distortion(double threshold) : threshold(threshold) {}

void Distortion::apply(std::vector<double>& samples) {
    for (auto& sample : samples) {
        if (sample > threshold) sample = threshold;
        else if (sample < -threshold) sample = -threshold;
    }
}

void Distortion::setThreshold(double threshold) {
    this->threshold = threshold;
}
#include "Mixer.h"
#include <algorithm>

void Mixer::addChannel(const std::vector<double>& signal, double volume) {
    channels.emplace_back(signal, volume);
}

std::vector<double> Mixer::mix() {
    if (channels.empty()) {
        return {};
    }

    size_t minLength = std::min_element(channels.begin(), channels.end(),
        [](const auto& a, const auto& b) {
            return a.first.size() < b.first.size();
        })->first.size();

    std::vector<double> mixedSignal(minLength, 0.0);

    for (const auto& [signal, volume] : channels) {
        for (size_t i = 0; i < minLength; ++i) {
            mixedSignal[i] += signal[i] * volume;
        }
    }

    for (auto& sample : mixedSignal) {
        if (sample > 1.0) sample = 1.0;
        else if (sample < -1.0) sample = -1.0;
    }

    channels.clear();
    return mixedSignal;
}
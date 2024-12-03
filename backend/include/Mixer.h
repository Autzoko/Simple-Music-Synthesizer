#ifndef MIXER_H
#define MIXER_H

#include <vector>

class Mixer {
public:
    void addChannel(const std::vector<double>& signal, double volume = 1.0);
    std::vector<double> mix();

private:
    std::vector<std::pair<std::vector<double>, double>> channels;
};

#endif //MIXER_H
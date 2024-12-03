#ifndef EFFECT_H
#define EFFECT_H

#include <vector>

class Effect {
public:
    virtual void apply(std::vector<double>& samples) = 0;

    virtual ~Effect() {}
};

#endif
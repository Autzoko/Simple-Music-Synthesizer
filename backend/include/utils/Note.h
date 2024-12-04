#ifndef NOTE_H
#define NOTE_H

#include <vector>
#include <string>

class Note {
public:
    Note(const std::string& name, double mainFrequency, const std::vector<double>& subFrequencies);

    const std::string& getName() const;
    double getMainFrequency() const;
    const double& getSubFrequencies(unsigned int index) const;

private:
    std::string name;
    double mainFrequency;
    std::vector<double> subFrequencies;
};

#endif
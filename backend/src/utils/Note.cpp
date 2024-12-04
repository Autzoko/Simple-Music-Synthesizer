#include "Note.h"

Note::Note(const std::string& name, double mainFrequency, const std::vector<double>& subFrequencies)
    : name(name), mainFrequency(mainFrequency), subFrequencies(subFrequencies) {}

const std::string& Note::getName() const {
    return name;
}

double Note::getMainFrequency() const {
    return mainFrequency;
}

const std::vector<double>& Note::getSubFrequencies() const {
    return subFrequencies;
}
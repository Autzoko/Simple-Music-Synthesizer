#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>

template <typename T>
T clamp(const T& value, const T& min, const T& max);

void writeWavFile(const std::string& filename, const std::vector<double>& buffer, unsigned int sampleRate);

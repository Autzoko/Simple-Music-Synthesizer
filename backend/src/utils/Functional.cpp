#include "Functional.h"

template <typename T>
T clamp(const T& value, const T& min, const T& max) {
    return (value < min) ? min : (value > max ? max : value);
}

void writeWavFile(const std::string& filename, const std::vector<double>& buffer, unsigned int sampleRate) {
    std::ofstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    // WAV 文件头部分
    const uint32_t subchunk1Size = 16; // PCM
    const uint16_t audioFormat = 1;   // PCM
    const uint16_t numChannels = 1;   // 单声道
    const uint32_t byteRate = sampleRate * numChannels * sizeof(int16_t);
    const uint16_t blockAlign = numChannels * sizeof(int16_t);
    const uint16_t bitsPerSample = 16; // 每个样本 16 位
    const uint32_t subchunk2Size = buffer.size() * sizeof(int16_t);
    const uint32_t chunkSize = 4 + (8 + subchunk1Size) + (8 + subchunk2Size);

    // 写入 RIFF 块
    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&chunkSize), 4);
    file.write("WAVE", 4);

    // 写入 fmt 块
    file.write("fmt ", 4);
    file.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // 写入 data 块
    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&subchunk2Size), 4);

    // 转换并写入音频数据
    for (double sample : buffer) {
        int16_t pcmSample = static_cast<int16_t>(clamp(sample, -1.0, 1.0) * 32767);
        file.write(reinterpret_cast<const char*>(&pcmSample), sizeof(int16_t));
    }

    file.close();

    std::cout << "Audio written to " << filename << std::endl;
}

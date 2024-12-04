#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include "RtAudio.h"
#include "Synthesizer.h"
#include "AudioOutput.h"

void writeWavFile(const std::string &filename, const std::vector<double> &samples, 
                  unsigned int sampleRate, unsigned int numChannels) {
    // 打开文件
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // WAV 文件头
    uint32_t fileSize = 36 + samples.size() * sizeof(int16_t) * numChannels;
    uint32_t byteRate = sampleRate * numChannels * sizeof(int16_t);
    uint16_t blockAlign = numChannels * sizeof(int16_t);
    uint16_t bitsPerSample = 16;

    // RIFF Chunk
    file.write("RIFF", 4);
    uint32_t chunkSize = fileSize - 8;
    file.write(reinterpret_cast<const char *>(&chunkSize), 4);
    file.write("WAVE", 4);

    // Format Subchunk
    file.write("fmt ", 4);
    uint32_t subChunk1Size = 16;
    file.write(reinterpret_cast<const char *>(&subChunk1Size), 4);
    uint16_t audioFormat = 1; // PCM
    file.write(reinterpret_cast<const char *>(&audioFormat), 2);
    file.write(reinterpret_cast<const char *>(&numChannels), 2);
    file.write(reinterpret_cast<const char *>(&sampleRate), 4);
    file.write(reinterpret_cast<const char *>(&byteRate), 4);
    file.write(reinterpret_cast<const char *>(&blockAlign), 2);
    file.write(reinterpret_cast<const char *>(&bitsPerSample), 2);

    // Data Subchunk
    file.write("data", 4);
    uint32_t subChunk2Size = samples.size() * sizeof(int16_t) * numChannels;
    file.write(reinterpret_cast<const char *>(&subChunk2Size), 4);

    // 写入音频数据
    for (const auto &sample : samples) {
        int16_t intSample = static_cast<int16_t>(std::clamp(sample, -1.0, 1.0) * 32767);
        file.write(reinterpret_cast<const char *>(&intSample), sizeof(int16_t));
    }

    file.close();
    std::cout << "WAV file written to " << filename << std::endl;
}

int main() {
    unsigned int sampleRate = 44100;
    double duration = 2.0;

    Synthesizer synth(sampleRate);
    
    Oscillator* osc1 = new Oscillator(
        WaveformType::SAWTOOTH,
        440.0,
        1.0,
        0.0,
        sampleRate
    );
    synth.addOscillator(osc1);

    EnvelopeGenerator* env = new EnvelopeGenerator(
        0.01,
        0.1,
        0.8,
        0.5,
        sampleRate
    );
    synth.setEnvelope(env);

    Filter* filter = new Filter(
        FilterType::LOWPASS,
        1000.0,
        0.7071,
        sampleRate
    );
    synth.setFilter(filter);

    DelayEffect* delay = new DelayEffect(
        0.5,
        0.5,
        0.3
    );
    synth.addEffect(delay);

    std::vector<double> samples = synth.noteOn(440.0, duration);

    AudioOutput audioOutput(sampleRate);
    audioOutput.play(samples);

    writeWavFile("output.wav", samples, sampleRate, 1);

    delete osc1;
    delete env;
    delete filter;
    delete delay;

    return 0;
}
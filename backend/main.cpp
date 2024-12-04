#include "Synthesizer.h"
#include "Note.h"
#include "Key.h"
#include "Functional.h"
#include "Chorus.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

struct NoteEvent {
    std::string key;   // 音符
    double startTime;  // 起始时间（秒）
    double duration;   // 持续时间（秒）
};

int main() {
    const unsigned int sampleRate = 44100;
    const double totalDuration = 5.0; // 总时长 5 秒

    // 创建合成器
    Synthesizer synth(sampleRate);

    // 音符事件队列
    std::vector<NoteEvent> events = {
        {"C4", 0.0, 2.0}, // C4 从 0 秒开始，持续 2 秒
        {"E4", 1.0, 2.0}, // E4 从 1 秒开始，持续 2 秒（部分重叠）
        {"G4", 2.0, 2.0}, // G4 从 2 秒开始，持续 2 秒（部分重叠）
        {"C4", 2.5, 1.5}  // C4 从 2.5 秒开始，持续 1.5 秒（完全重叠）
    };

    // 创建音频缓冲区
    std::vector<double> audioBuffer(static_cast<size_t>(totalDuration * sampleRate), 0.0);

    // 按时间线合成音频
    for (const auto& event : events) {
        Note note = getNoteFromKey(event.key);
        synth.noteOn(note, 1.0);

        unsigned int startFrame = static_cast<unsigned int>(event.startTime * sampleRate);
        unsigned int numFrames = static_cast<unsigned int>(event.duration * sampleRate);

        std::vector<double> tempBuffer(numFrames, 0.0);
        synth.generateAudio(tempBuffer, numFrames);

        for (size_t i = 0; i < numFrames; ++i) {
            if (startFrame + i < audioBuffer.size()) {
                audioBuffer[startFrame + i] += tempBuffer[i];
            }
        }

        synth.noteOff(note);
    }

    // 写入 WAV 文件
    writeWavFile("overlapping_notes.wav", audioBuffer, sampleRate);

    return 0;
}

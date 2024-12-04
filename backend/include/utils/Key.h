#include <stdexcept>
#include <unordered_map>

#include "Note.h"

// 音符频率表，手动定义主频和副频倍率
const std::unordered_map<std::string, Note> noteMap = {
    {"C4", Note("C4", 261.63, {1.5, 2.0, 2.5})},
    {"D4", Note("D4", 293.66, {1.5, 2.0, 2.5})},
    {"E4", Note("E4", 329.63, {1.5, 2.0, 2.5})},
    {"F4", Note("F4", 349.23, {1.5, 2.0, 2.5})},
    {"G4", Note("G4", 392.00, {1.5, 2.0, 2.5})},
    {"A4", Note("A4", 440.00, {1.5, 2.0, 2.5})},
    {"B4", Note("B4", 493.88, {1.5, 2.0, 2.5})},
    {"C5", Note("C5", 523.25, {1.5, 2.0, 2.5})},
    {"C-1", Note("C-1", 8.18, {1.5, 2.0, 2.5})},
    {"C#-1", Note("C#-1", 8.66, {1.5, 2.0, 2.5})},
    {"D-1", Note("D-1", 9.18, {1.5, 2.0, 2.5})},
    {"G9", Note("G9", 12543.85, {1.5, 2.0, 2.5})},
};

Note getNoteFromKey(const std::string& key);

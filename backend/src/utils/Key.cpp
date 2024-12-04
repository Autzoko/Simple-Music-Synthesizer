#include "Key.h"

Note getNoteFromKey(const std::string& key) {
    auto it = noteMap.find(key);
    if (it == noteMap.end()) {
        throw std::invalid_argument("Invalid key: " + key);
    }
    return it->second;
}

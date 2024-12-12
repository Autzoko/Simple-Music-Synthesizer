#include <MIDIHandler.h>
#include <iostream>

MIDIHandler::MIDIHandler() {
    midiIn = std::make_unique<RtMidiIn>();
    unsigned int protCount = midiIn->getPortCount();

    if (protCount == 0) {
        std::cerr << "No MIDI input devices found.\n";
        return;
    }

    midiIn->openPort(0);
    midiIn->ignoreTypes(false, false, false);
}

MIDIHandler::~MIDIHandler() {
    stop();
}

void MIDIHandler::start() {
    if (!midiIn) return;

    midiIn->setCallback([](double deltatime, std::vector<unsigned char>* message, void* userData) {
        auto* handler = static_cast<MIDIHandler*>(userData);
        handler->processMidiMessage(deltatime, message);
    }, this);
}

void MIDIHandler::stop() {
    if (midiIn) {
        midiIn->cancelCallback();
    }
}

void MIDIHandler::setNoteOnCallback(const std::function<void(int, double)>& callback) {
    noteOnCallback = callback;
}

void MIDIHandler::setNoteOffCallback(const std::function<void(int)>& callback) {
    noteOffCallback = callback;
}

void MIDIHandler::processMidiMessage(double deltaTime, std::vector<unsigned char>* message) {
    if (message->empty()) return;

    unsigned char status = message->at(0) & 0xF0;
    unsigned char note = message->at(1);
    unsigned char velocity = message->at(2);

    switch (status) {
        case 0x90:
            if (velocity > 0 && noteOnCallback) {
                noteOnCallback(note, velocity / 127.0);
            }
            break;
        case 0x80:
            if (noteOffCallback) {
                noteOffCallback(note);
            }
            break;
        default:
            break;
    }
}
#include "UI.h"
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

SynthesizerUI::SynthesizerUI(QWidget* parent)
    : QMainWindow(parent) {
    
    synthesizer = std::make_shared<Synthesizer>(44100);
    try {
        keyToNoteMapping = loadNotesFromFile("resources/midi_notes.json");
        std::cout << "Notes loaded" << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error notes loading: " << e.what() << std::endl;
    }

    setupUI();
    connectUI();

    audioPlayer = new AudioOutput(*synthesizer);
    audioPlayer->start();
}

SynthesizerUI::~SynthesizerUI() {
    if (audioPlayer) {
        audioPlayer->stop();
        delete audioPlayer;
    }
}

void SynthesizerUI::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // 左侧：Oscillator 控制面板
    QGridLayout* oscillatorLayout = new QGridLayout();
    for (int i = 0; i < 4; ++i) {
        auto panel = new OscillatorControlPanel(i, this);
        oscillatorPanels.append(panel);
        oscillatorLayout->addWidget(panel, i / 2, i % 2); // 田字格布局
    }
    QGroupBox* oscillatorGroup = new QGroupBox("Oscillators", centralWidget);
    oscillatorGroup->setLayout(oscillatorLayout);
    oscillatorGroup->setStyleSheet("border: 1px solid lightgray; padding: 5px;");

    // 右上角：Envelope Panel
    envelopePanel = new EnvelopePanel(synthesizer, this);
    QGroupBox* envelopeGroup = new QGroupBox("Envelope", centralWidget);
    envelopeGroup->setLayout(new QVBoxLayout());
    envelopeGroup->layout()->addWidget(envelopePanel);
    envelopeGroup->setStyleSheet("border: 1px solid lightgray; padding: 5px;");

    // 右下角：Filter Panel
    filterPanel = new FilterPanel(synthesizer, this);
    QGroupBox* filterGroup = new QGroupBox("Filter", centralWidget);
    filterGroup->setLayout(new QVBoxLayout());
    filterGroup->layout()->addWidget(filterPanel);
    filterGroup->setStyleSheet("border: 1px solid lightgray; padding: 5px;");

    // 右侧：Effect 控制面板
    QVBoxLayout* effectLayout = new QVBoxLayout();
    effectGroup = new EffectGroup(synthesizer, this);
    effectLayout->addWidget(effectGroup);
    QGroupBox* effectGroupBox = new QGroupBox("Effects", centralWidget);
    effectGroupBox->setLayout(effectLayout);
    effectGroupBox->setStyleSheet("border: 1px solid lightgray; padding: 5px;");

    // 合并布局
    QVBoxLayout* rightPanelLayout = new QVBoxLayout();
    rightPanelLayout->addWidget(envelopeGroup, 1);
    rightPanelLayout->addWidget(filterGroup, 1);

    mainLayout->addWidget(oscillatorGroup, 2);
    mainLayout->addLayout(rightPanelLayout, 1);
    mainLayout->addWidget(effectGroupBox, 1);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}



void SynthesizerUI::connectUI() {
    // Connect oscillator panels to Synthesizer
    for (int i = 0; i < oscillatorPanels.size(); ++i) {
        auto* panel = oscillatorPanels[i];
        connect(panel, &OscillatorControlPanel::waveformChanged, this, [this, i](int index, int waveform) {
            synthesizer->setOscillatorWaveform(index, static_cast<WaveformType>(waveform));
        });
        connect(panel, &OscillatorControlPanel::weightChanged, this, [this, i](int index, double weight) {
            synthesizer->setOscillatorWeight(index, weight);
        });
        connect(panel, &OscillatorControlPanel::detuneChanged, this, [this, i](int index, double detune) {
            synthesizer->setOscillatorDetune(index, detune);
        });
    }

    // EnvelopePanel and FilterPanel signals are connected internally
    // EffectGroup handles its own connections
}

void SynthesizerUI::keyPressEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) return;
    int key = event->key();
    if (keyToNoteMapping.contains(key)) {
        Note note = keyToNoteMapping[key];
        synthesizer->noteOn(note, 1.0);
        activeKeys.insert(key);
    }
}

void SynthesizerUI::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) return;

    int key = event->key();
    if (activeKeys.contains(key)) {
        Note note = keyToNoteMapping[key];
        synthesizer->noteOff(note);
        activeKeys.remove(key);
    }
}

std::map<int, Note> SynthesizerUI::loadNotesFromFile(const std::string& filePath) {
    std::map<int, Note> keyToNoteMapping;

    // 打开 JSON 文件
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open JSON file: " + filePath);
    }

    // 解析 JSON 数据
    json jsonData;
    file >> jsonData;

    for (const auto& [key, value] : jsonData.items()) {
        std::string name = value["name"];
        double mainFrequency = value["mainFrequency"];
        std::vector<double> subFrequencies = value["subFrequencies"].get<std::vector<double>>();

        int noteKey = std::stoi(key);
        keyToNoteMapping[noteKey] = Note(name, mainFrequency, subFrequencies);
    }

    return keyToNoteMapping;
}
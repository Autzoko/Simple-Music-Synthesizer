#include "UI.h"
#include <iostream>

SynthesizerUI::SynthesizerUI(QWidget* parent)
    : QMainWindow(parent) {
    
    synthesizer = std::make_shared<Synthesizer>(44100);

    keyToNoteMapping[Qt::Key_A] = Note("A", 440.0, {440.0, 440.0, 440.0});
    keyToNoteMapping[Qt::Key_W] = Note("W", 466.16, {466.16, 466.16, 466.16});
    keyToNoteMapping[Qt::Key_S] = Note("S", 493.88, {493.88, 493.88, 493.88});
    keyToNoteMapping[Qt::Key_D] = Note("D", 523.25, {523.25, 523.25, 523.25});
    keyToNoteMapping[Qt::Key_R] = Note("R", 554.37, {554.0, 554.0, 554.0});
    keyToNoteMapping[Qt::Key_F] = Note("F", 587.33, {587.0, 587.0, 587.0});
    keyToNoteMapping[Qt::Key_T] = Note("T", 622.25, {622.0, 622.0, 622.0});
    keyToNoteMapping[Qt::Key_G] = Note("G", 659.25, {659.0, 659.0, 659.0});
    keyToNoteMapping[Qt::Key_H] = Note("H", 698.46, {698.0, 698.0, 698.0});
    
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
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    // Oscillator Panels
    oscillatorPanelContainer = new QWidget(centralWidget);
    oscillatorPanelLayout = new QVBoxLayout(oscillatorPanelContainer);

    for (int i = 0; i < 4; ++i) {
        auto* panel = new OscillatorControlPanel(i, oscillatorPanelContainer);
        oscillatorPanels.append(panel);
        oscillatorPanelLayout->addWidget(panel);
    }

    oscillatorPanelContainer->setLayout(oscillatorPanelLayout);
    mainLayout->addWidget(oscillatorPanelContainer);

    // Envelope Panel
    envelopePanel = new EnvelopePanel(synthesizer, centralWidget);
    mainLayout->addWidget(envelopePanel);

    // Filter Panel
    filterPanel = new FilterPanel(synthesizer, centralWidget);
    mainLayout->addWidget(filterPanel);

    // Effect Group
    effectGroup = new EffectGroup(synthesizer, centralWidget);
    mainLayout->addWidget(effectGroup);

    centralWidget->setLayout(mainLayout);
}

void SynthesizerUI::connectUI() {
    // Connect oscillator panels to Synthesizer
    for (int i = 0; i < oscillatorPanels.size(); ++i) {
        auto* panel = oscillatorPanels[i];
        connect(panel, &OscillatorControlPanel::waveformChanged, this, [this, i](int waveform) {
            synthesizer->setOscillatorWaveform(i, static_cast<WaveformType>(waveform));
        });
        connect(panel, &OscillatorControlPanel::weightChanged, this, [this, i](double weight) {
            synthesizer->setOscillatorWeight(i, weight);
        });
        connect(panel, &OscillatorControlPanel::detuneChanged, this, [this, i](double detune) {
            synthesizer->setOscillatorDetune(i, detune);
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
        qDebug() << "Key " << event->text() << " Note " << note.getMainFrequency();
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
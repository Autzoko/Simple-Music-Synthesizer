#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QKeyEvent>
#include <QSet>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <memory>
#include <map>
#include "OscillatorControlPanel.h"
#include "EnvelopePanel.h"
#include "EffectGroup.h"
#include "FilterPanel.h"
#include "Synthesizer.h"
#include "Note.h"
#include "AudioOutput.h"
#include "MIDIHandler.h"

class SynthesizerUI : public QMainWindow {
    Q_OBJECT

public:
    explicit SynthesizerUI(QWidget* parent = nullptr);
    ~SynthesizerUI();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    std::shared_ptr<Synthesizer> synthesizer;

    QWidget* centralWidget;
    QVBoxLayout* mainLayout;

    // Panels
    QWidget* oscillatorPanelContainer;
    QVBoxLayout* oscillatorPanelLayout;
    QVector<OscillatorControlPanel*> oscillatorPanels;

    EnvelopePanel* envelopePanel;
    FilterPanel* filterPanel;
    EffectGroup* effectGroup;

    std::map<int, Note> keyToNoteMapping;

    AudioOutput* audioPlayer;

    QSet<int> activeKeys;

    std::unique_ptr<MIDIHandler> midiHandler;
    
    void setupUI();
    void connectUI();
    void setupMIDI();

    std::map<int, Note> loadNotesFromFile(const std::string& filePath);

};
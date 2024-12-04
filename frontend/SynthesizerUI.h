#ifndef SYNTHESIZER_UI_H
#define SYNTHESIZER_UI_H

#include <QWidget>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <memory>

// 引入后端头文件
#include "Synthesizer.h"

class SynthesizerUI : public QWidget {
    Q_OBJECT

public:
    explicit SynthesizerUI(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateOscillatorWaveform(int index);
    void updateOscillatorWeight(int index);
    void updateFilterParams();
    void updateEnvelopeParams();
    void addEffect();
    void removeEffect();
    void updateEffectParams();

private:
    std::shared_ptr<Synthesizer> synthesizer;

    // UI 控件
    QComboBox *waveformSelector;
    std::vector<QSlider*> oscillatorWeightSliders;
    QSlider *filterFrequencySlider;
    QSlider *filterQSlider;
    QSlider *attackSlider;
    QSlider *decaySlider;
    QSlider *sustainSlider;
    QSlider *releaseSlider;
    QVBoxLayout *effectsLayout;

    void setupUI();
    void setupConnections();
};

#endif // SYNTHESIZER_UI_H

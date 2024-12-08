#ifndef SYNTHESIZER_UI_H
#define SYNTHESIZER_UI_H

#include <QMainWindow>
#include "ui_SynthesizerUI.h"
#include <array>
#include "Synthesizer.h"

class SynthesizerUI : public QMainWindow {
    Q_OBJECT

public:
    explicit SynthesizerUI(QWidget *parent = nullptr);
    ~SynthesizerUI();

private slots:
    // 波形
    void onWaveformChanged(int id);

    // 滤波器
    void onFilterTypeChanged(int id);
    void onFilterCutoffChanged(int value);
    void onFilterResonanceChanged(int value);

    // 振荡器
    void onOscWeightChanged(int osc, int value);
    void onOscDetuneChanged(int osc, int value);
    void onOscWaveformChanged(int osc, int id);

    // 包络
    void onAttackChanged(int value);
    void onDecayChanged(int value);
    void onSustainChanged(int value);
    void onReleaseChanged(int value);

    // 效果器
    void onChorusEnableChanged(bool enabled);
    void onChorusDepthChanged(int value);
    void onChorusRateChanged(int value);

    void onReverbEnableChanged(bool enabled);
    void onReverbDecayChanged(int value);
    void onReverbMixChanged(int value);

    void onDistortionEnableChanged(bool enabled);
    void onDistortionThresholdChanged(int value);

    void onEqEnableChanged(bool enabled);
    void onEqGainChanged(int value);

    void onPitchShifterEnableChanged(bool enabled);
    void onPitchFactorChanged(int value);

    void onDelayEnableChanged(bool enabled);
    void onDelayTimeChanged(int value);
    void onDelayFeedbackChanged(int value);
    void onDelayMixChanged(int value);

    void onTremoloEnableChanged(bool enabled);
    void onTremoloDepthChanged(int value);
    void onTremoloRateChanged(int value);

private:
    Ui::SynthesizerUI *ui;
    std::shared_ptr<Synthesizer> synthesizer;

    void setupConnections();
};

#endif

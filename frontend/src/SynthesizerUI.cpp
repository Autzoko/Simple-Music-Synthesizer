#include "SynthesizerUI.h"

SynthesizerUI::SynthesizerUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SynthesizerUI),
    synthesizer(std::make_shared<Synthesizer>(44100))
{
    ui->setupUi(this);
    setupConnections();
}

SynthesizerUI::~SynthesizerUI()
{
    delete ui;
}

void SynthesizerUI::setupConnections() {
    // 振荡器连接
    connect(ui->WeightDial1, &QDial::valueChanged,
            this, [this](int value) { onOscWeightChanged(0, value); });
    connect(ui->DetuneDial1, &QDial::valueChanged,
            this, [this](int value) { onOscDetuneChanged(0, value); });

    connect(ui->WeightDial2, &QDial::valueChanged,
            this, [this](int value) { onOscWeightChanged(1, value); });
    connect(ui->DetuneDial2, &QDial::valueChanged,
            this, [this](int value) { onOscDetuneChanged(1, value); });

    connect(ui->WeightDial3, &QDial::valueChanged,
            this, [this](int value) { onOscWeightChanged(2, value); });
    connect(ui->DetuneDial3, &QDial::valueChanged,
            this, [this](int value) { onOscDetuneChanged(2, value); });

    connect(ui->WeightDial4, &QDial::valueChanged,
            this, [this](int value) { onOscWeightChanged(3, value); });
    connect(ui->DetuneDial4, &QDial::valueChanged,
            this, [this](int value) { onOscDetuneChanged(3, value); });

    // 效果器连接
    // Chorus
    connect(ui->EnablecheckBox1, &QCheckBox::toggled,
            this, &SynthesizerUI::onChorusEnableChanged);
    connect(ui->ChorusDepthDial, &QDial::valueChanged,
            this, &SynthesizerUI::onChorusDepthChanged);
    connect(ui->ChorusRateDial, &QDial::valueChanged,
            this, &SynthesizerUI::onChorusRateChanged);

    // Reverb
    connect(ui->EnablecheckBox2, &QCheckBox::toggled,
            this, &SynthesizerUI::onReverbEnableChanged);
    connect(ui->ReverbDecayDial, &QDial::valueChanged,
            this, &SynthesizerUI::onReverbDecayChanged);
    connect(ui->ChorusRateDial_6, &QDial::valueChanged,
            this, &SynthesizerUI::onReverbMixChanged);

    // Distortion
    connect(ui->EnablecheckBox3, &QCheckBox::toggled,
            this, &SynthesizerUI::onDistortionEnableChanged);
    connect(ui->DistortionThresholdDial, &QDial::valueChanged,
            this, &SynthesizerUI::onDistortionThresholdChanged);

    // Equalizer
    connect(ui->EnablecheckBox4, &QCheckBox::toggled,
            this, &SynthesizerUI::onEqEnableChanged);
    connect(ui->EqualizerGainDial, &QDial::valueChanged,
            this, &SynthesizerUI::onEqGainChanged);

    // Pitch Shifter
    connect(ui->EnablecheckBox5, &QCheckBox::toggled,
            this, &SynthesizerUI::onPitchShifterEnableChanged);
    connect(ui->Pitch_ShifterFactorDial, &QDial::valueChanged,
            this, &SynthesizerUI::onPitchFactorChanged);

    // Delay
    connect(ui->EnablecheckBox6, &QCheckBox::toggled,
            this, &SynthesizerUI::onDelayEnableChanged);
    connect(ui->DelayTimeDial, &QDial::valueChanged,
            this, &SynthesizerUI::onDelayTimeChanged);
    connect(ui->DelayFeedbackDial, &QDial::valueChanged,
            this, &SynthesizerUI::onDelayFeedbackChanged);
    connect(ui->DelayMixDial, &QDial::valueChanged,
            this, &SynthesizerUI::onDelayMixChanged);

    // Tremolo
    connect(ui->EnablecheckBox7, &QCheckBox::toggled,
            this, &SynthesizerUI::onTremoloEnableChanged);
    connect(ui->TremoloDepthDial, &QDial::valueChanged,
            this, &SynthesizerUI::onTremoloDepthChanged);
    connect(ui->TremoloRateDial, &QDial::valueChanged,
            this, &SynthesizerUI::onTremoloRateChanged);

    // 包络
    connect(ui->AttackDail, &QDial::valueChanged,
            this, &SynthesizerUI::onAttackChanged);
    connect(ui->DecayDail, &QDial::valueChanged,
            this, &SynthesizerUI::onDecayChanged);
    connect(ui->SustainDail, &QDial::valueChanged,
            this, &SynthesizerUI::onSustainChanged);
    connect(ui->ReleaseDail, &QDial::valueChanged,
            this, &SynthesizerUI::onReleaseChanged);

    // 滤波器
    connect(ui->CutoggDial, &QDial::valueChanged,
            this, &SynthesizerUI::onFilterCutoffChanged);
    connect(ui->ResonanceDial, &QDial::valueChanged,
            this, &SynthesizerUI::onFilterResonanceChanged);
}


// 振荡器相关槽函数
void SynthesizerUI::onOscWeightChanged(int osc, int value) {
    // 将0-100的UI值转换为0-1的音量权重
    synthesizer->setOscillatorWeight(osc, value / 100.0);
}

void SynthesizerUI::onOscDetuneChanged(int osc, int value) {
    // 将0-100的UI值转换为0-1的音高偏移量
    synthesizer->setOscillatorDetune(osc, value / 100.0);
}

void SynthesizerUI::onOscWaveformChanged(int osc, int id) {
    // 将按钮ID转换为波形类型枚举
    WaveformType type;
    switch(id) {
    case 0: type = WaveformType::SINE; break;     // 正弦波
    case 1: type = WaveformType::SQUARE; break;   // 方波
    case 2: type = WaveformType::SAWTOOTH; break; // 锯齿波
    case 3: type = WaveformType::TRIANGLE; break; // 三角波
    case 4: type = WaveformType::NOISE; break;    // 噪声
    }
    synthesizer->setOscillatorWaveform(osc, type);
}

// 滤波器相关槽函数
void SynthesizerUI::onFilterTypeChanged(int id) {
    // 将按钮ID转换为滤波器类型
    FilterType type;
    switch(id) {
    case 0: type = FilterType::HIGHPASS; break; // 高通滤波器
    case 1: type = FilterType::LOWPASS; break; // 低通滤波器
    case 2: type = FilterType::BANDPASS; break; // 带通滤波器
    }
    synthesizer->setFilterType(type);
}

void SynthesizerUI::onFilterCutoffChanged(int value) {
    // 直接设置截止频率(20-20000Hz)
    synthesizer->setFilterCutoff(value);
}

void SynthesizerUI::onFilterResonanceChanged(int value) {
    // 将0-10的UI值转换为0-1的谐振值
    synthesizer->setFilterResonance(value / 10.0);
}

// ADSR包络相关槽函数
void SynthesizerUI::onAttackChanged(int value) {
    // 设置起音时间(ms)
    synthesizer->setEnvelopeAttack(value);
}

void SynthesizerUI::onDecayChanged(int value) {
    // 设置衰减时间(ms)
    synthesizer->setEnvelopeDecay(value);
}

void SynthesizerUI::onSustainChanged(int value) {
    // 将0-100的UI值转换为0-1的持续电平
    synthesizer->setEnvelopeSustain(value / 100.0);
}

void SynthesizerUI::onReleaseChanged(int value) {
    // 设置释音时间(ms)
    synthesizer->setEnvelopeRelease(value);
}

// 效果器相关槽函数
// Chorus
void SynthesizerUI::onChorusEnableChanged(bool enabled) {
    synthesizer->enableEffect("chorus", enabled);
}

void SynthesizerUI::onChorusDepthChanged(int value) {
    // 设置合唱效果深度(0-1)
    synthesizer->setEffectParam("chorus", "depth", value / 100.0);
}

void SynthesizerUI::onChorusRateChanged(int value) {
    // 设置合唱效果速率(0-10Hz)
    synthesizer->setEffectParam("chorus", "rate", value / 10.0);
}

// Reverb
void SynthesizerUI::onReverbEnableChanged(bool enabled) {
    synthesizer->enableEffect("reverb", enabled);
}

void SynthesizerUI::onReverbDecayChanged(int value) {
    // 设置混响衰减时间(0-1)
    synthesizer->setEffectParam("reverb", "decay", value / 100.0);
}

void SynthesizerUI::onReverbMixChanged(int value) {
    // 设置混响干湿比(0-1)
    synthesizer->setEffectParam("reverb", "mix", value / 100.0);
}

// Distortion
void SynthesizerUI::onDistortionEnableChanged(bool enabled) {
    synthesizer->enableEffect("distortion", enabled);
}

void SynthesizerUI::onDistortionThresholdChanged(int value) {
    // 设置失真阈值(0-1)
    synthesizer->setEffectParam("distortion", "threshold", value / 100.0);
}

// Equalizer
void SynthesizerUI::onEqEnableChanged(bool enabled) {
    synthesizer->enableEffect("equalizer", enabled);
}

void SynthesizerUI::onEqGainChanged(int value) {
    // 设置均衡器增益(-12到+12dB)
    synthesizer->setEffectParam("equalizer", "gain", value);
}

// Pitch Shifter
void SynthesizerUI::onPitchShifterEnableChanged(bool enabled) {
    synthesizer->enableEffect("pitchshifter", enabled);
}

void SynthesizerUI::onPitchFactorChanged(int value) {
    // 设置音高偏移因子(0.5-2.0)
    synthesizer->setEffectParam("pitchshifter", "factor", value / 10.0);
}

// Delay
void SynthesizerUI::onDelayEnableChanged(bool enabled) {
    synthesizer->enableEffect("delay", enabled);
}

void SynthesizerUI::onDelayTimeChanged(int value) {
    // 设置延迟时间(0-2s)
    synthesizer->setEffectParam("delay", "time", value / 100.0);
}

void SynthesizerUI::onDelayFeedbackChanged(int value) {
    // 设置延迟反馈量(0-1)
    synthesizer->setEffectParam("delay", "feedback", value / 100.0);
}

void SynthesizerUI::onDelayMixChanged(int value) {
    // 设置延迟干湿比(0-1)
    synthesizer->setEffectParam("delay", "mix", value / 100.0);
}

// Tremolo
void SynthesizerUI::onTremoloEnableChanged(bool enabled) {
    synthesizer->enableEffect("tremolo", enabled);
}

void SynthesizerUI::onTremoloDepthChanged(int value) {
    // 设置颤音深度(0-1)
    synthesizer->setEffectParam("tremolo", "depth", value / 100.0);
}

void SynthesizerUI::onTremoloRateChanged(int value) {
    // 设置颤音速率(0-10Hz)
    synthesizer->setEffectParam("tremolo", "rate", value / 10.0);
}

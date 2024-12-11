#include "OscillatorControlPanel.h"

OscillatorControlPanel::OscillatorControlPanel(int index, QWidget* parent)
    : QWidget(parent), index(index) {
        setupUI();
        setupConnections();
}

OscillatorControlPanel::~OscillatorControlPanel() = default;

void OscillatorControlPanel::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    auto* waveformLayout = new QHBoxLayout();
    auto* weightLayout = new QHBoxLayout();
    auto* detuneLayout = new QHBoxLayout();

    // 波形选择
    waveformGroup = new QButtonGroup(this);
    QRadioButton* sineButton = new QRadioButton("Sine", this);
    QRadioButton* squareButton = new QRadioButton("Square", this);
    QRadioButton* sawtoothButton = new QRadioButton("Sawtooth", this);
    QRadioButton* triangleButton = new QRadioButton("Triangle", this);
    QRadioButton* noiseButton = new QRadioButton("Noise", this);

    waveformGroup->addButton(sineButton, 0);
    waveformGroup->addButton(squareButton, 1);
    waveformGroup->addButton(sawtoothButton, 2);
    waveformGroup->addButton(triangleButton, 3);
    waveformGroup->addButton(noiseButton, 4);

    waveformLayout->addWidget(sineButton);
    waveformLayout->addWidget(squareButton);
    waveformLayout->addWidget(sawtoothButton);
    waveformLayout->addWidget(triangleButton);
    waveformLayout->addWidget(noiseButton);
    mainLayout->addLayout(waveformLayout);

    // 权重旋钮
    QLabel* weightLabel = new QLabel("Weight", this);
    weightDial = new QDial(this);
    weightDial->setRange(0, 100);
    weightLayout->addWidget(weightLabel);
    weightLayout->addWidget(weightDial);
    mainLayout->addLayout(weightLayout);

    // Detune旋钮
    QLabel* detuneLabel = new QLabel("Detune", this);
    detuneDial = new QDial(this);
    detuneDial->setRange(0, 100);
    detuneLayout->addWidget(detuneLabel);
    detuneLayout->addWidget(detuneDial);
    mainLayout->addLayout(detuneLayout);
}

void OscillatorControlPanel::setupConnections() {
    // Connect waveform group button signals to waveformChanged signal
    connect(waveformGroup, &QButtonGroup::buttonClicked, this, [this](QAbstractButton* button) {
        waveformGroup->blockSignals(true);
        int waveformIndex = waveformGroup->id(button); // 获取按钮对应的 ID
        emit waveformChanged(index, waveformIndex);    // 发送自定义信号
        waveformGroup->blockSignals(false);
    });

    // Connect weight dial to weightChanged signal
    connect(weightDial, &QDial::valueChanged, this, [this](int value) {
        double weight = value / 100.0; // 将值从 0-100 映射到 0.0-1.0
        emit weightChanged(index, weight); // 发送自定义信号
    });

    // Connect detune dial to detuneChanged signal
    connect(detuneDial, &QDial::valueChanged, this, [this](int value) {
        double detune = value / 100.0; // 将值从 0-100 映射到 0.0-1.0
        emit detuneChanged(index, detune); // 发送自定义信号
    });
}

#include "OscillatorControlPanel.h"

OscillatorControlPanel::OscillatorControlPanel(int index, QWidget* parent)
    : QWidget(parent), index(index) {
        setupUI();
        setupConnections();
}

OscillatorControlPanel::~OscillatorControlPanel() = default;

void OscillatorControlPanel::setupUI() {
    // 主布局
    auto* mainLayout = new QVBoxLayout(this);

    // 旋钮布局
    auto* knobLayout = new QHBoxLayout();

    // 权重旋钮
    QLabel* weightLabel = new QLabel("Weight", this);
    weightDial = new QDial(this);
    weightDial->setRange(0, 100);
    weightDial->setValue(100);
    weightValueLabel = new QLabel("1.0", this);
    weightValueLabel->setAlignment(Qt::AlignCenter);

    knobLayout->addWidget(weightLabel);
    knobLayout->addWidget(weightDial);
    knobLayout->addWidget(weightValueLabel);

    // Detune旋钮
    QLabel* detuneLabel = new QLabel("Detune", this);
    detuneDial = new QDial(this);
    detuneDial->setRange(0, 100);
    detuneDial->setValue(0);
    detuneValueLabel = new QLabel("0.0", this);
    detuneValueLabel->setAlignment(Qt::AlignCenter);
    knobLayout->addWidget(detuneLabel);
    knobLayout->addWidget(detuneDial);
    knobLayout->addWidget(detuneValueLabel);

    // 将旋钮布局添加到主布局
    mainLayout->addLayout(knobLayout);

    // 波形选择布局
    auto* waveformLayout = new QVBoxLayout();
    waveformGroup = new QButtonGroup(this);

    QRadioButton* sineButton = new QRadioButton("Sine", this);
    sineButton->setChecked(true);
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

    // 将波形选择布局添加到主布局
    mainLayout->addLayout(waveformLayout);

    // 设置面板边框样式
    setStyleSheet(R"(
        QWidget {
            border: 1px solid lightgray;
            border-radius: 5px;
            padding: 10px;
            background-color: #f9f9f9;
        }
        QLabel {
            font-size: 12px;
            color: #333333;
        }
        QDial {
            background-color: #ffffff;
            border: 1px solid lightgray;
            border-radius: 15px;
        }
    )");

    setLayout(mainLayout);
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

    connect(weightDial, &QDial::valueChanged, this, [this](int value) {
        double weight = value / 100.0; // 显示值范围为 0.0 - 1.0
        weightValueLabel->setText(QString::number(weight, 'f', 2)); // 格式化为小数点后两位
        emit weightChanged(index, weight);
    });

    // 更新 Detune 的数值
    connect(detuneDial, &QDial::valueChanged, this, [this](int value) {
        double detune = value / 100.0; // 显示值范围为 0.0 - 1.0
        detuneValueLabel->setText(QString::number(detune, 'f', 2)); // 格式化为小数点后两位
        emit detuneChanged(index, detune);
    });
}

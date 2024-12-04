#include "SynthesizerUI.h"
#include "Key.h"

SynthesizerUI::SynthesizerUI(QWidget *parent)
    : QWidget(parent), synthesizer(std::make_shared<Synthesizer>(44100)) {
    setupUI();
    setupConnections();
}

void SynthesizerUI::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 波形选择
    waveformSelector = new QComboBox(this);
    waveformSelector->addItem("Sine");
    waveformSelector->addItem("Square");
    waveformSelector->addItem("Sawtooth");
    waveformSelector->addItem("Triangle");
    waveformSelector->addItem("Noise");
    mainLayout->addWidget(new QLabel("Waveform"));
    mainLayout->addWidget(waveformSelector);

    // 振荡器比重
    QVBoxLayout *oscLayout = new QVBoxLayout();
    for (int i = 0; i < 4; ++i) {
        QSlider *slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(0, 100);
        slider->setValue(25);
        oscillatorWeightSliders.push_back(slider);
        oscLayout->addWidget(new QLabel(QString("Oscillator %1 Weight").arg(i + 1)));
        oscLayout->addWidget(slider);
    }
    mainLayout->addLayout(oscLayout);

    // 滤波器
    filterFrequencySlider = new QSlider(Qt::Horizontal, this);
    filterFrequencySlider->setRange(20, 20000);
    filterFrequencySlider->setValue(1000);
    filterQSlider = new QSlider(Qt::Horizontal, this);
    filterQSlider->setRange(1, 10);
    filterQSlider->setValue(5);

    mainLayout->addWidget(new QLabel("Filter Frequency"));
    mainLayout->addWidget(filterFrequencySlider);
    mainLayout->addWidget(new QLabel("Filter Q"));
    mainLayout->addWidget(filterQSlider);

    // 包络生成器
    attackSlider = new QSlider(Qt::Horizontal, this);
    attackSlider->setRange(0, 5000); // 0 - 5000 ms
    attackSlider->setValue(100);
    decaySlider = new QSlider(Qt::Horizontal, this);
    decaySlider->setRange(0, 5000);
    decaySlider->setValue(200);
    sustainSlider = new QSlider(Qt::Horizontal, this);
    sustainSlider->setRange(0, 100);
    sustainSlider->setValue(70);
    releaseSlider = new QSlider(Qt::Horizontal, this);
    releaseSlider->setRange(0, 5000);
    releaseSlider->setValue(300);

    mainLayout->addWidget(new QLabel("Envelope - Attack"));
    mainLayout->addWidget(attackSlider);
    mainLayout->addWidget(new QLabel("Envelope - Decay"));
    mainLayout->addWidget(decaySlider);
    mainLayout->addWidget(new QLabel("Envelope - Sustain"));
    mainLayout->addWidget(sustainSlider);
    mainLayout->addWidget(new QLabel("Envelope - Release"));
    mainLayout->addWidget(releaseSlider);

    // 效果器
    QPushButton *addEffectButton = new QPushButton("Add Effect", this);
    QPushButton *removeEffectButton = new QPushButton("Remove Effect", this);
    mainLayout->addWidget(addEffectButton);
    mainLayout->addWidget(removeEffectButton);
    effectsLayout = new QVBoxLayout();
    mainLayout->addLayout(effectsLayout);

    setLayout(mainLayout);
}

void SynthesizerUI::setupConnections() {
    connect(waveformSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SynthesizerUI::updateOscillatorWaveform);
    for (size_t i = 0; i < oscillatorWeightSliders.size(); ++i) {
        connect(oscillatorWeightSliders[i], &QSlider::valueChanged, this, [this, i]() {
            updateOscillatorWeight(i);
        });
    }
    connect(filterFrequencySlider, &QSlider::valueChanged, this, &SynthesizerUI::updateFilterParams);
    connect(filterQSlider, &QSlider::valueChanged, this, &SynthesizerUI::updateFilterParams);
    connect(attackSlider, &QSlider::valueChanged, this, &SynthesizerUI::updateEnvelopeParams);
    connect(decaySlider, &QSlider::valueChanged, this, &SynthesizerUI::updateEnvelopeParams);
    connect(sustainSlider, &QSlider::valueChanged, this, &SynthesizerUI::updateEnvelopeParams);
    connect(releaseSlider, &QSlider::valueChanged, this, &SynthesizerUI::updateEnvelopeParams);
}

void SynthesizerUI::keyPressEvent(QKeyEvent *event) {
    if (event->type() == QKeyEvent::KeyPress) {
        QString note = QKeySequence(event->key()).toString(); // Convert key to note
        synthesizer->noteOn(getNoteFromKey(note.toStdString()), 1.0);
    }
}

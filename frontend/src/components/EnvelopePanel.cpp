#include "EnvelopePanel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDial>

EnvelopePanel::EnvelopePanel(std::shared_ptr<Synthesizer> synthesizer, QWidget* parent)
    : QWidget(parent), synthesizer(std::move(synthesizer)) {
    setupUI();
    connectUI();
}

void EnvelopePanel::setupUI() {
    auto* layout = new QVBoxLayout(this);

    // Attack Dial
    auto* attackLabel = new QLabel("Attack", this);
    attackDial = new QDial(this);
    attackDial->setRange(0, 100);
    layout->addWidget(attackLabel);
    layout->addWidget(attackDial);

    // Decay Dial
    auto* decayLabel = new QLabel("Decay", this);
    decayDial = new QDial(this);
    decayDial->setRange(0, 100);
    layout->addWidget(decayLabel);
    layout->addWidget(decayDial);

    // Sustain Dial
    auto* sustainLabel = new QLabel("Sustain", this);
    sustainDial = new QDial(this);
    sustainDial->setRange(0, 100);
    layout->addWidget(sustainLabel);
    layout->addWidget(sustainDial);

    // Release Dial
    auto* releaseLabel = new QLabel("Release", this);
    releaseDial = new QDial(this);
    releaseDial->setRange(0, 100);
    layout->addWidget(releaseLabel);
    layout->addWidget(releaseDial);

    setLayout(layout);
}

void EnvelopePanel::connectUI() {
    connect(attackDial, &QDial::valueChanged, this, [this](int value) {
        synthesizer->getEnvelope()->setAttack(value / 100.0);
    });

    connect(decayDial, &QDial::valueChanged, this, [this](int value) {
        synthesizer->getEnvelope()->setDecay(value / 100.0);
    });

    connect(sustainDial, &QDial::valueChanged, this, [this](int value) {
        synthesizer->getEnvelope()->setSustain(value / 100.0);
    });

    connect(releaseDial, &QDial::valueChanged, this, [this](int value) {
        synthesizer->getEnvelope()->setRelease(value / 100.0);
    });
}

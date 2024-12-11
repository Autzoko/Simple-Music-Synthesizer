#include "FilterPanel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QDial>

FilterPanel::FilterPanel(std::shared_ptr<Synthesizer> synthesizer, QWidget* parent)
    : QWidget(parent), synthesizer(std::move(synthesizer)) {
    setupUI();
    connectUI();
}

void FilterPanel::setupUI() {
    auto* layout = new QVBoxLayout(this);

    // Filter Type ComboBox
    auto* typeLabel = new QLabel("Filter Type", this);
    filterTypeComboBox = new QComboBox(this);
    filterTypeComboBox->addItem("Low Pass");
    filterTypeComboBox->addItem("High Pass");
    filterTypeComboBox->addItem("Band Pass");
    filterTypeComboBox->setCurrentIndex(0);
    layout->addWidget(typeLabel);
    layout->addWidget(filterTypeComboBox);

    // Cutoff Dial
    auto* cutoffLabel = new QLabel("Cutoff Frequency", this);
    cutoffDial = new QDial(this);
    cutoffDial->setRange(20, 20000); // Frequency range in Hz
    cutoffDial->setValue(1000);
    layout->addWidget(cutoffLabel);
    layout->addWidget(cutoffDial);

    // Resonance Dial
    auto* resonanceLabel = new QLabel("Resonance", this);
    resonanceDial = new QDial(this);
    resonanceDial->setRange(0, 100); // Resonance range (scaled)
    resonanceDial->setValue(100);
    layout->addWidget(resonanceLabel);
    layout->addWidget(resonanceDial);

    setLayout(layout);
}

void FilterPanel::connectUI() {
    auto filter = synthesizer->getFilter();

    if (!filter) {
        qDebug() << "Filter is null. Initializing...";
        synthesizer->setFilter(new Filter());
        qDebug() << "Filter initialized";
    }
    connect(filterTypeComboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        FilterType type = static_cast<FilterType>(index);
        synthesizer->getFilter()->setFilterType(type);
    });


    connect(cutoffDial, &QDial::valueChanged, this, [this](int value) {
        synthesizer->getFilter()->setCutoff(value);
    });

    connect(resonanceDial, &QDial::valueChanged, this, [this](int value) {
        synthesizer->getFilter()->setResonance(value / 100.0); // Normalize resonance
    });
}

#ifndef ENVELOPE_PANEL_H
#define ENVELOPE_PANEL_H

#include <QWidget>
#include <memory>
#include <QDial>
#include "Synthesizer.h"

class EnvelopePanel : public QWidget {
    Q_OBJECT

public:
    EnvelopePanel(std::shared_ptr<Synthesizer> synthesizer, QWidget* parent = nullptr);

private:
    std::shared_ptr<Synthesizer> synthesizer;

    // Add UI components like sliders or dials for ADSR
    // Example:
    QDial* attackDial;
    QDial* decayDial;
    QDial* sustainDial;
    QDial* releaseDial;

    void setupUI();
    void connectUI();
};

#endif // ENVELOPE_PANEL_H

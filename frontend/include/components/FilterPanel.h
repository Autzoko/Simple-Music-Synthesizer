#ifndef FILTER_PANEL_H
#define FILTER_PANEL_H

#include <QWidget>
#include <memory>
#include <QComboBox>
#include <QDial>
#include <QLabel>
#include "Synthesizer.h"

class FilterPanel : public QWidget {
    Q_OBJECT

public:
    FilterPanel(std::shared_ptr<Synthesizer> synthesizer, QWidget* parent = nullptr);

private:
    std::shared_ptr<Synthesizer> synthesizer;

    // UI components for filter settings
    QComboBox* filterTypeComboBox;
    QDial* cutoffDial;
    QDial* resonanceDial;

    void setupUI();
    void connectUI();
};

#endif // FILTER_PANEL_H

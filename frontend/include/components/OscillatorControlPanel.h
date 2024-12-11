#ifndef OSCILLATOR_CONTROL_PANEL_H
#define OSCILLATOR_CONTROL_PANEL_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDial>
#include <QLabel>

class OscillatorControlPanel : public QWidget {
    Q_OBJECT

public:
    explicit OscillatorControlPanel(int index, QWidget* parent = nullptr);
    ~OscillatorControlPanel();

signals:
    void waveformChanged(int index, int waveformId);
    void weightChanged(int index, double weight);
    void detuneChanged(int index, double detune);

private:
    int index;
    QButtonGroup* waveformGroup;
    QDial* weightDial;
    QDial* detuneDial;

    QLabel* weightValueLabel;
    QLabel* detuneValueLabel;

    void setupUI();
    void setupConnections();
};

#endif
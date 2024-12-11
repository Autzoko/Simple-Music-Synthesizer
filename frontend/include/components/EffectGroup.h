#ifndef EFFECT_GROUP_H
#define EFFECT_GROUP_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QPushButton>
#include <QMap>
#include <QLabel>
#include <QGroupBox>
#include <memory>
#include "Synthesizer.h"

#include "Chorus.h"
#include "Delay.h"
#include "Distortion.h"
#include "Equilizer.h"
#include "PitchShifter.h"
#include "Reverb.h"
#include "Tremolo.h"

class EffectGroup : public QWidget {
    Q_OBJECT

public:
    explicit EffectGroup(std::shared_ptr<Synthesizer> synthesizer, QWidget* parent = nullptr);

private:
    std::shared_ptr<Synthesizer> synthesizer;
    QVBoxLayout* mainLayout;
    QMap<QString, std::shared_ptr<Effect>> effectMap;

    void addEffectUI(const QString& effectName, const std::vector<QString>& paramNames);

private slots:
    void onEffectToggled(const QString& effectName, bool enabled);
    void onEffectParamChanged(const QString& effectName, const QString& paramName, int value);
};

#endif // EFFECT_GROUP_H

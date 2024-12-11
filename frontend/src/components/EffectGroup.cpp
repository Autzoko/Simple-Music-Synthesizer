#include "EffectGroup.h"

EffectGroup::EffectGroup(std::shared_ptr<Synthesizer> synthesizer, QWidget* parent)
    : QWidget(parent), synthesizer(std::move(synthesizer)) {
    mainLayout = new QVBoxLayout(this);

    // Add individual effect UI components
    addEffectUI("Chorus", {"Depth", "Rate"});
    addEffectUI("Delay", {"DelayTime", "Feedback", "Mix"});
    addEffectUI("Distortion", {"Threshold"});
    addEffectUI("Equilizer", {"Band1Gain", "Band2Gain", "Band3Gain"}); // Example: 3 bands
    addEffectUI("PitchShifter", {"PitchFactor"});
    addEffectUI("Reverb", {"Decay", "Mix"});
    addEffectUI("Tremolo", {"Depth", "Rate"});

    setLayout(mainLayout);
}

void EffectGroup::addEffectUI(const QString& effectName, const std::vector<QString>& paramNames) {
    // Create a group for this effect
    QGroupBox* effectBox = new QGroupBox(effectName, this);
    QVBoxLayout* effectLayout = new QVBoxLayout(effectBox);

    // Enable/Disable checkbox
    QCheckBox* enableCheckBox = new QCheckBox("Enable", effectBox);
    effectLayout->addWidget(enableCheckBox);

    // Sliders for parameters
    QMap<QString, QSlider*> sliders;
    for (const auto& paramName : paramNames) {
        QLabel* label = new QLabel(paramName, effectBox);
        QSlider* slider = new QSlider(Qt::Horizontal, effectBox);
        slider->setRange(0, 100); // Normalize to 0-100 for UI, convert to actual range in slot
        sliders[paramName] = slider;

        effectLayout->addWidget(label);
        effectLayout->addWidget(slider);

        connect(slider, &QSlider::valueChanged, this, [this, effectName, paramName](int value) {
            onEffectParamChanged(effectName, paramName, value);
        });
    }

    connect(enableCheckBox, &QCheckBox::toggled, this, [this, effectName](bool enabled) {
        onEffectToggled(effectName, enabled);
    });

    mainLayout->addWidget(effectBox);
}

void EffectGroup::onEffectToggled(const QString& effectName, bool enabled) {
    auto effect = effectMap.value(effectName, nullptr);

    if (enabled) {
        if (!effect) {
            // Add the effect to the synthesizer
            if (effectName == "Chorus") {
                effect = std::make_shared<ChorusEffect>();
            } else if (effectName == "Delay") {
                effect = std::make_shared<DelayEffect>();
            } else if (effectName == "Distortion") {
                effect = std::make_shared<Distortion>();
            } else if (effectName == "Equilizer") {
                std::vector<Equilizer::Band> bands = {
                    {100.0}, {1000.0}, {5000.0} // Example frequencies
                };
                effect = std::make_shared<Equilizer>(44100, bands);
            } else if (effectName == "PitchShifter") {
                effect = std::make_shared<PitchShifter>(44100);
            } else if (effectName == "Reverb") {
                effect = std::make_shared<ReverbEffect>();
            } else if (effectName == "Tremolo") {
                effect = std::make_shared<TremoloEffect>();
            }

            if (effect) {
                synthesizer->addEffect(effect);
                effectMap.insert(effectName, effect);
            }
        }
    } else {
        // Remove the effect from the synthesizer
        auto it = effectMap.find(effectName);
        if (it != effectMap.end()) {
            size_t index = std::distance(effectMap.begin(), it);
            synthesizer->removeEffect(index);
            effectMap.remove(effectName);
        }
    }
}

void EffectGroup::onEffectParamChanged(const QString& effectName, const QString& paramName, int value) {
    auto effect = effectMap.value(effectName, nullptr);
    if (!effect) return;

    double normalizedValue = value / 100.0; // Normalize 0-100 to 0.0-1.0
    if (effectName == "Chorus") {
        auto chorus = std::dynamic_pointer_cast<ChorusEffect>(effect);
        if (paramName == "Depth") {
            chorus->setDepth(normalizedValue);
        } else if (paramName == "Rate") {
            chorus->setRate(normalizedValue * 10.0); // Example scaling
        }
    } else if (effectName == "Delay") {
        auto delay = std::dynamic_pointer_cast<DelayEffect>(effect);
        if (paramName == "DelayTime") {
            delay->setDelayTime(normalizedValue);
        } else if (paramName == "Feedback") {
            delay->setFeedback(normalizedValue);
        } else if (paramName == "Mix") {
            delay->setMix(normalizedValue);
        }
    } else if (effectName == "Distortion") {
        auto distortion = std::dynamic_pointer_cast<Distortion>(effect);
        if (paramName == "Threshold") {
            distortion->setThreshold(normalizedValue);
        }
    } else if (effectName == "Equilizer") {
        auto equilizer = std::dynamic_pointer_cast<Equilizer>(effect);
        if (paramName.startsWith("Band")) {
            int bandIndex = paramName.mid(4, 1).toInt() - 1; // Extract band index
            equilizer->setBandGain(bandIndex, normalizedValue * 12.0 - 6.0); // Example scaling
        }
    } else if (effectName == "PitchShifter") {
        auto pitchShifter = std::dynamic_pointer_cast<PitchShifter>(effect);
        if (paramName == "PitchFactor") {
            pitchShifter->setPitchFactor(normalizedValue * 2.0); // Example scaling
        }
    } else if (effectName == "Reverb") {
        auto reverb = std::dynamic_pointer_cast<ReverbEffect>(effect);
        if (paramName == "Decay") {
            reverb->setDecay(normalizedValue);
        } else if (paramName == "Mix") {
            reverb->setMix(normalizedValue);
        }
    } else if (effectName == "Tremolo") {
        auto tremolo = std::dynamic_pointer_cast<TremoloEffect>(effect);
        if (paramName == "Depth") {
            tremolo->setDepth(normalizedValue);
        } else if (paramName == "Rate") {
            tremolo->setRate(normalizedValue * 10.0); // Example scaling
        }
    }
}

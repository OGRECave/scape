#include "BrushSettingsWidget.h"

#include <limits>

const int BrushSettingsWidget::RADIUS_SLIDER_MIN = 1;
const int BrushSettingsWidget::RADIUS_SLIDER_MAX = 1024;
const int BrushSettingsWidget::RAMP_POWER_SLIDER_MIN = 1;
const int BrushSettingsWidget::RAMP_POWER_SLIDER_MAX = 25;

const double BrushSettingsWidget::RADIUS_SPINBOX_MIN = 0.1;
const double BrushSettingsWidget::RADIUS_SPINBOX_MAX = std::numeric_limits<double>::infinity();
const double BrushSettingsWidget::RAMP_POWER_SPINBOX_MIN = 0.1;
const double BrushSettingsWidget::RAMP_POWER_SPINBOX_MAX = 25.0;

BrushSettingsWidget::BrushSettingsWidget(QWidget* parent, ScapeEngine::HeightfieldBrushSettings& settings)
    : QWidget(parent), mSettings(settings)
{
    mBrushSettingsWidgetUI = new Ui::BrushSettingsWidget();
    mBrushSettingsWidgetUI->setupUi(this);

    setupWidget();
    connectActions();
}

BrushSettingsWidget::~BrushSettingsWidget() { delete mBrushSettingsWidgetUI; }

void BrushSettingsWidget::update()
{
    mBrushSettingsWidgetUI->radiusSpinBox->setValue(mSettings.getOuterRadius());
    mBrushSettingsWidgetUI->innerRadiusSpinBox->setValue(mSettings.getInnerRadius());
    mBrushSettingsWidgetUI->outerRadiusSpinBox->setValue(mSettings.getOuterRadius());
    mBrushSettingsWidgetUI->rampPowerSpinBox->setValue(mSettings.getRampPower());

    mBrushSettingsWidgetUI->radiusSlider->setValue(std::max(
        std::min((int)std::round(mSettings.getOuterRadius()), RADIUS_SLIDER_MAX), RADIUS_SLIDER_MIN));
    mBrushSettingsWidgetUI->innerRadiusSlider->setValue(std::max(
        std::min((int)std::round(mSettings.getInnerRadius()), RADIUS_SLIDER_MAX), RADIUS_SLIDER_MIN));
    mBrushSettingsWidgetUI->outerRadiusSlider->setValue(std::max(
        std::min((int)std::round(mSettings.getOuterRadius()), RADIUS_SLIDER_MAX), RADIUS_SLIDER_MIN));
    mBrushSettingsWidgetUI->rampPowerSlider->setValue(std::max(
        std::min((int)std::round(mSettings.getRampPower()), RAMP_POWER_SLIDER_MAX), RAMP_POWER_SLIDER_MIN));
}

void BrushSettingsWidget::spinBoxValueChanged()
{
    QObject* sender = QObject::sender();
    if (sender == mBrushSettingsWidgetUI->radiusSpinBox)
    {
        mSettings.setRadius(mBrushSettingsWidgetUI->radiusSpinBox->value());
    }
    else if (sender == mBrushSettingsWidgetUI->innerRadiusSpinBox)
    {
        mSettings.setInnerRadius(mBrushSettingsWidgetUI->innerRadiusSpinBox->value());
    }
    else if (sender == mBrushSettingsWidgetUI->outerRadiusSpinBox)
    {
        mSettings.setOuterRadius(mBrushSettingsWidgetUI->outerRadiusSpinBox->value());
    }
    else if (sender == mBrushSettingsWidgetUI->rampPowerSpinBox)
    {
        mSettings.setRampPower(mBrushSettingsWidgetUI->rampPowerSpinBox->value());
    }
}

void BrushSettingsWidget::sliderValueChanged()
{
    QObject* sender = QObject::sender();
    if (sender == mBrushSettingsWidgetUI->radiusSlider)
    {
        mSettings.setRadius(mBrushSettingsWidgetUI->radiusSlider->value());
    }
    else if (sender == mBrushSettingsWidgetUI->innerRadiusSlider)
    {
        mSettings.setInnerRadius(mBrushSettingsWidgetUI->innerRadiusSlider->value());
    }
    else if (sender == mBrushSettingsWidgetUI->outerRadiusSlider)
    {
        mSettings.setOuterRadius(mBrushSettingsWidgetUI->outerRadiusSlider->value());
    }
    else if (sender == mBrushSettingsWidgetUI->rampPowerSlider)
    {
        mSettings.setRampPower(mBrushSettingsWidgetUI->rampPowerSlider->value());
    }
}

void BrushSettingsWidget::setupWidget()
{
    mBrushSettingsWidgetUI->radiusSlider->setMinimum(RADIUS_SLIDER_MIN);
    mBrushSettingsWidgetUI->radiusSlider->setMaximum(RADIUS_SLIDER_MAX);

    mBrushSettingsWidgetUI->radiusSpinBox->setMinimum(RADIUS_SPINBOX_MIN);
    mBrushSettingsWidgetUI->radiusSpinBox->setMaximum(RADIUS_SPINBOX_MAX);

    mBrushSettingsWidgetUI->innerRadiusSlider->setMinimum(RADIUS_SLIDER_MIN);
    mBrushSettingsWidgetUI->innerRadiusSlider->setMaximum(RADIUS_SLIDER_MAX);

    mBrushSettingsWidgetUI->innerRadiusSpinBox->setMinimum(RADIUS_SPINBOX_MIN);
    mBrushSettingsWidgetUI->innerRadiusSpinBox->setMaximum(RADIUS_SPINBOX_MAX);

    mBrushSettingsWidgetUI->outerRadiusSlider->setMinimum(RADIUS_SLIDER_MIN);
    mBrushSettingsWidgetUI->outerRadiusSlider->setMaximum(RADIUS_SLIDER_MAX);

    mBrushSettingsWidgetUI->outerRadiusSpinBox->setMinimum(RADIUS_SPINBOX_MIN);
    mBrushSettingsWidgetUI->outerRadiusSpinBox->setMaximum(RADIUS_SPINBOX_MAX);

    mBrushSettingsWidgetUI->rampPowerSlider->setMinimum(RAMP_POWER_SLIDER_MIN);
    mBrushSettingsWidgetUI->rampPowerSlider->setMaximum(RAMP_POWER_SLIDER_MAX);

    mBrushSettingsWidgetUI->rampPowerSpinBox->setMinimum(RAMP_POWER_SPINBOX_MIN);
    mBrushSettingsWidgetUI->rampPowerSpinBox->setMaximum(RAMP_POWER_SPINBOX_MAX);
}

void BrushSettingsWidget::connectActions()
{
    connect(mBrushSettingsWidgetUI->radiusSlider, SIGNAL(valueChanged(int)), this,
            SLOT(sliderValueChanged()));
    connect(mBrushSettingsWidgetUI->innerRadiusSlider, SIGNAL(valueChanged(int)), this,
            SLOT(sliderValueChanged()));
    connect(mBrushSettingsWidgetUI->outerRadiusSlider, SIGNAL(valueChanged(int)), this,
            SLOT(sliderValueChanged()));
    connect(mBrushSettingsWidgetUI->rampPowerSlider, SIGNAL(valueChanged(int)), this,
            SLOT(sliderValueChanged()));

    connect(mBrushSettingsWidgetUI->radiusSpinBox, SIGNAL(editingFinished()), this,
            SLOT(spinBoxValueChanged()));
    connect(mBrushSettingsWidgetUI->innerRadiusSpinBox, SIGNAL(editingFinished()), this,
            SLOT(spinBoxValueChanged()));
    connect(mBrushSettingsWidgetUI->outerRadiusSpinBox, SIGNAL(editingFinished()), this,
            SLOT(spinBoxValueChanged()));
    connect(mBrushSettingsWidgetUI->rampPowerSpinBox, SIGNAL(editingFinished()), this,
            SLOT(spinBoxValueChanged()));

    mSettings.addObserver(this);
    update();
}

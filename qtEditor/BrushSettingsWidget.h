#ifndef BRUSHSETTINGSWIDGET_H
#define BRUSHSETTINGSWIDGET_H

#include <QWidget>
#include "ui_brushsettingswidget.h"

#include "Utils/Observer.h"
#include "HeightfieldBrush/HeightfieldBrushSettings.h"

class BrushSettingsWidget : public QWidget, public ScapeEngine::Observer
{
    Q_OBJECT

public:
    BrushSettingsWidget(QWidget* parent, ScapeEngine::HeightfieldBrushSettings& settings);
    virtual ~BrushSettingsWidget();

    virtual void update();

protected:
    Ui::BrushSettingsWidget* mBrushSettingsWidgetUI;

    ScapeEngine::HeightfieldBrushSettings& mSettings;

protected slots:
    void spinBoxValueChanged();
    void sliderValueChanged();

private:
    static const int RADIUS_SLIDER_MIN;
    static const int RADIUS_SLIDER_MAX;
    static const int RAMP_POWER_SLIDER_MIN;
    static const int RAMP_POWER_SLIDER_MAX;

    static const double RADIUS_SPINBOX_MIN;
    static const double RADIUS_SPINBOX_MAX;
    static const double RAMP_POWER_SPINBOX_MIN;
    static const double RAMP_POWER_SPINBOX_MAX;

    void setupWidget();
    void connectActions();
};

#endif /* BRUSHSETTINGSWIDGET_H */

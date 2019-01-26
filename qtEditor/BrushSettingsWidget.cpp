#include "BrushSettingsWidget.h"

BrushSettingsWidget::BrushSettingsWidget(QWidget* parent) : QWidget(parent)
{
    mBrushSettingsWidgetUI = new Ui::BrushSettingsWidget();
    mBrushSettingsWidgetUI->setupUi(this);
}

BrushSettingsWidget::~BrushSettingsWidget() { delete mBrushSettingsWidgetUI; }

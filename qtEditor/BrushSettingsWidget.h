#ifndef BRUSHSETTINGSWIDGET_H
#define BRUSHSETTINGSWIDGET_H

#include <QWidget>
#include "ui_brushsettingswidget.h"

class BrushSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    BrushSettingsWidget(QWidget* parent = NULL);
    virtual ~BrushSettingsWidget();

protected:
    Ui::BrushSettingsWidget* mBrushSettingsWidgetUI;
};

#endif /* BRUSHSETTINGSWIDGET_H */

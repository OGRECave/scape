#ifndef PRESETSWIDGET_H
#define PRESETSWIDGET_H

#include <QWidget>

namespace Ui {
class PresetsWidget;
}

class PresetsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PresetsWidget(QWidget *parent = 0);
    ~PresetsWidget();

private:
    Ui::PresetsWidget *ui;
};

#endif // PRESETSWIDGET_H

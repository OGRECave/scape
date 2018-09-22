#ifndef PRESETSWIDGET_H
#define PRESETSWIDGET_H

#include <QWidget>
#include <QToolBar>

namespace Ui
{
class PresetsWidget;
}

class PresetsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PresetsWidget(QWidget* parent = 0);
    ~PresetsWidget();

    QToolBar* getToolBar() const;

private:
    Ui::PresetsWidget* ui;

    QToolBar* mToolBar;
};

#endif // PRESETSWIDGET_H

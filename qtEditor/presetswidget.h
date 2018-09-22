#ifndef PRESETSWIDGET_H
#define PRESETSWIDGET_H

#include <QWidget>
#include <QToolBar>
#include <QListWidget>

#include <vector>
#include <string>

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

    void populate(const std::vector<std::string>& presets);

signals:
    void presetLoading(const std::string& preset);

private:
    Ui::PresetsWidget* ui;

    QToolBar* mToolBar;

    std::vector<std::string> mPresets;
};

#endif // PRESETSWIDGET_H

#include "presetswidget.h"
#include "ui_presetswidget.h"

PresetsWidget::PresetsWidget(QWidget* parent) : QWidget(parent), ui(new Ui::PresetsWidget)
{
    ui->setupUi(this);

    mToolBar = new QToolBar();
    ui->mainLayout->insertWidget(0, mToolBar);
}

PresetsWidget::~PresetsWidget() { delete ui; }

QToolBar* PresetsWidget::getToolBar() const
{
    return mToolBar;
}

void PresetsWidget::populate(const std::vector<std::string>& presets)
{
    ui->presetsListWidget->clear();
    mPresets = presets;
    for (std::vector<std::string>::const_iterator it = mPresets.begin(); it != mPresets.end(); it++)
    {
        ui->presetsListWidget->addItem(QString(it->c_str()));
    }
}

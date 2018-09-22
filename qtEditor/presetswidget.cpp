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

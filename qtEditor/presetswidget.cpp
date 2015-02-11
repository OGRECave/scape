#include "presetswidget.h"
#include "ui_presetswidget.h"

PresetsWidget::PresetsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PresetsWidget)
{
    ui->setupUi(this);
}

PresetsWidget::~PresetsWidget()
{
    delete ui;
}

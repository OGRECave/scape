#include "AboutDialog.h"
#include "ui_aboutdialog.h"
#include "ScapeVersion.h"

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->titleLabel->setText("Scape " SCAPE_VERSION_NAME);
}

AboutDialog::~AboutDialog() { delete ui; }

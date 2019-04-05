#include "StartupSettingsDialog.h"

#include "EngineCore/StartupSettings.h"

#include <limits>

StartupSettingsDialog::StartupSettingsDialog(
    QWidget* parent, ScapeEngine::StartupSettingsDataAccessObject& startupSettingsDataAccessObject)
    : QDialog(parent), mStartupSettingsDataAccessObject(startupSettingsDataAccessObject)
{
    ui = new Ui::StartupSettingsDialog();
    ui->setupUi(this);

    setupWidget();
    connectActions();
    initValues();
}

StartupSettingsDialog::~StartupSettingsDialog() { delete ui; }

void StartupSettingsDialog::setupWidget()
{
    ui->heightfieldColumnsSpinBox->setMinimum(1);
    ui->heightfieldColumnsSpinBox->setMaximum(std::numeric_limits<int>::max());

    ui->heightfieldRowsSpinBox->setMinimum(1);
    ui->heightfieldRowsSpinBox->setMaximum(std::numeric_limits<int>::max());

    ui->heightfieldHeightDoubleSpinBox->setMinimum(0.0);
    ui->heightfieldHeightDoubleSpinBox->setMaximum(std::numeric_limits<double>::infinity());
}

void StartupSettingsDialog::connectActions()
{
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clicked(QAbstractButton*)));
}

void StartupSettingsDialog::initValues()
{
    ScapeEngine::StartupSettings startupSettings = mStartupSettingsDataAccessObject.getStartupSettings();
    ui->heightfieldColumnsSpinBox->setValue(startupSettings.getHeightfieldColumns());
    ui->heightfieldRowsSpinBox->setValue(startupSettings.getHeightfieldRows());
    ui->heightfieldHeightDoubleSpinBox->setValue(startupSettings.getHeightfieldHeight());
}

void StartupSettingsDialog::clicked(QAbstractButton* button)
{
    if (button == (QAbstractButton*)ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
    {
        ui->heightfieldColumnsSpinBox->setValue(4096);
        ui->heightfieldRowsSpinBox->setValue(4096);
        ui->heightfieldHeightDoubleSpinBox->setValue(500.0);
    }
    else if (button == (QAbstractButton*)ui->buttonBox->button(QDialogButtonBox::Apply))
    {
        ScapeEngine::StartupSettings startupSettings;
        startupSettings.setHeightfieldColumns(ui->heightfieldColumnsSpinBox->value());
        startupSettings.setHeightfieldRows(ui->heightfieldRowsSpinBox->value());
        startupSettings.setHeightfieldHeight(ui->heightfieldHeightDoubleSpinBox->value());

        mStartupSettingsDataAccessObject.updateStartupSettings(startupSettings);

        accept();
    }
}

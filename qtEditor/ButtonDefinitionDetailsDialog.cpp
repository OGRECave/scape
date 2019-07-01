#include "ButtonDefinitionDetailsDialog.h"

ButtonDefinitionDetailsDialog::ButtonDefinitionDetailsDialog(
    QWidget* parent, ScapeEngine::ButtonDefinition::DeviceButtonSet deviceButtonsSet)
    : QDialog(parent), mDeviceButtonsSet(deviceButtonsSet)
{
    ui = new Ui::ButtonDefinitionDetailsWidget();
    ui->setupUi(this);

    connectActions();

    updateLists();
}

ButtonDefinitionDetailsDialog::~ButtonDefinitionDetailsDialog() { delete ui; }

ScapeEngine::ButtonDefinition::DeviceButtonSet ButtonDefinitionDetailsDialog::getDeviceButtonsSet()
{
    return mDeviceButtonsSet;
}

void ButtonDefinitionDetailsDialog::moveToLeftButtonClicked()
{
    if (ui->activeListWidget->currentItem() != NULL)
    {
        QListWidgetItem* currentItem = ui->activeListWidget->currentItem();
        std::string currentItemText = currentItem->text().toStdString();

        std::map<ScapeEngine::DeviceButtonId::EDeviceButtonId, std::string> buttons =
            ScapeEngine::DeviceButtonId::getDeviceButtonIdMap();

        for (std::map<ScapeEngine::DeviceButtonId::EDeviceButtonId, std::string>::iterator it =
                 buttons.begin();
             it != buttons.end(); it++)
        {
            if (it->second == currentItemText)
            {
                mDeviceButtonsSet.erase(it->first);
                break;
            }
        }
    }
    updateLists();
}

void ButtonDefinitionDetailsDialog::moveToRightButtonClicked()
{
    if (ui->inactiveListWidget->selectedItems().size() == 1)
    {
        QListWidgetItem* currentItem = ui->inactiveListWidget->selectedItems()[0];
        std::string currentItemText = currentItem->text().toStdString();

        std::map<ScapeEngine::DeviceButtonId::EDeviceButtonId, std::string> buttons =
            ScapeEngine::DeviceButtonId::getDeviceButtonIdMap();

        for (std::map<ScapeEngine::DeviceButtonId::EDeviceButtonId, std::string>::iterator it =
                 buttons.begin();
             it != buttons.end(); it++)
        {
            if (it->second == currentItemText)
            {
                mDeviceButtonsSet.insert(it->first);
                break;
            }
        }
    }
    updateLists();
}

void ButtonDefinitionDetailsDialog::connectActions()
{
    connect(ui->moveToLeftButton, SIGNAL(clicked(bool)), this, SLOT(moveToLeftButtonClicked()));
    connect(ui->moveToRightButton, SIGNAL(clicked(bool)), this, SLOT(moveToRightButtonClicked()));

    connect(ui->mainButtonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clicked(QAbstractButton*)));
    connect(ui->mainButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ButtonDefinitionDetailsDialog::updateLists()
{
    ui->activeListWidget->clear();
    ui->inactiveListWidget->clear();

    std::map<ScapeEngine::DeviceButtonId::EDeviceButtonId, std::string> buttons =
        ScapeEngine::DeviceButtonId::getDeviceButtonIdMap();

    for (std::map<ScapeEngine::DeviceButtonId::EDeviceButtonId, std::string>::iterator it = buttons.begin();
         it != buttons.end(); it++)
    {
        if (mDeviceButtonsSet.find(it->first) != mDeviceButtonsSet.end())
        {
            ui->activeListWidget->addItem(QString(it->second.c_str()));
        }
        else
        {
            ui->inactiveListWidget->addItem(QString(it->second.c_str()));
        }
    }
}

void ButtonDefinitionDetailsDialog::clicked(QAbstractButton* button)
{
    if (button == (QAbstractButton*)ui->mainButtonBox->button(QDialogButtonBox::Apply))
    {
        accept();
    }
}

#include "ButtonDefinitionsDialog.h"

#include "ButtonDefinitionDetailsDialog.h"

ButtonDefinitionsDialog::ButtonDefinitionsDialog(
    QWidget* parent, ScapeEngine::ButtonDefinitionDataAccessObject& buttonDefinitionDataAccessObject)
    : QDialog(parent), mButtonDefinitionDataAccessObject(buttonDefinitionDataAccessObject)
{
    ui = new Ui::ButtonDefinitionsWidget();
    ui->setupUi(this);

    setupWidget();
    initValues();
    connectActions();

    ui->definitionComboBox->setCurrentIndex(0);
    definitionChanged(ui->definitionComboBox->currentIndex());
}

ButtonDefinitionsDialog::~ButtonDefinitionsDialog() { delete ui; }

void ButtonDefinitionsDialog::setupWidget()
{
    mToolBar = new QToolBar();
    ui->buttonDefinitionDetailsLayout->insertWidget(1, mToolBar);

    mCreateAction = new QAction(tr("Create button definition detail"), this);
    mCreateAction->setStatusTip(tr("Create button definition detail"));
    mCreateAction->setIcon(QIcon(":/icons/plus"));

    mEditAction = new QAction(tr("Edit button definition detail"), this);
    mEditAction->setStatusTip(tr("Edit button definition detail"));
    mEditAction->setIcon(QIcon(":/icons/edit"));

    mDeleteAction = new QAction(tr("Delete button definition detail"), this);
    mDeleteAction->setStatusTip(tr("Delete button definition detail"));
    mDeleteAction->setIcon(QIcon(":/icons/trash"));

    mToolBar->addAction(mCreateAction);
    mToolBar->addAction(mEditAction);
    mToolBar->addAction(mDeleteAction);
}

void ButtonDefinitionsDialog::connectActions()
{
    connect(ui->prioritySpinBox, SIGNAL(valueChanged(int)), this, SLOT(priorityChanged(int)));

    connect(mCreateAction, SIGNAL(triggered()), this, SLOT(createDefinitionDetail()));
    connect(mEditAction, SIGNAL(triggered()), this, SLOT(editDefinitionDetail()));
    connect(ui->buttonDefinitionDetailsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
            SLOT(editDefinitionDetail()));
    connect(mDeleteAction, SIGNAL(triggered()), this, SLOT(deleteDefinitionDetail()));

    connect(ui->definitionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(definitionChanged(int)));

    connect(ui->mainButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->mainButtonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clicked(QAbstractButton*)));
}

void ButtonDefinitionsDialog::initValues()
{
    std::map<ScapeEngine::ButtonId::EButtonId, std::string> buttons =
        ScapeEngine::ButtonId::getButtonIdMap();

    for (std::map<ScapeEngine::ButtonId::EButtonId, std::string>::iterator it = buttons.begin();
         it != buttons.end(); it++)
    {
        ui->definitionComboBox->addItem(QString(it->second.c_str()));
        mComboBoxButtons.push_back(it->first);
    }

    ScapeEngine::ButtonDefinitionDataAccessObject::ButtonDefinitions buttonDefinitions =
        mButtonDefinitionDataAccessObject.getButtonDefinitions();
    for (ScapeEngine::ButtonDefinitionDataAccessObject::ButtonDefinitions::iterator it =
             buttonDefinitions.begin();
         it != buttonDefinitions.end(); it++)
    {
        mButtonDefinitions.insert(std::make_pair(it->getButtonId(), *it));
    }
}

void ButtonDefinitionsDialog::updateButtonDefinitionDetailsList()
{
    ui->buttonDefinitionDetailsListWidget->clear();

    ScapeEngine::ButtonDefinition& currentButtonDefinition = getCurrentButtonDefinition();

    ui->prioritySpinBox->setValue(currentButtonDefinition.getPriority());

    for (ScapeEngine::ButtonDefinition::DeviceButtonSetsVector::iterator deviceButtonSetsIt =
             currentButtonDefinition.getDeviceButtonSets().begin();
         deviceButtonSetsIt != currentButtonDefinition.getDeviceButtonSets().end(); deviceButtonSetsIt++)
    {
        std::string buttonDefinitionDetails = "";

        const ScapeEngine::ButtonDefinition::DeviceButtonSet& deviceButtons = *deviceButtonSetsIt;
        for (ScapeEngine::ButtonDefinition::DeviceButtonSet::const_iterator deviceButtonIt =
                 deviceButtons.begin();
             deviceButtonIt != deviceButtons.end(); deviceButtonIt++)
        {
            std::map<ScapeEngine::DeviceButtonId::EDeviceButtonId, std::string> deviceButtons =
                ScapeEngine::DeviceButtonId::getDeviceButtonIdMap();
            std::map<ScapeEngine::DeviceButtonId::EDeviceButtonId, std::string>::iterator devIt =
                deviceButtons.find((*deviceButtonIt));
            if (devIt != deviceButtons.end())
            {
                buttonDefinitionDetails = buttonDefinitionDetails.length() == 0
                                              ? devIt->second
                                              : buttonDefinitionDetails + "," + devIt->second;
            }
        }

        ui->buttonDefinitionDetailsListWidget->addItem(QString(buttonDefinitionDetails.c_str()));
    }
}

ScapeEngine::ButtonId::EButtonId ButtonDefinitionsDialog::getCurrentButtonId()
{
    return mComboBoxButtons[ui->definitionComboBox->currentIndex()];
}

ScapeEngine::ButtonDefinition& ButtonDefinitionsDialog::getCurrentButtonDefinition()
{
    return mButtonDefinitions.find(getCurrentButtonId())->second;
}

void ButtonDefinitionsDialog::priorityChanged(int value)
{
    getCurrentButtonDefinition().setPriority(ui->prioritySpinBox->value());
}

void ButtonDefinitionsDialog::createDefinitionDetail()
{
    ScapeEngine::ButtonId::EButtonId currentButtonId = getCurrentButtonId();

    ButtonDefinitionDetailsDialog dialog(this, ScapeEngine::ButtonDefinition::DeviceButtonSet());
    if (dialog.exec() == QDialog::Accepted)
    {
        getCurrentButtonDefinition().getDeviceButtonSets().push_back(dialog.getDeviceButtonsSet());
        updateButtonDefinitionDetailsList();
    }
}

void ButtonDefinitionsDialog::editDefinitionDetail()
{
    int index = ui->buttonDefinitionDetailsListWidget->currentRow();
    if (index >= 0)
    {
        ButtonDefinitionDetailsDialog dialog(this,
                                             getCurrentButtonDefinition().getDeviceButtonSets()[index]);
        if (dialog.exec() == QDialog::Accepted)
        {
            getCurrentButtonDefinition().getDeviceButtonSets()[index] = dialog.getDeviceButtonsSet();
            updateButtonDefinitionDetailsList();
        }
    }
}

void ButtonDefinitionsDialog::deleteDefinitionDetail()
{
    int index = ui->buttonDefinitionDetailsListWidget->currentRow();
    if (index >= 0)
    {
        getCurrentButtonDefinition().getDeviceButtonSets().erase(
            getCurrentButtonDefinition().getDeviceButtonSets().begin() + index);
        updateButtonDefinitionDetailsList();
    }
}

void ButtonDefinitionsDialog::definitionChanged(int index) { updateButtonDefinitionDetailsList(); }

void ButtonDefinitionsDialog::clicked(QAbstractButton* button)
{
    if (button == (QAbstractButton*)ui->mainButtonBox->button(QDialogButtonBox::Apply))
    {
        ScapeEngine::ButtonDefinitionDataAccessObject::ButtonDefinitions buttonDefinitions;
        for (ButtonDefinitionsMap::iterator it = mButtonDefinitions.begin(); it != mButtonDefinitions.end();
             it++)
        {
            buttonDefinitions.push_back(it->second);
        }
        mButtonDefinitionDataAccessObject.updateButtonDefinitions(buttonDefinitions);
        accept();
    }
}

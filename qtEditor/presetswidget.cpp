#include "presetswidget.h"
#include "ui_presetswidget.h"

PresetsWidget::PresetsWidget(QWidget* parent) : QWidget(parent), ui(new Ui::PresetsWidget)
{
    ui->setupUi(this);

    mToolBar = new QToolBar();
    ui->mainLayout->insertWidget(0, mToolBar);

    connectActions();
}

PresetsWidget::~PresetsWidget() { delete ui; }

QToolBar* PresetsWidget::getToolBar() const
{
    return mToolBar;
}

std::string PresetsWidget::getSelectedPreset() const
{
    QList<QListWidgetItem*> selectedItems = ui->presetsListWidget->selectedItems();
    if (selectedItems.size() > 0)
    {
        QListWidgetItem* item = selectedItems.at(0);
        for (int i = 0; i < ui->presetsListWidget->count(); i++)
        {
            if (ui->presetsListWidget->item(i) == item)
            {
                return mPresets[i];
            }
        }
    }
    return "";
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

void PresetsWidget::itemDoubleClicked(QListWidgetItem* item)
{
    for (int i = 0; i < ui->presetsListWidget->count(); i++)
    {
        if (ui->presetsListWidget->item(i) == item)
        {
            emit presetLoading(mPresets[i]);
            break;
        }
    }
}

void PresetsWidget::connectActions()
{
    connect(ui->presetsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
            SLOT(itemDoubleClicked(QListWidgetItem*)));
}

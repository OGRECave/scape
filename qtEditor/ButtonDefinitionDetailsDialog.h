#ifndef BUTTONDEFINITIONDETAILSDIALOG_H
#define BUTTONDEFINITIONDETAILSDIALOG_H

#include "ui_buttondefinitiondetailswidget.h"

#include <QDialog>

#include "Input/ButtonDefinition.h"

class ButtonDefinitionDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    ButtonDefinitionDetailsDialog(QWidget* parent,
                                  ScapeEngine::ButtonDefinition::DeviceButtonSet deviceButtonsSet);
    ~ButtonDefinitionDetailsDialog();

    ScapeEngine::ButtonDefinition::DeviceButtonSet getDeviceButtonsSet();

protected:
    Ui::ButtonDefinitionDetailsWidget* ui;
    ScapeEngine::ButtonDefinition::DeviceButtonSet mDeviceButtonsSet;

protected slots:
    void moveToLeftButtonClicked();
    void moveToRightButtonClicked();

    void clicked(QAbstractButton* button);

private:
    void connectActions();

    void updateLists();
};

#endif // BUTTONDEFINITIONDETAILSDIALOG_H

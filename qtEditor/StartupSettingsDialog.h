#ifndef STARTUPSETTINGSDIALOG_H
#define STARTUPSETTINGSDIALOG_H

#include <QDialog>
#include "ui_startupsettingsdialog.h"

#include "EngineCore/StartupSettingsDataAccessObject.h"

class StartupSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    StartupSettingsDialog(QWidget* parent,
                          ScapeEngine::StartupSettingsDataAccessObject& startupSettingsDataAccessObject);
    ~StartupSettingsDialog();

protected:
    Ui::StartupSettingsDialog* ui;
    ScapeEngine::StartupSettingsDataAccessObject& mStartupSettingsDataAccessObject;

protected slots:
    void clicked(QAbstractButton* button);

private:
    void setupWidget();
    void connectActions();
    void initValues();
};

#endif // STARTUPSETTINGSDIALOG_H

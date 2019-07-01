#ifndef BUTTONDEFINITIONSDIALOG_H
#define BUTTONDEFINITIONSDIALOG_H

#include "ui_buttondefinitionswidget.h"

#include <map>
#include <vector>

#include <QDialog>
#include <QToolBar>
#include <QAction>

#include "Input/ButtonId.h"
#include "Input/ButtonDefinition.h"
#include "Input/ButtonDefinitionDataAccessObject.h"

class ButtonDefinitionsDialog : public QDialog
{
    Q_OBJECT

public:
    ButtonDefinitionsDialog(
        QWidget* parent, ScapeEngine::ButtonDefinitionDataAccessObject& buttonDefinitionDataAccessObject);
    ~ButtonDefinitionsDialog();

protected:
    Ui::ButtonDefinitionsWidget* ui;
    ScapeEngine::ButtonDefinitionDataAccessObject& mButtonDefinitionDataAccessObject;

protected slots:
    void priorityChanged(int value);

    void createDefinitionDetail();
    void editDefinitionDetail();
    void deleteDefinitionDetail();

    void definitionChanged(int index);

    void clicked(QAbstractButton* button);

private:
    typedef std::map<ScapeEngine::ButtonId::EButtonId, ScapeEngine::ButtonDefinition> ButtonDefinitionsMap;

    std::vector<ScapeEngine::ButtonId::EButtonId> mComboBoxButtons;
    ButtonDefinitionsMap mButtonDefinitions;

    QToolBar* mToolBar;

    QAction* mCreateAction;
    QAction* mEditAction;
    QAction* mDeleteAction;

    void setupWidget();
    void connectActions();
    void initValues();

    void updateButtonDefinitionDetailsList();

    ScapeEngine::ButtonId::EButtonId getCurrentButtonId();
    ScapeEngine::ButtonDefinition& getCurrentButtonDefinition();
};

#endif // BUTTONDEFINITIONSDIALOG_H

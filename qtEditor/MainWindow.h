#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBox>
#include <QtCore/QTimer>
#include <QActionGroup>

class OgreWidget;
class PropertiesWidget;
class PresetsWidget;
class BrushSettingsWidget;

namespace Ui
{
class MainWindow;
}
namespace ScapeEngine
{
class EngineInterface;
struct string;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

public Q_SLOTS:
    void importImage();
    void exportImage();
    void pencilEraserGPU();
    void directionalNoiseGPU();
    void noiseGPU();
    void softenGPU();
    void sharpenSoften();
    void noiseCPU();
    void levelUnlevel();
    void waterErosion();
    void pencilEraser();
    void materialProps();
    void skyProps();
    void renderwindowProps();
    void createPreset();
    void savePreset();
    void deletePreset();
    void exportPreset();
    void importPreset();
    void timerLoop();
    void aboutApp();
    void exitApp();

    void propertyValueChanged(const std::string& key, const std::string& value);

    void presetLoading(const std::string& preset);

protected:
    virtual void resizeEvent(QResizeEvent* rEvent);

private:
    void createActions();
    void connectActions();
    void createDockWidgets();
    void populateToolbar();
    void populateMainMenu();
    void selectTool(QString toolName, int category);
    void populatePropertyGrid();
    void populatePresetPanel();

    void openImportImageDialog();
    void openExportImageDialog();

    QMenu* menuFile;
    QMenu* menuHelp;
    QDockWidget* mPropertiesDockWidget;
    PropertiesWidget* mPropertiesWidget;
    QDockWidget* mPresetsDockWidget;
    PresetsWidget* mPresetsWidget;
    QDockWidget* mBrushSettingsDockWidget;
    BrushSettingsWidget* mBrushSettingsWidget;

    Ui::MainWindow* ui;
    ScapeEngine::EngineInterface* mEngineInterface;
    QString mSelectedToolElementName;
    int mSelectedToolElementGroupId;

    OgreWidget* mOgreWidget;
    QTimer* mTimer;

    bool mAttachedInputToEngine;

    QActionGroup* mToolActionGroup;

    QAction* actExit;
    QAction* actAbout;
    QAction* actImportImage;
    QAction* actExportImage;

    QAction* actPencilEraserGPU;
    QAction* actDirectionalNoiseGPU;
    QAction* actNoiseGPU;
    QAction* actSoftenGPU;

    QAction* actSharpenSoften;
    QAction* actNoiseCPU;
    QAction* actLevelUnlevel;
    QAction* actWaterErosion;
    QAction* actPencilEraser;

    QAction* actMaterialProps;
    QAction* actSkyProps;
    QAction* actRenderwindowProps;

    QAction* actCreatePreset;
    QAction* actSavePreset;
    QAction* actDeletePreset;
    QAction* actExportPreset;
    QAction* actImportPreset;
};

#endif // MAINWINDOW_H

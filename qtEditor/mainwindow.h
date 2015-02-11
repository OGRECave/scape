#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBox>

class OgreWidget;

namespace Ui {
class MainWindow;
}
namespace ScapeEngine
{
    class EngineInterface;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
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
    void exitApp();

protected:
    virtual void resizeEvent(QResizeEvent *rEvent);

private:
    void colorize();
    void createActions();
    void connectActions();
    void populateToolbar();
    void populateMainMenu();

    QMenu* menuFile;
    QToolBox* mPropertiesToolBox;
    QToolBox* mPresetsToolBox;
    QDockWidget* mPropertiesDockWidget;

    Ui::MainWindow *ui;
    class ScapeEngine::EngineInterface* mEngineInterface;

    OgreWidget* mOgreWidget;

    bool mAttachedInputToEngine;

    QAction*  actExit;
    QAction*  actImportImage;
    QAction*  actExportImage;

    QAction*  actPencilEraserGPU;
    QAction*  actDirectionalNoiseGPU;
    QAction*  actNoiseGPU;
    QAction*  actSoftenGPU;

    QAction*  actSharpenSoften;
    QAction*  actNoiseCPU;
    QAction*  actLevelUnlevel;
    QAction*  actWaterErosion;
    QAction*  actPencilEraser;

    QAction*  actMaterialProps;
    QAction*  actSkyProps;
    QAction*  actRenderwindowProps;

    QAction*  actCreatePreset;
    QAction*  actSavePreset;
    QAction*  actDeletePreset;
    QAction*  actExportPreset;
    QAction*  actImportPreset;
};

#endif // MAINWINDOW_H

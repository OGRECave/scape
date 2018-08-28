#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ogrewidget.h"
#include "EngineCore/EngineInterface.h"
#include <sstream>
#include "propertieswidget.h"
#include <iostream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mAttachedInputToEngine(false), mOgreWidget(0),
      mEngineInterface(0)
{
    ui->setupUi(this);

    createActions();
    populateMainMenu();
    populateToolbar();
    connectActions();

    mEngineInterface = new ScapeEngine::EngineInterface();
    mEngineInterface->initialize();

    setWindowTitle("qtScape 15.02");

    mOgreWidget = new OgreWidget(mEngineInterface, this);
    setCentralWidget(mOgreWidget);

    mPropertiesWidget = new PropertiesWidget(this);
    connect(mPropertiesWidget, SIGNAL(propertyValueChanged(const std::string&, const std::string&)), this,
            SLOT(propertyValueChanged(const std::string&, const std::string&)));

    mPropertiesToolBox = new QToolBox(this);
    mPropertiesToolBox->addItem(mPropertiesWidget, tr("General"));

    mPropertiesDockWidget = new QDockWidget(this);
    mPropertiesDockWidget->setWindowTitle(tr("Properties"));
    mPropertiesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    mPropertiesDockWidget->setObjectName(QString::fromUtf8("propertiesDockWidget"));
    mPropertiesDockWidget->setWidget(mPropertiesToolBox);
    mPropertiesDockWidget->setMinimumWidth(180);
    this->addDockWidget(Qt::RightDockWidgetArea, mPropertiesDockWidget);

    mTimer = new QTimer(this);
    mTimer->setInterval(0);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timerLoop()));
    mTimer->start();
}

MainWindow::~MainWindow()
{
    mEngineInterface->deinitialize();
    delete mEngineInterface;
    delete ui;
    delete mTimer;
    delete mToolActionGroup;
}

void MainWindow::resizeEvent(QResizeEvent* rEvent)
{
    if (rEvent)
    {
        QWidget::resizeEvent(rEvent);
        if (mOgreWidget && !mAttachedInputToEngine)
        {
            std::stringstream s;
            s << this->winId();
            mEngineInterface->attachInputToWindow(s.str().c_str());
            mAttachedInputToEngine = true;
        }
    }
}

void MainWindow::timerLoop() { mEngineInterface->update(); }

void MainWindow::createActions()
{
    actExit = new QAction(tr("Exit"), this);
    actExit->setStatusTip(tr("Exit application"));
    actExit->setShortcut(QKeySequence::Close);

    actAbout = new QAction(tr("About"), this);
    actAbout->setStatusTip(tr("About qtScape"));
    // actAbout->setIcon(QIcon(":/icons/about"));

    actImportImage = new QAction(tr("Import Image"), this);
    actImportImage->setStatusTip(tr("Import heightmap image"));
    actImportImage->setIcon(QIcon(":/icons/import"));

    actExportImage = new QAction(tr("Export Image"), this);
    actExportImage->setStatusTip(tr("Export heightmap image"));
    actExportImage->setIcon(QIcon(":/icons/export"));

    actPencilEraserGPU = new QAction(tr("Pencil/Eraser GPU"), this);
    actPencilEraserGPU->setStatusTip(tr("Pencil/Eraser GPU"));
    actPencilEraserGPU->setIcon(QIcon(":/icons/pencil"));
    actPencilEraserGPU->setCheckable(true);

    actDirectionalNoiseGPU = new QAction(tr("Directional Noise GPU"), this);
    actDirectionalNoiseGPU->setStatusTip(tr("Directional Noise GPU"));
    actDirectionalNoiseGPU->setIcon(QIcon(":/icons/pencil"));
    actDirectionalNoiseGPU->setCheckable(true);

    actNoiseGPU = new QAction(tr("Noise GPU"), this);
    actNoiseGPU->setStatusTip(tr("Noise GPU"));
    actNoiseGPU->setIcon(QIcon(":/icons/pencil"));
    actNoiseGPU->setCheckable(true);

    actSoftenGPU = new QAction(tr("Soften GPU"), this);
    actSoftenGPU->setStatusTip(tr("Soften GPU"));
    actSoftenGPU->setIcon(QIcon(":/icons/pencil"));
    actSoftenGPU->setCheckable(true);

    actSharpenSoften = new QAction(tr("Sharpen/Soften"), this);
    actSharpenSoften->setStatusTip(tr("Sharpen/Soften"));
    actSharpenSoften->setIcon(QIcon(":/icons/smooth"));
    actSharpenSoften->setCheckable(true);

    actNoiseCPU = new QAction(tr("Noise"), this);
    actNoiseCPU->setStatusTip(tr("Noise"));
    actNoiseCPU->setIcon(QIcon(":/icons/noise"));
    actNoiseCPU->setCheckable(true);

    actLevelUnlevel = new QAction(tr("Level/Unlevel"), this);
    actLevelUnlevel->setStatusTip(tr("Level/Unlevel"));
    actLevelUnlevel->setIcon(QIcon(":/icons/level"));
    actLevelUnlevel->setCheckable(true);

    actWaterErosion = new QAction(tr("Water Erosion"), this);
    actWaterErosion->setStatusTip(tr("Water Erosion"));
    actWaterErosion->setIcon(QIcon(":/icons/erosion"));
    actWaterErosion->setCheckable(true);

    actPencilEraser = new QAction(tr("Eraser"), this);
    actPencilEraser->setStatusTip(tr("Eraser"));
    actPencilEraser->setIcon(QIcon(":/icons/ledge"));
    actPencilEraser->setCheckable(true);

    actMaterialProps = new QAction(tr("Material"), this);
    actMaterialProps->setStatusTip(tr("Material Properties"));
    actMaterialProps->setIcon(QIcon(":/icons/material"));

    actSkyProps = new QAction(tr("Sky"), this);
    actSkyProps->setStatusTip(tr("Sky Properties"));
    actSkyProps->setIcon(QIcon(":/icons/light"));

    actRenderwindowProps = new QAction(tr("Renderwindow"), this);
    actRenderwindowProps->setStatusTip(tr("Renderwindow Properties"));
    actRenderwindowProps->setIcon(QIcon(":/icons/material"));

    actCreatePreset = new QAction(tr("Create Preset"), this);
    actCreatePreset->setStatusTip(tr("Create Preset"));
    actCreatePreset->setIcon(QIcon(":/icons/plus"));

    actSavePreset = new QAction(tr("Save Preset"), this);
    actSavePreset->setStatusTip(tr("Save Preset"));
    actSavePreset->setIcon(QIcon(":/icons/save"));

    actDeletePreset = new QAction(tr("Delete Preset"), this);
    actDeletePreset->setStatusTip(tr("Delete Preset"));
    actDeletePreset->setIcon(QIcon(":/icons/trash"));

    actExportPreset = new QAction(tr("Export Preset"), this);
    actExportPreset->setStatusTip(tr("Export Preset"));
    actExportPreset->setIcon(QIcon(":/icons/export"));

    actImportPreset = new QAction(tr("Import Preset"), this);
    actImportPreset->setStatusTip(tr("Import Preset"));
    actImportPreset->setIcon(QIcon(":/icons/import"));

    mToolActionGroup = new QActionGroup(this);
    mToolActionGroup->addAction(actPencilEraserGPU);
    mToolActionGroup->addAction(actDirectionalNoiseGPU);
    mToolActionGroup->addAction(actNoiseGPU);
    mToolActionGroup->addAction(actSoftenGPU);
    mToolActionGroup->addAction(actSharpenSoften);
    mToolActionGroup->addAction(actNoiseCPU);
    mToolActionGroup->addAction(actLevelUnlevel);
    mToolActionGroup->addAction(actWaterErosion);
    mToolActionGroup->addAction(actPencilEraser);
}

void MainWindow::connectActions()
{
    connect(actExit, SIGNAL(triggered()), this, SLOT(exitApp()));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(aboutApp()));
    connect(actImportImage, SIGNAL(triggered()), this, SLOT(importImage()));
    connect(actExportImage, SIGNAL(triggered()), this, SLOT(exportImage()));

    connect(actPencilEraserGPU, SIGNAL(triggered()), this, SLOT(pencilEraserGPU()));
    connect(actDirectionalNoiseGPU, SIGNAL(triggered()), this, SLOT(directionalNoiseGPU()));
    connect(actNoiseGPU, SIGNAL(triggered()), this, SLOT(noiseGPU()));
    connect(actSoftenGPU, SIGNAL(triggered()), this, SLOT(softenGPU()));

    connect(actSharpenSoften, SIGNAL(triggered()), this, SLOT(sharpenSoften()));
    connect(actNoiseCPU, SIGNAL(triggered()), this, SLOT(noiseCPU()));
    connect(actLevelUnlevel, SIGNAL(triggered()), this, SLOT(levelUnlevel()));
    connect(actWaterErosion, SIGNAL(triggered()), this, SLOT(waterErosion()));
    connect(actPencilEraser, SIGNAL(triggered()), this, SLOT(pencilEraser()));

    connect(actMaterialProps, SIGNAL(triggered()), this, SLOT(materialProps()));
    connect(actSkyProps, SIGNAL(triggered()), this, SLOT(skyProps()));
    connect(actRenderwindowProps, SIGNAL(triggered()), this, SLOT(renderwindowProps()));

    connect(actCreatePreset, SIGNAL(triggered()), this, SLOT(createPreset()));
    connect(actSavePreset, SIGNAL(triggered()), this, SLOT(savePreset()));
    connect(actDeletePreset, SIGNAL(triggered()), this, SLOT(deletePreset()));
    connect(actExportPreset, SIGNAL(triggered()), this, SLOT(exportPreset()));
    connect(actImportPreset, SIGNAL(triggered()), this, SLOT(importPreset()));
}

void MainWindow::populateToolbar()
{
    ui->mToolBar->addAction(actPencilEraserGPU);
    ui->mToolBar->addAction(actDirectionalNoiseGPU);
    ui->mToolBar->addAction(actNoiseGPU);
    ui->mToolBar->addAction(actSoftenGPU);
    ui->mToolBar->addSeparator();

    ui->mToolBar->addAction(actSharpenSoften);
    ui->mToolBar->addAction(actNoiseCPU);
    ui->mToolBar->addAction(actLevelUnlevel);
    ui->mToolBar->addAction(actWaterErosion);
    ui->mToolBar->addAction(actPencilEraser);
    ui->mToolBar->addSeparator();

    ui->mToolBar->addAction(actMaterialProps);
    ui->mToolBar->addAction(actSkyProps);
    ui->mToolBar->addAction(actRenderwindowProps);

    // actCreatePreset;
    // actSavePreset;
    // actDeletePreset;
    // actExportPreset;
    // actImportPreset;
}

void MainWindow::populateMainMenu()
{
    menuFile = new QMenu(tr("File"), ui->mMenuBar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    ui->mMenuBar->addAction(menuFile->menuAction());
    menuFile->addAction(actImportImage);
    menuFile->addAction(actExportImage);
    menuFile->addSeparator();
    menuFile->addAction(actExit);
    menuHelp = new QMenu(tr("Help"), ui->mMenuBar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    ui->mMenuBar->addAction(menuHelp->menuAction());
    menuHelp->addAction(actAbout);
}

void MainWindow::selectTool(QString toolName, int category)
{
    mSelectedToolElementName = toolName;
    mSelectedToolElementGroupId = (ScapeEngine::EScapeUIElementGroupId)category;

    if (mSelectedToolElementGroupId == ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION)
    {
        mEngineInterface->selectOperation(toolName.toStdString().c_str());
    }

    populatePropertyGrid();
    // populatePresetPanel();
}

void MainWindow::populatePropertyGrid()
{
    ScapeEngine::StringList nameList = mEngineInterface->getUIElementPropertyNameList(
        (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
        mSelectedToolElementName.toStdString().c_str());

    UIElementPropertyGridItemList itemList;

    ScapeEngine::StringList::iterator nameIt, nameItEnd = nameList.end();

    QString propertySetName = mSelectedToolElementName;

    for (nameIt = nameList.begin(); nameIt != nameItEnd; ++nameIt)
    {
        UIElementPropertyGridItem item;
        item.name = *nameIt;
        item.label = mEngineInterface->getUIElementPropertyField(propertySetName.toStdString().c_str(),
                                                                 *nameIt, "SHORT");
        item.description = mEngineInterface->getUIElementPropertyField(
            propertySetName.toStdString().c_str(), *nameIt, "LONG");
        item.category = mEngineInterface->getUIElementPropertyField(propertySetName.toStdString().c_str(),
                                                                    *nameIt, "CATEGORY");
        item.type = mEngineInterface->getUIElementPropertyField(propertySetName.toStdString().c_str(),
                                                                *nameIt, "TYPE");
        itemList.push_back(item);
    }

    mPropertiesWidget->populate(itemList,
                                mEngineInterface->getUIElementPropertyValueMap(
                                    (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
                                    mSelectedToolElementName.toStdString()));
}

void MainWindow::propertyValueChanged(const std::string& key, const std::string& value)
{
    std::string ret = mEngineInterface->setUIElementPropertyValue(
        (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
        mSelectedToolElementName.toStdString(), key, value);
    mPropertiesWidget->setValue(key, ret);
}

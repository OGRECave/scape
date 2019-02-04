#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "OgreWidget.h"
#include "EngineCore/EngineInterface.h"
#include <sstream>
#include "PropertiesWidget.h"
#include "PresetsWidget.h"
#include "BrushSettingsWidget.h"
#include <iostream>
#include "ImportImageDialog.h"
#include "ExportImageDialog.h"
#include "AboutDialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <OgreStringConverter.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mAttachedInputToEngine(false), mOgreWidget(0),
      mEngineInterface(0)
{
    mEngineInterface = new ScapeEngine::EngineInterface();
    mEngineInterface->initialize();

    ui->setupUi(this);

    createActions();
    createDockWidgets();
    populateMainMenu();
    populateToolbar();
    connectActions();

    setWindowTitle("qtScape 15.02");

    mOgreWidget = new OgreWidget(mEngineInterface, this);
    setCentralWidget(mOgreWidget);

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
    actMaterialProps->setCheckable(true);

    actSkyProps = new QAction(tr("Sky"), this);
    actSkyProps->setStatusTip(tr("Sky Properties"));
    actSkyProps->setIcon(QIcon(":/icons/light"));
    actSkyProps->setCheckable(true);

    actRenderwindowProps = new QAction(tr("Renderwindow"), this);
    actRenderwindowProps->setStatusTip(tr("Renderwindow Properties"));
    actRenderwindowProps->setIcon(QIcon(":/icons/material"));
    actRenderwindowProps->setCheckable(true);

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
    mToolActionGroup->addAction(actMaterialProps);
    mToolActionGroup->addAction(actSkyProps);
    mToolActionGroup->addAction(actRenderwindowProps);
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

void MainWindow::createDockWidgets()
{
    mPropertiesWidget = new PropertiesWidget(this);
    connect(mPropertiesWidget, SIGNAL(propertyValueChanged(const std::string&, const std::string&)), this,
            SLOT(propertyValueChanged(const std::string&, const std::string&)));

    mPropertiesDockWidget = new QDockWidget(this);
    mPropertiesDockWidget->setWindowTitle(tr("Properties"));
    mPropertiesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    mPropertiesDockWidget->setObjectName(QString::fromUtf8("propertiesDockWidget"));
    mPropertiesDockWidget->setWidget(mPropertiesWidget);
    mPropertiesDockWidget->setMinimumWidth(180);
    addDockWidget(Qt::RightDockWidgetArea, mPropertiesDockWidget);

    mPresetsWidget = new PresetsWidget();
    connect(mPresetsWidget, SIGNAL(presetLoading(const std::string&)), this,
            SLOT(presetLoading(const std::string&)));

    mPresetsDockWidget = new QDockWidget(this);
    mPresetsDockWidget->setWindowTitle(tr("Presets"));
    mPresetsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    mPresetsDockWidget->setObjectName(QString::fromUtf8("presetsDockWidget"));
    mPresetsDockWidget->setWidget(mPresetsWidget);
    mPresetsDockWidget->setMinimumWidth(180);
    addDockWidget(Qt::RightDockWidgetArea, mPresetsDockWidget);

    mBrushSettingsWidget = new BrushSettingsWidget(NULL, mEngineInterface->getHeightfieldBrushSettings());

    mBrushSettingsDockWidget = new QDockWidget(this);
    mBrushSettingsDockWidget->setWindowTitle(tr("Brush Settings"));
    mBrushSettingsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    mBrushSettingsDockWidget->setObjectName(QString::fromUtf8("brushSettingsDockWidget"));
    mBrushSettingsDockWidget->setWidget(mBrushSettingsWidget);
    mBrushSettingsDockWidget->setMinimumWidth(180);
    addDockWidget(Qt::RightDockWidgetArea, mBrushSettingsDockWidget);
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

    mPresetsWidget->getToolBar()->addAction(actCreatePreset);
    mPresetsWidget->getToolBar()->addAction(actSavePreset);
    mPresetsWidget->getToolBar()->addAction(actDeletePreset);
    mPresetsWidget->getToolBar()->addAction(actExportPreset);
    mPresetsWidget->getToolBar()->addAction(actImportPreset);
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
    populatePresetPanel();
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

void MainWindow::populatePresetPanel()
{
    std::vector<std::string> presets;

    ScapeEngine::StringList presetList = mEngineInterface->getUIElementPresetPropertyNames(
        (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
        mSelectedToolElementName.toStdString());
    for (ScapeEngine::StringList::const_iterator nameIt = presetList.begin(); nameIt != presetList.end();
         ++nameIt)
    {
        presets.push_back(*nameIt);
    }

    mPresetsWidget->populate(presets);
}

void MainWindow::openImportImageDialog()
{
    ScapeEngine::StringStringStringPairMap fileFilterMap =
        mEngineInterface->getFileFilterMap(ScapeEngine::SCAPEUIELEMENTGROUPID_FILEIMPORT);

    ScapeEngine::StringList names = mEngineInterface->getUIElementNameList(
        (ScapeEngine::EScapeUIElementGroupId)ScapeEngine::SCAPEUIELEMENTGROUPID_FILEIMPORT);

    ImportImageDialog::FormatItemVector itemVector;
    ImportImageDialog::FormatOptionItemMap itemMap;

    for (ScapeEngine::StringList::const_iterator nameIt = names.begin(); nameIt != names.end(); ++nameIt)
    {
        ImportImageDialog::FormatItem item;
        item.name = *nameIt;
        item.label = mEngineInterface->getUIElementPropertyField(*nameIt, "LABEL", "SHORT");
        item.description = mEngineInterface->getUIElementPropertyField(*nameIt, "LABEL", "LONG");

        std::pair<string, string> fileFilter = fileFilterMap[*nameIt];
        item.formatName = fileFilter.first;
        item.formatExtensions = fileFilter.second;

        itemVector.push_back(item);

        ScapeEngine::StringList subNameList = mEngineInterface->getUIElementPropertyNameList(
            (ScapeEngine::EScapeUIElementGroupId)ScapeEngine::SCAPEUIELEMENTGROUPID_FILEIMPORT, *nameIt);

        std::vector<ImportImageDialog::FormatOptionItem> subVector;
        for (ScapeEngine::StringList::const_iterator subNameIt = subNameList.begin();
             subNameIt != subNameList.end(); ++subNameIt)
        {
            ImportImageDialog::FormatOptionItem item;
            item.name = *subNameIt;

            item.label = mEngineInterface->getUIElementPropertyField(*nameIt, *subNameIt, "SHORT");
            item.description = mEngineInterface->getUIElementPropertyField(*nameIt, *subNameIt, "LONG");
            item.type = mEngineInterface->getUIElementPropertyField(*nameIt, *subNameIt, "TYPE");
            item.options = mEngineInterface->getUIElementPropertyField(*nameIt, *subNameIt, "OPTIONS");
            subVector.push_back(item);
        }

        itemMap[*nameIt] = subVector;
    }

    ImportImageDialog* importImageDialog = new ImportImageDialog(this);
    importImageDialog->populate(itemVector, itemMap);
    importImageDialog->setModal(true);
    importImageDialog->setWindowModality(Qt::WindowModal);
    int result = importImageDialog->exec();
    if (result == QDialog::Accepted)
    {
        ScapeEngine::StringStringMap valueMap;
        valueMap["FLIPX"] = importImageDialog->getFlipX() ? "1" : "0";
        valueMap["FLIPY"] = importImageDialog->getFlipY() ? "1" : "0";

        int width = importImageDialog->getWidth();
        if (width >= 0)
        {
            valueMap["WIDTH"] = Ogre::StringConverter::toString(width);
        }

        int height = importImageDialog->getHeight();
        if (height >= 0)
        {
            valueMap["HEIGHT"] = Ogre::StringConverter::toString(height);
        }

        int endianness = importImageDialog->getEndiannessIndex();
        if (endianness >= 0)
        {
            valueMap["BIGENDIAN"] = Ogre::StringConverter::toString(endianness);
        }

        int bpp = importImageDialog->getBppIndex();
        if (bpp >= 0)
        {
            valueMap["BPP"] = Ogre::StringConverter::toString(bpp);
        }

        mEngineInterface->setUIElementPropertyValueMap(
            (ScapeEngine::EScapeUIElementGroupId)ScapeEngine::SCAPEUIELEMENTGROUPID_FILEIMPORT,
            importImageDialog->getSelectedFormatName(), valueMap);

        std::string error;
        if (!mEngineInterface->importImageFile(importImageDialog->getSelectedFormatName(),
                                               importImageDialog->getFilePath(), &error))
        {
            QMessageBox::critical(this, "Import failed!", QString(error.c_str()));
        }
    }
    delete importImageDialog;
}

void MainWindow::openExportImageDialog()
{
    ScapeEngine::StringStringStringPairMap fileFilterMap =
        mEngineInterface->getFileFilterMap(ScapeEngine::SCAPEUIELEMENTGROUPID_FILEEXPORT);

    ScapeEngine::StringList names = mEngineInterface->getUIElementNameList(
        (ScapeEngine::EScapeUIElementGroupId)ScapeEngine::SCAPEUIELEMENTGROUPID_FILEEXPORT);

    ExportImageDialog::FormatItemVector itemVector;
    ScapeEngine::StringList::const_iterator nameIt, nameItEnd = names.end();
    for (nameIt = names.begin(); nameIt != nameItEnd; ++nameIt)
    {
        ExportImageDialog::FormatItem item;
        item.name = *nameIt;
        item.label = mEngineInterface->getUIElementPropertyField(*nameIt, "LABEL", "SHORT");
        item.description = mEngineInterface->getUIElementPropertyField(*nameIt, "LABEL", "LONG");

        std::pair<string, string> fileFilter = fileFilterMap[*nameIt];
        item.formatName = fileFilter.first;
        item.formatExtensions = fileFilter.second;

        itemVector.push_back(item);
    }

    ExportImageDialog* exportImageDialog = new ExportImageDialog(this);
    exportImageDialog->populate(itemVector);
    exportImageDialog->setModal(true);
    exportImageDialog->setWindowModality(Qt::WindowModal);
    int result = exportImageDialog->exec();
    if (result == QDialog::Accepted)
    {
        mEngineInterface->setUIElementPropertyValue(ScapeEngine::SCAPEUIELEMENTGROUPID_FILEEXPORT,
                                                    exportImageDialog->getSelectedFormatName(), "FLIPX",
                                                    exportImageDialog->getFlipX() ? "1" : "0");
        mEngineInterface->setUIElementPropertyValue(ScapeEngine::SCAPEUIELEMENTGROUPID_FILEEXPORT,
                                                    exportImageDialog->getSelectedFormatName(), "FLIPY",
                                                    exportImageDialog->getFlipY() ? "1" : "0");
        std::string error;
        if (!mEngineInterface->exportImageFile(exportImageDialog->getSelectedFormatName(),
                                               exportImageDialog->getFilePath(), &error))
        {
            QMessageBox::critical(this, "Export failed!", QString(error.c_str()));
        }
    }
    delete exportImageDialog;
}

void MainWindow::importImage()
{
    openImportImageDialog();
}

void MainWindow::exportImage()
{
    openExportImageDialog();
}

void MainWindow::pencilEraserGPU()
{
    selectTool("HeightfieldOperationGPUPencil", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::directionalNoiseGPU()
{
    selectTool("HeightfieldOperationGPUNoiseDrag", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::noiseGPU()
{
    selectTool("HeightfieldOperationGPUNoise", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::softenGPU()
{
    selectTool("HeightfieldOperationGPUBrush", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::sharpenSoften()
{
    selectTool("HeightfieldOperationCPUSmooth", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::noiseCPU()
{
    selectTool("HeightfieldOperationCPUNoise", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::levelUnlevel()
{
    selectTool("HeightfieldOperationCPULevel", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::waterErosion()
{
    selectTool("HeightfieldOperationCPUErosion", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::pencilEraser()
{
    selectTool("HeightfieldOperationCPUBrush", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::materialProps()
{
    selectTool("MaterialSettings", ScapeEngine::SCAPEUIELEMENTGROUPID_MATERIAL);
}

void MainWindow::skyProps()
{
    selectTool("SkySettings", ScapeEngine::SCAPEUIELEMENTGROUPID_SKY);
}

void MainWindow::renderwindowProps()
{
    selectTool("RenderWindowSettings", ScapeEngine::SCAPEUIELEMENTGROUPID_RENDERWINDOW);
}

void MainWindow::createPreset()
{
    bool ok;
    QString presetName =
        QInputDialog::getText(this, "Create Preset", "Preset Name:", QLineEdit::Normal, "Preset", &ok);
    if (ok && !presetName.isEmpty())
    {
        string uniquePresetName = mEngineInterface->makeUniquePresetName(
            (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
            mSelectedToolElementName.toStdString(), presetName.toStdString());
        ScapeEngine::StringStringMap valueMap = mEngineInterface->getUIElementPropertyValueMap(
            (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
            mSelectedToolElementName.toStdString());
        mEngineInterface->setUIElementPresetPropertyValueMap(
            (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
            mSelectedToolElementName.toStdString(), string(uniquePresetName), valueMap);
        populatePresetPanel();
    }
}

void MainWindow::savePreset()
{
    std::string presetName = mPresetsWidget->getSelectedPreset();
    if (presetName.length() > 0)
    {
        ScapeEngine::StringStringMap valueMap = mEngineInterface->getUIElementPropertyValueMap(
            (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
            mSelectedToolElementName.toStdString());

        mEngineInterface->setUIElementPresetPropertyValueMap(
            (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
            mSelectedToolElementName.toStdString(), presetName, valueMap);
    }
}

void MainWindow::deletePreset()
{
    std::string presetName = mPresetsWidget->getSelectedPreset();
    if (presetName.length() > 0)
    {
        mEngineInterface->deleteUIElementPreset(
            (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
            mSelectedToolElementName.toStdString(), presetName);
        populatePresetPanel();
    }
}

void MainWindow::exportPreset()
{
    std::string format = "Preset files (*.spe);;All files (*.*)";

    QString fileName =
        QFileDialog::getSaveFileName(this, QString(), QDir::currentPath(), QString(format.c_str()),
                                     Q_NULLPTR, QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty())
    {
        mEngineInterface->exportUIElementPreset(
            (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
            mSelectedToolElementName.toStdString(), fileName.toStdString());
    }
}

void MainWindow::importPreset()
{
    std::string format = "Preset files (*.spe);;All files (*.*)";

    QString fileName =
        QFileDialog::getOpenFileName(this, QString(), QDir::currentPath(), QString(format.c_str()),
                                     Q_NULLPTR, QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty())
    {
        std::string presetName = mEngineInterface->importUIElementPreset(
            (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
            mSelectedToolElementName.toStdString(), fileName.toStdString());
        populatePresetPanel();
    }
}

void MainWindow::aboutApp()
{
    AboutDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
        return;
}

void MainWindow::exitApp() { close(); }

void MainWindow::propertyValueChanged(const std::string& key, const std::string& value)
{
    std::string ret = mEngineInterface->setUIElementPropertyValue(
        (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
        mSelectedToolElementName.toStdString(), key, value);
    mPropertiesWidget->setValue(key, ret);
}

void MainWindow::presetLoading(const std::string& preset)
{
    ScapeEngine::StringStringMap valueMap = mEngineInterface->getUIElementPresetPropertyValueMap(
        (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
        mSelectedToolElementName.toStdString(), preset);
    ScapeEngine::StringStringMap correctValueMap = mEngineInterface->setUIElementPropertyValueMap(
        (ScapeEngine::EScapeUIElementGroupId)mSelectedToolElementGroupId,
        mSelectedToolElementName.toStdString(), valueMap);
    populatePropertyGrid();
}

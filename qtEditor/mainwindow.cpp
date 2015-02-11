#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ogrewidget.h"
#include "EngineCore/EngineInterface.h"
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mAttachedInputToEngine(false),
    mOgreWidget(0),
    mEngineInterface(0)
{
    ui->setupUi(this);

    colorize();
    createActions();
    populateMainMenu();
    populateToolbar();
    connectActions();

    mEngineInterface = new ScapeEngine::EngineInterface();
    mEngineInterface->initialize();

    mOgreWidget = new OgreWidget(mEngineInterface , this);
    setCentralWidget(mOgreWidget);

}

MainWindow::~MainWindow()
{
    mEngineInterface->deinitialize();
    delete mEngineInterface;
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *rEvent)
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

void MainWindow::createActions()
{
    actExit = new QAction(tr("Exit"), this);
    actExit->setStatusTip(tr("Exit application"));
    actExit->setShortcut(QKeySequence::Close);

    actImportImage = new QAction(tr("Import Image"), this);
    actImportImage->setStatusTip(tr("Import heightmap image"));
    actImportImage->setIcon(QIcon(":/icons/import"));

    actExportImage = new QAction(tr("Export Image"), this);
    actExportImage->setStatusTip(tr("Export heightmap image"));
    actExportImage->setIcon(QIcon(":/icons/export"));

    actPencilEraserGPU = new QAction(tr("Pencil/Eraser GPU"), this);
    actPencilEraserGPU->setStatusTip(tr("Pencil/Eraser GPU"));
    actPencilEraserGPU->setIcon(QIcon(":/icons/pencil"));

//    actDirectionalNoiseGPU;
//    actNoiseGPU;
//    actSoftenGPU;

//    actSharpenSoften;
//    actNoiseCPU;
//    actLevelUnlevel;
//    actWaterErosion;
//    actPencilEraser;

//    actMaterialProps;
//    actSkyProps;
//    actRenderwindowProps;

//    actCreatePreset;
//    actSavePreset;
//    actDeletePreset;
//    actExportPreset;
//    actImportPreset;
}

void MainWindow::connectActions()
{
    connect(actExit, SIGNAL(triggered()), this, SLOT(exitApp()));
}

void MainWindow::populateToolbar()
{
    ui->mToolBar->addAction(actPencilEraserGPU);
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
}

void MainWindow::colorize()
{
    QPalette palette;
    QBrush brush(QColor(255, 255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    QBrush brush1(QColor(91, 91, 91, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Button, brush1);
    QBrush brush2(QColor(137, 137, 137, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Light, brush2);
    QBrush brush3(QColor(114, 114, 114, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
    QBrush brush4(QColor(45, 45, 45, 255));
    brush4.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
    QBrush brush5(QColor(60, 60, 60, 255));
    brush5.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
    palette.setBrush(QPalette::Active, QPalette::Text, brush);
    palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
    palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
    QBrush brush6(QColor(0, 0, 0, 255));
    brush6.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush6);
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    palette.setBrush(QPalette::Active, QPalette::Shadow, brush6);
    palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
    QBrush brush7(QColor(255, 255, 220, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
    palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
    palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
    palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
    palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
    palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
    palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
    palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
    palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
    palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
    palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush6);
    palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
    palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
    this->setPalette(palette);
}

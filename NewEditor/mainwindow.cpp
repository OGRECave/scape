#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "EngineCore/EngineInterface.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mEngineInterface = new ScapeEngine::EngineInterface();
    mEngineInterface->initialize();
}

MainWindow::~MainWindow()
{
    mEngineInterface->deinitialize();
    delete mEngineInterface;
    delete ui;
}

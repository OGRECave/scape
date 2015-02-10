#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "EngineCore/EngineInterface.h"
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mAttachedInputToEngine(false)
{
    ui->setupUi(this);

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

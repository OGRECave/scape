#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "EngineCore/EngineInterface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ScapeEngine::EngineInterface::openConsole();

    MainWindow w;
    w.show();

    return a.exec();
}

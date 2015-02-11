#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>
#include <QtWidgets/QStyleFactory>
#include "EngineCore/EngineInterface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("Scape");
    a.setApplicationName("qtScape");
    a.setStyle(QStyleFactory::create("Fusion"));

    QSplashScreen *splash;
    QPixmap pixmap(":/images/scape_splash.jpg");
    splash = new QSplashScreen(pixmap);
    splash->show();

    ScapeEngine::EngineInterface::openConsole();

    MainWindow w;
    w.show();

    splash->finish(&w);
    delete splash;

    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <qfile.h>
#include "EngineCore/EngineInterface.h"

#include <Ogre.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));

    ScapeEngine::EngineInterface::openConsole();

//    QPalette darkPalette;
//    darkPalette.setColor(QPalette::Window, QColor(53,53,53));// rgb(53,53,53)
//    darkPalette.setColor(QPalette::WindowText, QColor(220, 220, 220)); // rgb(220, 220, 220)
//    darkPalette.setColor(QPalette::Base, QColor(25,25,25)); // rgb(25,25,25)
//    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53)); // rgb(63,63,63)
//    darkPalette.setColor(QPalette::ToolTipBase, QColor(220, 220, 220));
//    darkPalette.setColor(QPalette::ToolTipText, QColor(220, 220, 220));
//    darkPalette.setColor(QPalette::Text, Qt::white);
//    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
//    darkPalette.setColor(QPalette::ButtonText, Qt::white);
//    darkPalette.setColor(QPalette::BrightText, Qt::red);
//    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218)); // rgb(42, 130, 218)
//    darkPalette.setColor(QPalette::Highlight, QColor(35,35,35)); // rgb(35,35,35)
//    darkPalette.setColor(QPalette::HighlightedText, QColor(220, 220, 220));

//    qApp->setPalette(darkPalette);


//    QFile File("darkorange.qss");
//     File.open(QIODevice::ReadOnly | QIODevice::Text);
//     QString StyleSheet = QLatin1String(File.readAll());

//     qApp->setStyleSheet(StyleSheet);

    MainWindow w;
    w.show();

    return a.exec();
}

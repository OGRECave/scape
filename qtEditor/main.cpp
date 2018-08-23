#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>
#include <QtWidgets/QStyleFactory>
#include "EngineCore/EngineInterface.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
//#include "stdredirector.h"

void outcallback(const char* ptr, std::streamsize count, void* pTextBox)
{
    (void)count;
    QTextEdit* p = static_cast<QTextEdit*>(pTextBox);
    p->append(ptr);
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("Scape");
    a.setApplicationName("qtScape");
    a.setApplicationVersion("15.02");
    a.setStyle(QStyleFactory::create("Fusion"));

    QSplashScreen* splash;
    QPixmap pixmap(":/images/scape_splash.jpg");
    splash = new QSplashScreen(pixmap);

    QVBoxLayout* layout = new QVBoxLayout(splash);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* splashLabel = new QLabel(QString::fromUtf8("Loading ..."), splash);
    splashLabel->setStyleSheet("color: maroon; font-size: 18px;");
    splashLabel->setAlignment(Qt::AlignCenter);
    QTextEdit* textedit = new QTextEdit("", splash);
    textedit->setStyleSheet("background-color:transparent; font-size: 16px;");
    layout->addWidget(splashLabel);
    layout->addWidget(textedit);
    splash->setLayout(layout);
    // StdRedirector<>* myRedirector = new StdRedirector<>(std::cout, outcallback, textedit);

    splash->show();

    ScapeEngine::EngineInterface::openConsole();

    MainWindow w;
    w.show();

    splash->finish(&w);
    delete splash;
    // delete myRedirector;
    return a.exec();
}

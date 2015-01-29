#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
namespace ScapeEngine
{
    class EngineInterface;
    struct string;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    class ScapeEngine::EngineInterface* mEngineInterface;
};

#endif // MAINWINDOW_H

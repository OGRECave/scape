#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ogrewidget.h"

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

protected:
    virtual void resizeEvent(QResizeEvent *rEvent);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    class ScapeEngine::EngineInterface* mEngineInterface;
    OgreWidget* mOgreWidget;
    bool mAttachedInputToEngine;
};

#endif // MAINWINDOW_H

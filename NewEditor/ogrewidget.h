#ifndef OGREWIDGET_H
#define OGREWIDGET_H

#include <QWidget>
#include "EngineCore/EngineInterface.h"

class OgreWidget : public QWidget
{
    Q_OBJECT

public:
    OgreWidget(ScapeEngine::EngineInterface* engineInterface, QWidget *parent);
    ~OgreWidget();

    // Override QWidget::paintEngine to return NULL
    QPaintEngine* paintEngine() const; // Turn off QTs paint engine for the Ogre widget.

protected: // interface
    /** @name Inherited from QWidget
     */
    //@{
    virtual void paintEvent(QPaintEvent *pEvent);
    virtual void resizeEvent(QResizeEvent *rEvent);
    virtual void update();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    //@}
private:
    ScapeEngine::EngineInterface* mEngineInterface;
    bool mHasView;
};

#endif // OGREWIDGET_H

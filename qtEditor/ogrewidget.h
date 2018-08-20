#ifndef OGREWIDGET_H
#define OGREWIDGET_H

#include <QWidget>
#include "EngineCore/EngineInterface.h"
#include "Ogre.h"

typedef std::string string;
#include "Utils/TVector2.h"
#include "Utils/TVector3.h"
#include "Input/InputListener.h"

class OgreWidget : public QWidget, public ScapeEngine::InputListener
{
    Q_OBJECT

public:
    OgreWidget(ScapeEngine::EngineInterface* engineInterface, QWidget* parent);
    ~OgreWidget();

    // Override QWidget::paintEngine to return NULL
    QPaintEngine* paintEngine() const; // Turn off QTs paint engine for the Ogre widget.

protected: // interface
    /** @name Inherited from QWidget
     */
    //@{
    virtual void paintEvent(QPaintEvent* pEvent);
    virtual void resizeEvent(QResizeEvent* rEvent);
    virtual void update();
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void focusOutEvent(QFocusEvent* evt);
    virtual void focusInEvent(QFocusEvent* evt);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    //@}
private:
    ScapeEngine::EngineInterface* mEngineInterface;
    bool mHasView;
    bool mHasFocus;
};

#endif // OGREWIDGET_H

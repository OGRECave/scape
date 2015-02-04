#include "ogrewidget.h"
#include <sstream>

OgreWidget::OgreWidget(ScapeEngine::EngineInterface *engineInterface, QWidget *parent)
    : mEngineInterface(engineInterface),
      mHasView(false)
{
    if (!parent) {
        throw std::runtime_error("Parent widget supplied was uninitialised!"); // interface requirement
    }

    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);
    setAttribute(Qt::WA_NoBackground);
    setAttribute(Qt::WA_PaintOnScreen);

}

OgreWidget::~OgreWidget()
{

}

void OgreWidget::paintEvent(QPaintEvent *pEvent) {
    this->update();
}

QPaintEngine* OgreWidget::paintEngine() const
{
    // We don't want another paint engine to get in the way for our Ogre based paint engine.
    // So we return nothing.
    return NULL;
}


void OgreWidget::resizeEvent(QResizeEvent *rEvent)
{
    if (rEvent)
    {
        QWidget::resizeEvent(rEvent);
        this->update();
    }
}


void OgreWidget::mousePressEvent(QMouseEvent *event)
{
}


void OgreWidget::mouseMoveEvent(QMouseEvent *event)
{
}


void OgreWidget::update()
{
    QWidget::update();
    if (!mHasView)
    {
        std::stringstream s;
        s << this->winId();
        mEngineInterface->createRenderView(1, s.str().c_str(), this->x(), this->y(), this->width(), this->height());
        mHasView = true;
    }
    else
    {
        mEngineInterface->onRenderViewSetFocus(1);
        mEngineInterface->onRenderViewMovedOrResized(1, this->x(), this->y(), this->width(), this->height());
        mEngineInterface->update();
    }
}

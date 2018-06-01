#include "ogrewidget.h"
#include <sstream>
#include <QtGui/QFocusEvent>

#include "PCH/stdafx.h"
#include "Input/InputManager.h"

OgreWidget::OgreWidget(ScapeEngine::EngineInterface *engineInterface,
                       QWidget *parent)
    : ScapeEngine::InputListener(
          ScapeEngine::EngineCore::getSingleton().getInputManager()),
      mEngineInterface(engineInterface),
      mHasView(false),
      mHasFocus(false) {
  if (!parent) {
    throw std::runtime_error(
        "Parent widget supplied was uninitialised!");  // interface requirement
  }

  setFocusPolicy(Qt::WheelFocus);
  setMouseTracking(true);
  setAttribute(Qt::WA_NoBackground);
  setAttribute(Qt::WA_PaintOnScreen);

  mInputManager->attachToWindow(this);
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


void OgreWidget::mousePressEvent(QMouseEvent *e)
{
    using namespace ScapeEngine;
    if (mActive)
    {
      DeviceButtonId::EDeviceButtonId button =
          e->button() == 1 ? DeviceButtonId::MB_Left : DeviceButtonId::MB_Right;
      mDeviceButtonPressed[button] = true;
      mInputManager->onDeviceButtonPressed(button);
    }
}

void OgreWidget::mouseReleaseEvent(QMouseEvent *e)
{
    using namespace ScapeEngine;
    if (mActive)
    {
        DeviceButtonId::EDeviceButtonId button =
            e->button() == 1 ? DeviceButtonId::MB_Left : DeviceButtonId::MB_Right;
        mDeviceButtonPressed[button] = false;
        mInputManager->onDeviceButtonReleased(button);
    }
}

static ScapeEngine::DeviceButtonId::EDeviceButtonId toButton(int key) {
  using namespace ScapeEngine::DeviceButtonId;
  switch (key) {
    case Qt::Key_W:
      return KC_W;
    case Qt::Key_A:
      return KC_A;
    case Qt::Key_S:
      return KC_S;
    case Qt::Key_D:
      return KC_D;
    case Qt::Key_Q:
      return KC_Q;
    case Qt::Key_C:
      return KC_C;
    case Qt::Key_L:
      return KC_L;
    case Qt::Key_R:
      return KC_R;
    case Qt::Key_Control:
      return KC_LCONTROL;
    case Qt::Key_Alt:
      return KC_LMENU;
    default:
      return DEVICEBUTTONID_UNKNOWN;
  }
}

void OgreWidget::keyPressEvent(QKeyEvent *e)
{
    using namespace ScapeEngine;
    if (mActive)
    {
        DeviceButtonId::EDeviceButtonId button = toButton(e->key());
        mDeviceButtonPressed[button] = true;
        mInputManager->onDeviceButtonPressed(button);
    }
}
void OgreWidget::keyReleaseEvent(QKeyEvent *e)
{
    using namespace ScapeEngine;
    if (mActive)
    {
        DeviceButtonId::EDeviceButtonId button = toButton(e->key());
        mDeviceButtonPressed[button] = false;
        mInputManager->onDeviceButtonReleased(button);
    }
}

void OgreWidget::mouseMoveEvent(QMouseEvent *e)
{
    using namespace ScapeEngine;
    if (mActive)
    {
        mPointerPressure = -1;
        IVector3 newPosition(e->x(), e->y(), 0);
        mPointerDeltaPosition = newPosition - mPointerPosition;

        if (newPosition.x != mPointerPosition.x ||
            newPosition.y != mPointerPosition.y ||
            newPosition.z != mPointerPosition.z)
        {
            mPointerPosition = newPosition;
            mInputManager->onPointerMove();
        }
    }
}

void OgreWidget::focusOutEvent(QFocusEvent *evt)
{
    if (mHasView)
    {
        mEngineInterface->onRenderViewKillFocus(1);
    }
    mHasFocus = false;
    evt->setAccepted(true);
}

void OgreWidget::focusInEvent(QFocusEvent *evt)
{
    if (mHasView)
    {
        mEngineInterface->onRenderViewSetFocus(1);
    }
    mHasFocus = true;
    evt->setAccepted(true);
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

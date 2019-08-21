#include "OgreWidget.h"
#include <sstream>
#include <QtGui/QFocusEvent>

#include "ScapeEngineStableHeaders.h"
#include "Input/InputManager.h"

OgreWidget::OgreWidget(ScapeEngine::EngineInterface* engineInterface, QWidget* parent)
    : ScapeEngine::InputListener(ScapeEngine::EngineCore::getSingleton().getInputManager()),
      mEngineInterface(engineInterface), mHasView(false), mHasFocus(false)
{
    if (!parent)
    {
        throw std::runtime_error("Parent widget supplied was uninitialised!"); // interface requirement
    }

    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);
    setAttribute(Qt::WA_NoBackground);
    setAttribute(Qt::WA_PaintOnScreen);

    mInputManager->attachToWindow(this);
}

OgreWidget::~OgreWidget() {}

void OgreWidget::paintEvent(QPaintEvent* pEvent) { this->update(); }

QPaintEngine* OgreWidget::paintEngine() const
{
    // We don't want another paint engine to get in the way for our Ogre based paint engine.
    // So we return nothing.
    return NULL;
}

void OgreWidget::resizeEvent(QResizeEvent* rEvent)
{
    if (rEvent)
    {
        QWidget::resizeEvent(rEvent);
        this->update();
    }
}

void OgreWidget::mousePressEvent(QMouseEvent* e)
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

void OgreWidget::mouseReleaseEvent(QMouseEvent* e)
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

static ScapeEngine::DeviceButtonId::EDeviceButtonId toButton(int key, int modifiers)
{
    using namespace ScapeEngine::DeviceButtonId;

    bool numpad = (modifiers & Qt::KeypadModifier) == Qt::KeypadModifier;

    switch (key)
    {
    case Qt::Key_1:
        return numpad ? KC_NUMPAD1 : KC_1;
    case Qt::Key_2:
        return numpad ? KC_NUMPAD2 : KC_2;
    case Qt::Key_3:
        return numpad ? KC_NUMPAD3 : KC_3;
    case Qt::Key_4:
        return numpad ? KC_NUMPAD4 : KC_4;
    case Qt::Key_5:
        return numpad ? KC_NUMPAD5 : KC_5;
    case Qt::Key_6:
        return numpad ? KC_NUMPAD6 : KC_6;
    case Qt::Key_7:
        return numpad ? KC_NUMPAD7 : KC_7;
    case Qt::Key_8:
        return numpad ? KC_NUMPAD8 : KC_8;
    case Qt::Key_9:
        return numpad ? KC_NUMPAD9 : KC_9;
    case Qt::Key_0:
        return numpad ? KC_NUMPAD0 : KC_0;

    case Qt::Key_Q:
        return KC_Q;
    case Qt::Key_W:
        return KC_W;
    case Qt::Key_E:
        return KC_E;
    case Qt::Key_R:
        return KC_R;
    case Qt::Key_T:
        return KC_T;
    case Qt::Key_Y:
        return KC_Y;
    case Qt::Key_U:
        return KC_U;
    case Qt::Key_I:
        return KC_I;
    case Qt::Key_O:
        return KC_O;
    case Qt::Key_P:
        return KC_P;
    case Qt::Key_A:
        return KC_A;
    case Qt::Key_S:
        return KC_S;
    case Qt::Key_D:
        return KC_D;
    case Qt::Key_F:
        return KC_F;
    case Qt::Key_G:
        return KC_G;
    case Qt::Key_H:
        return KC_H;
    case Qt::Key_J:
        return KC_J;
    case Qt::Key_K:
        return KC_K;
    case Qt::Key_L:
        return KC_L;
    case Qt::Key_Z:
        return KC_Z;
    case Qt::Key_X:
        return KC_X;
    case Qt::Key_C:
        return KC_C;
    case Qt::Key_V:
        return KC_V;
    case Qt::Key_B:
        return KC_B;
    case Qt::Key_N:
        return KC_N;
    case Qt::Key_M:
        return KC_M;

    case Qt::Key_Escape:
        return KC_ESCAPE;
    case Qt::Key_Plus:
        return KC_ADD;
    case Qt::Key_Minus:
        return numpad ? KC_SUBTRACT : KC_MINUS;
    case Qt::Key_Asterisk:
        return KC_MULTIPLY;
    case Qt::Key_Equal:
        return KC_EQUALS;
    case Qt::Key_Backspace:
        return KC_BACK;
    case Qt::Key_Tab:
        return KC_TAB;
    case Qt::Key_BracketLeft:
        return KC_LBRACKET;
    case Qt::Key_BracketRight:
        return KC_RBRACKET;
    case Qt::Key_Return:
        return KC_RETURN;
    case Qt::Key_Control:
        return KC_LCONTROL;
    case Qt::Key_Semicolon:
        return KC_SEMICOLON;
    case Qt::Key_Apostrophe:
        return KC_APOSTROPHE;
    case Qt::Key_Shift:
        return KC_LSHIFT;
    case Qt::Key_Backslash:
        return KC_BACKSLASH;
    case Qt::Key_Comma:
        return numpad ? KC_DECIMAL : KC_COMMA;
    case Qt::Key_Period:
        return numpad ? KC_DECIMAL : KC_PERIOD;
    case Qt::Key_Slash:
        return numpad ? KC_DIVIDE : KC_SLASH;
    case Qt::Key_Alt:
        return KC_LMENU;
    case Qt::Key_Space:
        return KC_SPACE;
    case Qt::Key_CapsLock:
        return KC_CAPITAL;

    case Qt::Key_F1:
        return KC_F1;
    case Qt::Key_F2:
        return KC_F2;
    case Qt::Key_F3:
        return KC_F3;
    case Qt::Key_F4:
        return KC_F4;
    case Qt::Key_F5:
        return KC_F5;
    case Qt::Key_F6:
        return KC_F6;
    case Qt::Key_F7:
        return KC_F7;
    case Qt::Key_F8:
        return KC_F8;
    case Qt::Key_F9:
        return KC_F9;
    case Qt::Key_F10:
        return KC_F10;
    case Qt::Key_F11:
        return KC_F11;
    case Qt::Key_F12:
        return KC_F12;
    case Qt::Key_F13:
        return KC_F13;
    case Qt::Key_F14:
        return KC_F14;
    case Qt::Key_F15:
        return KC_F15;

    case Qt::Key_NumLock:
        return KC_NUMLOCK;
    case Qt::Key_ScrollLock:
        return KC_SCROLL;
    case Qt::Key_Less:
        return KC_OEM_102;
    case Qt::Key_Colon:
        return KC_COLON;
    case Qt::Key_Underscore:
        return KC_UNDERLINE;
    case Qt::Key_Enter:
        return KC_NUMPADENTER;
    case Qt::Key_Pause:
        return KC_PAUSE;
    case Qt::Key_Home:
        return KC_HOME;
    case Qt::Key_Up:
        return KC_UP;
    case Qt::Key_Left:
        return KC_LEFT;
    case Qt::Key_Right:
        return KC_RIGHT;
    case Qt::Key_End:
        return KC_END;
    case Qt::Key_Down:
        return KC_DOWN;
    case Qt::Key_PageUp:
        return KC_PGUP;
    case Qt::Key_PageDown:
        return KC_PGDOWN;
    case Qt::Key_Insert:
        return KC_INSERT;
    case Qt::Key_Delete:
        return KC_DELETE;
    case Qt::Key_Meta:
        return KC_LWIN;
    default:
        return KC_UNASSIGNED;
    }
}

void OgreWidget::keyPressEvent(QKeyEvent* e)
{
    using namespace ScapeEngine;
    if (mActive)
    {
        DeviceButtonId::EDeviceButtonId button = toButton(e->key(), e->modifiers());
        mDeviceButtonPressed[button] = true;
        mInputManager->onDeviceButtonPressed(button);
    }
}
void OgreWidget::keyReleaseEvent(QKeyEvent* e)
{
    using namespace ScapeEngine;
    if (mActive)
    {
        DeviceButtonId::EDeviceButtonId button = toButton(e->key(), e->modifiers());
        mDeviceButtonPressed[button] = false;
        mInputManager->onDeviceButtonReleased(button);
    }
}

void OgreWidget::mouseMoveEvent(QMouseEvent* e)
{
    using namespace ScapeEngine;
    if (mActive)
    {
        mPointerPressure = -1;
        IVector3 newPosition(e->x(), e->y(), 0);
        mPointerDeltaPosition = newPosition - mPointerPosition;

        if (newPosition.x != mPointerPosition.x || newPosition.y != mPointerPosition.y ||
            newPosition.z != mPointerPosition.z)
        {
            mPointerPosition = newPosition;
            mInputManager->onPointerMove();
        }
    }
}

void OgreWidget::focusOutEvent(QFocusEvent* evt)
{
    if (mHasView)
    {
        mEngineInterface->onRenderViewKillFocus(1);
    }
    mHasFocus = false;
    evt->setAccepted(true);
}

void OgreWidget::focusInEvent(QFocusEvent* evt)
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
        mEngineInterface->createRenderView(1, s.str().c_str(), this->x(), this->y(), this->width(),
                                           this->height());
        mHasView = true;
    }
    else
    {
        mEngineInterface->onRenderViewSetFocus(1);
        mEngineInterface->onRenderViewMovedOrResized(1, this->x(), this->y(), this->width(),
                                                     this->height());
        mEngineInterface->update();
    }
}

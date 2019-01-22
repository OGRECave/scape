/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "InputManager.h"
#include "InputListener.h"
#include "TabletInput.h"

using namespace ScapeEngine;
using namespace ScapeEngine::DeviceButtonId;

InputListener::InputListener(InputManager* inputManager)
    : mInputManager(inputManager), mPointerPosition(IVector3::ZERO), mPointerDeltaPosition(IVector3::ZERO),
      mPointerMovement(IVector3::ZERO)
{
    mTabletInput = TabletInput::create("");
    mTabletInput->connect();

    mActive = true;
    for (int i = 0; i < DeviceButtonId::DEVICEBUTTONID_ENUM_LENGTH; ++i)
    {
        mDeviceButtonPressed[i] = false;
    }
}

InputListener::~InputListener()
{
    if (mTabletInput)
    {
        mTabletInput->disconnect();
        SAFE_DELETE(mTabletInput);
    }
}

void InputListener::captureInput()
{
    if (mActive)
    {
        mPointerPressure = -1;
        if (mTabletInput)
        {
            Ogre::Real pressure = mTabletInput->getPressure();
            if (pressure >= 0.0f)
            {
                mPointerPressure = Utils::clamp((int)(pressure * 1000.0f), 0, 1000);
            }
        }
    }
}

int InputListener::getAnalogInput(AnalogInputId::EAnalogInputId analogInputId) const
{
    switch (analogInputId)
    {
    case AnalogInputId::POINTER_X:
        return mPointerPosition.x;

    case AnalogInputId::POINTER_Y:
        return mPointerPosition.y;

    case AnalogInputId::POINTER_Z:
        return mPointerPosition.z;

    case AnalogInputId::POINTER_DELTAX:
        return mPointerDeltaPosition.x;

    case AnalogInputId::POINTER_DELTAY:
        return mPointerDeltaPosition.y;

    case AnalogInputId::POINTER_DELTAZ:
        return mPointerDeltaPosition.z;

    case AnalogInputId::POINTER_MOVEX:
        return mPointerMovement.x;

    case AnalogInputId::POINTER_MOVEY:
        return mPointerMovement.y;

    case AnalogInputId::POINTER_MOVEZ:
        return mPointerMovement.z;

    case AnalogInputId::POINTER_PRESSURE:
        return mPointerPressure;
    }
    return 0;
}

bool InputListener::isDeviceButtonPressed(DeviceButtonId::EDeviceButtonId deviceButton) const
{
    return mDeviceButtonPressed[deviceButton];
}

void InputListener::setActive(bool active)
{
    if (mActive != active && mInputManager)
    {
        mActive = active;

        for (int index = 0; index < DeviceButtonId::DEVICEBUTTONID_ENUM_LENGTH; index++)
        {
            bool wasPressed = mDeviceButtonPressed[(EDeviceButtonId)index];
            if (!active && wasPressed)
            {
                mDeviceButtonPressed[(EDeviceButtonId)index] = false;
                mInputManager->onDeviceButtonReleased((EDeviceButtonId)index);
            }
        }
    }
}

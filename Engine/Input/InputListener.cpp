/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "InputManager.h"
#include "InputListener.h"
#include "TabletInput.h"

using namespace ScapeEngine;
using namespace ScapeEngine::DeviceButtonId;

// ----------------------------------------------------------------------------
InputListener::InputListener(InputManager* inputManager, const string& inputWindow) :
	mInputManager(inputManager),
	mPointerPosition(IVector3::ZERO),
	mPointerDeltaPosition(IVector3::ZERO),
	mPointerMovement(IVector3::ZERO)
{
	Ogre::LogManager::getSingletonPtr()->logMessage(_T("*** Initializing OIS ***"));

	OIS::ParamList pl;	
	pl.insert(std::make_pair(string(_T("WINDOW")), inputWindow));
	pl.insert(std::make_pair(string(_T("w32_mouse")), string(_T("DISCL_BACKGROUND"))));
	pl.insert(std::make_pair(string(_T("w32_mouse")), string(_T("DISCL_NONEXCLUSIVE"))));
	pl.insert(std::make_pair(string(_T("w32_keyboard")), string(_T("DISCL_BACKGROUND"))));
	pl.insert(std::make_pair(string(_T("w32_keyboard")), string(_T("DISCL_NONEXCLUSIVE"))));

	mOISInputManager = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(mOISInputManager->createInputObject( OIS::OISKeyboard, true ));
	mKeyboard->setEventCallback(this);
	mKeyboard->setBuffered(true);

	mMouse = static_cast<OIS::Mouse*>(mOISInputManager->createInputObject( OIS::OISMouse, true ));
	mMouse->setEventCallback(this);
	mMouse->setBuffered(true);

	mMouse->getMouseState().width = 10000;
	mMouse->getMouseState().height = 10000;

	mTabletInput = TabletInput::create(inputWindow);
	mTabletInput->connect();

	mActive = true;
	for (int i = 0; i < DeviceButtonId::DEVICEBUTTONID_ENUM_LENGTH; ++i)
	{
		mDeviceButtonPressed[i] = false;
	}
}


// ----------------------------------------------------------------------------
InputListener::~InputListener()
{
	if (mTabletInput)
	{
		mTabletInput->disconnect();
		SAFE_DELETE(mTabletInput);
	}

	if (mOISInputManager)
	{
		if (mKeyboard)
		{
			mOISInputManager->destroyInputObject(mKeyboard); 
			mKeyboard = NULL;
		}
		if (mMouse)
		{
			mOISInputManager->destroyInputObject(mMouse); 
			mMouse = NULL;
		}
		OIS::InputManager::destroyInputSystem(mOISInputManager);
		mOISInputManager = NULL;
	}
}


// ----------------------------------------------------------------------------
void InputListener::captureInput()
{
	if (mActive)
	{
		if (mKeyboard) mKeyboard->capture();
	
		if (mMouse)
		{
			IVector3 oldPosition = mPointerPosition;
			mMouse->capture();
			const OIS::MouseState &ms = mMouse->getMouseState();
			// seems to return zero values before any mouse event has fired since the start of the app
			IVector3 newPosition(ms.X.abs, ms.Y.abs, ms.Z.abs);
			mPointerDeltaPosition = newPosition - oldPosition;
			mPointerPosition = newPosition;
			mPointerMovement = IVector3(ms.X.rel, ms.Y.rel, ms.Z.rel);
		}

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


// ----------------------------------------------------------------------------
int InputListener::getAnalogInput(AnalogInputId::EAnalogInputId analogInputId) const
{
	if (mMouse)
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
	}
	return 0;
}


// ----------------------------------------------------------------------------
bool InputListener::isDeviceButtonPressed(DeviceButtonId::EDeviceButtonId deviceButton) const
{
	return mDeviceButtonPressed[deviceButton];

}


// ----------------------------------------------------------------------------
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


// ----------------------------------------------------------------------------
bool InputListener::keyPressed( const OIS::KeyEvent &e ) 
{
	if (mActive)
	{
		EDeviceButtonId button = DeviceButtonId::getDeviceButtonIdFromOISKeyCode(e.key);
		mDeviceButtonPressed[button] = true;
		mInputManager->onDeviceButtonPressed(button);
	}
	return true;
}


// ----------------------------------------------------------------------------
bool InputListener::keyReleased( const OIS::KeyEvent &e ) 
{
	if (mActive)
	{
		EDeviceButtonId button = DeviceButtonId::getDeviceButtonIdFromOISKeyCode(e.key);
		mDeviceButtonPressed[button] = false;
		mInputManager->onDeviceButtonReleased(button);
	}
	return true;
}


// ----------------------------------------------------------------------------
bool InputListener::mouseMoved( const OIS::MouseEvent &e ) 
{
	if (mActive)
	{
		mPointerPressure = -1;
		if (mTabletInput)
		{
			mTabletInput->capture();
			Ogre::Real pressure = mTabletInput->getPressure();
			if (pressure >= 0.0f)
			{
				mPointerPressure = Utils::clamp((int)(pressure * 1000.0f), 0, 1000);
			}
		}

		const OIS::MouseState &ms = mMouse->getMouseState();
		IVector3 newPosition(ms.X.abs, ms.Y.abs, ms.Z.abs);
		if (newPosition.x != mPointerPosition.x || 
			newPosition.y != mPointerPosition.y || 
			newPosition.z != mPointerPosition.z)
		{
			mPointerPosition = newPosition;
			mInputManager->onPointerMove();
		}
	}
	return true;
}


// ----------------------------------------------------------------------------
bool InputListener::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) 
{
	if (mActive)
	{
		EDeviceButtonId button = DeviceButtonId::getDeviceButtonIdFromOISMouseButtonID(id);
		mDeviceButtonPressed[button] = true;
		mInputManager->onDeviceButtonPressed(button);
	}
	return true;
}


// ----------------------------------------------------------------------------
bool InputListener::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) 
{
	if (mActive)
	{
		EDeviceButtonId button = DeviceButtonId::getDeviceButtonIdFromOISMouseButtonID(id);
		mDeviceButtonPressed[button] = false;
		mInputManager->onDeviceButtonReleased(button);
	}
	return true;
}
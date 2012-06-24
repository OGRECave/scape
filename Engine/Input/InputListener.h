/**
 * ScapeEngine::InputListener class
 *
 * OIS mouse and keyboard listener
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __INPUTLISTENER_H__
#define __INPUTLISTENER_H__


#include "ButtonId.h"
#include "AnalogInputId.h"
#include "DeviceButtonId.h"


namespace ScapeEngine
{
	class TabletInput;

	class InputListener : public OIS::KeyListener, public OIS::MouseListener 
	{

	public:
		// Construct InputListener for the given InputManager and root window
		InputListener(class InputManager* inputManager, const string& inputWindow);

		// Destroy the InputListener
		~InputListener();

		// Capture all input events and update state. Call this at the beginning of each renderloop cycle
		void captureInput();

		// Is the specified DeviceButtonId currently pressed?
		bool isDeviceButtonPressed(DeviceButtonId::EDeviceButtonId deviceButton) const;

		// Get the analog input of the specified identifier
		int getAnalogInput(AnalogInputId::EAnalogInputId input) const;

		// OIS::KeyListener interface implementations
		bool keyPressed(const OIS::KeyEvent &e);
		bool keyReleased(const OIS::KeyEvent &e);

		// OIS::MouseListener interface implementation
		bool mouseMoved(const OIS::MouseEvent &e);
		bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

		void setActive(bool active);

	protected:
		// OIS mouse device reference
		OIS::Mouse*			mMouse;

		// OIS mouse device reference
		OIS::Keyboard*		mKeyboard;

		// OIS input manager reference
		OIS::InputManager*	mOISInputManager;

		// Tablet input reference
		TabletInput*		mTabletInput;

		// Reference to InputManager
		InputManager*		mInputManager;

		// Absolute pointer position
		IVector3			mPointerPosition;

		// Difference between current and previous absolute pointer position
		IVector3			mPointerDeltaPosition;

		// Unaccelerated movement of pointer (e.g. mickeys)
		IVector3			mPointerMovement;

		// Current pointer pressure. Negative if (and only if) not supported.
		int					mPointerPressure;

		bool				mActive;
		bool				mReactivate;
		bool				mDeviceButtonPressed[DeviceButtonId::DEVICEBUTTONID_ENUM_LENGTH];

	};
}

#endif // __INPUTLISTENER_H__
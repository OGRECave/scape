/**
 * ScapeEngine::InputListener class
 *
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

	class InputListener
	{

	public:
		// Construct InputListener for the given InputManager and root window
		InputListener(class InputManager* inputManager);

		// Destroy the InputListener
		~InputListener();

		// Capture all input events and update state. Call this at the beginning of each renderloop cycle
		void captureInput();

		// Is the specified DeviceButtonId currently pressed?
		bool isDeviceButtonPressed(DeviceButtonId::EDeviceButtonId deviceButton) const;

		// Get the analog input of the specified identifier
		int getAnalogInput(AnalogInputId::EAnalogInputId input) const;

		void setActive(bool active);

	protected:
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

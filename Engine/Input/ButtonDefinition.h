/**
 * ScapeEngine::ButtonDefinition class
 *
 * Definition of a button identifier through its list of defining hardware buttons
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __BUTTONDEFINITION_H__
#define __BUTTONDEFINITION_H__


#include "ButtonId.h"
#include "AnalogInputId.h"
#include "DeviceButtonId.h"


namespace ScapeEngine
{

	class InputManager;

	class ButtonDefinition
	{

		friend InputManager;

	public:
		// Construct a new definition of the given button identifier
		ButtonDefinition(ButtonId::EButtonId buttonId)
		{
			mButtonId = buttonId;
		}

		// Add a device button button to this definition
		void ButtonDefinition::addDeviceButton(DeviceButtonId::EDeviceButtonId buttonId) 
		{
			mDeviceButtons.push_front(buttonId);
		}


	protected:
		// Identifier of defined button
		ButtonId::EButtonId mButtonId;

		// Device button ordered container type
		typedef std::list<DeviceButtonId::EDeviceButtonId> DeviceButtonList;

		// Device button container
		DeviceButtonList mDeviceButtons;
	};
}

#endif // __BUTTONDEFINITION_H__
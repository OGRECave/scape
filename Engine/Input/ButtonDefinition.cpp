/**
 * G.J.P. de Carpentier, 2007 
 */

#include "PCH/stdafx.h"
#include "DeviceButton.h"
#include "ButtonDefinition.h"

using namespace ScapeEngine;


// ----------------------------------------------------------------------------
ButtonDefinition::ButtonDefinition(ButtonId::EButtonId buttonId) 
{
	mButtonId = buttonId;
}


// ----------------------------------------------------------------------------
void ButtonDefinition::addDeviceButton(OIS::MouseButtonID buttonId) 
{
	mDeviceButtons.push_front(DeviceButton(buttonId));
}


// ----------------------------------------------------------------------------
void ButtonDefinition::addDeviceButton(OIS::KeyCode keyCode) 
{
	mDeviceButtons.push_front(DeviceButton(keyCode));
}
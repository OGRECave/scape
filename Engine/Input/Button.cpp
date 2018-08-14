/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "Button.h"
#include "ButtonId.h"
#include "AnalogInputId.h"
#include "InputManager.h"



using namespace ScapeEngine;


// ----------------------------------------------------------------------------
Button::Button(ButtonId::EButtonId buttonId) :
	mButtonId(buttonId),
	mPressed(0),
	mJustPressed(0),
	mJustReleased(0),
	mOrdered(0),
	mPriority(0)
{
}


// ----------------------------------------------------------------------------
bool Button::isPressed(bool onlyHighestPriority) const
{
	return (mPressed || mJustPressed) && (!onlyHighestPriority || getEngineCore()->getInputManager()->getHighestPriorityPressed() == mPriority);
}


// ----------------------------------------------------------------------------
bool Button::isReleased() const
{
	return !mPressed || mJustReleased;
}


// ----------------------------------------------------------------------------
bool Button::isJustPressed(bool onlyHighestPriority) const
{
	return mJustPressed && (!onlyHighestPriority || getEngineCore()->getInputManager()->getHighestPriorityPressed() == mPriority);
}


// ----------------------------------------------------------------------------
bool Button::isJustReleased() const
{
	return mJustReleased;
}


// ----------------------------------------------------------------------------
bool Button::isOrdered() const
{
	return mOrdered;
}


// ----------------------------------------------------------------------------
ButtonId::EButtonId Button::getButtonId() const
{
	return mButtonId;
}


// ----------------------------------------------------------------------------
int Button::getPriority() const
{
	return mPriority;
}


// ----------------------------------------------------------------------------
void Button::setPriority(int priority)
{
	mPriority = priority;
}

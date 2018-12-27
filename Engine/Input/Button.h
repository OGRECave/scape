/**
 * ScapeEngine::Button class
 *
 * Virtual button, managed and coupled to a ButtonDefinition by InputManager
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "ScapeEnginePrerequisites.h"

#include "ButtonId.h"
#include "AnalogInputId.h"

namespace ScapeEngine
{

class InputManager;

class Button
{

    friend InputManager;

public:
    // Create a button for the given unique buttonId
    Button(ButtonId::EButtonId buttonId);

    // Is button currently pressed? (this frame). If onlyHighestPriority is true,
    // only returns true if pressed and is the highest assigned priority of all
    // currently pressed buttons.
    bool isPressed(bool onlyHighestPriority = true) const;

    // Is button currently released?
    bool isReleased() const;

    // Is the button pressed now while it wasn't pressed in the last frame?
    bool isJustPressed(bool onlyHighestPriority = true) const;

    // Is the button released now while it was pressed in the last frame?
    bool isJustReleased() const;

    // Is the button pressed or released in the right order?
    // This is only relevant for virtual buttons that use a button definition
    // that consists of multiple hardware buttons
    bool isOrdered() const;

    // Get the button identifier for this button
    ButtonId::EButtonId getButtonId() const;

    // Get the assigned priority for this button
    int getPriority() const;

    // Assign priority to this button
    void setPriority(int priority);

protected:
    // Button identifier of this button
    ButtonId::EButtonId mButtonId;

    // Is button currently pressed?
    bool mPressed;

    // Has the button just been pressed?
    bool mJustPressed;

    // Has the button just been released?
    bool mJustReleased;

    // Was the button press or release in the order of its definition
    bool mOrdered;

    // Assigned button priority
    int mPriority;
};
}

#endif // __BUTTON_H__
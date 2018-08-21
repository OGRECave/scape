/**
 * ScapeEngine::InputManager class
 *
 * Analog input and button manager
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__


#include "ButtonId.h"
#include "AnalogInputId.h"
#include "Button.h"
#include "ButtonDefinition.h"
#include "Utils/Utils.h"


namespace ScapeEngine
{

	class InputPointer;
	class InputListener;

	class InputManager
	{

	public:

		InputManager();

		~InputManager();

		InputPointer* getInputPointer() {return mInputPointer;}

		// Attach the input manager to the root window to receive input events from
		void attachToWindow(InputListener* input);

		// Add a new button definition. Multiple definitions of the same button
		// identifier may be defined.
		void addButtonDefinition(const ButtonDefinition &buttonDefinition);

		// Remove all added button definitions
		void clearButtonDefinitions();

		// Load a button definition XML file
		void loadButtonDefinitionsFromXML(const string& filename);
		
		// Capture all input events and update state. Call this at the beginning of each renderloop cycle
		void captureInput();

		// Did one or more button states change since the last frame?
		bool hasSomeButtonStateJustChanged() const;

		// Get button reference for the given button identifier
		Button* getButton(ButtonId::EButtonId buttonId);

		// Get analog input reference for the given input identifier
		int getAnalogInput(AnalogInputId::EAnalogInputId input);

		void onPointerMove();

		// Let the InputManager know the state of the given DeviceButtonId changed to being pressed
		void onDeviceButtonPressed(DeviceButtonId::EDeviceButtonId deviceButton);

		// Let the InputManager know the state of the given DeviceButtonId changed to being released
		void onDeviceButtonReleased(DeviceButtonId::EDeviceButtonId deviceButton);

		// Get the highest priority of all buttons currently being pressed
		int getHighestPriorityPressed() const;

		// Get the view identifier that currently should have the input foces
		class RenderView* getFocusedRenderView() const;

		void onRenderViewSetFocus(long viewId);

		void onRenderViewKillFocus(long viewId);

	protected:

		// Button definition container type 
		typedef std::list<ButtonDefinition> ButtonDefinitions;

		// Button definition container
		ButtonDefinitions mButtonDefinitions;

		// Button object container type
		typedef std::map<ButtonId::EButtonId, Button*> Buttons;

		// Button object container
		Buttons mButtons;

		// The highest priority of all buttons currently being pressed
		int mHighestPriorityPressed;

		// The view identifier that currently should have the input foces
		long mFocusedRenderViewId;
		bool mHasFocusedRenderView;

		// Total number of currently pressed Buttons
		int mNumButtonsPressed;

		// Did one or more button states change since the last frame?
		bool mSomeButtonStateJustChanged;

		// Pointer to its input listener, responsible for notifying the InputManager of any input state changes
		class InputListener* mInputListener;

		InputPointer* mInputPointer;

	};

}

#endif // __INPUTMANAGER_H__

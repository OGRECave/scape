/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
#include "InputManager.h"
#include "InputPointer.h"
#include "InputListener.h"
#include "DeviceButtonId.h"
#include "Utils/Utils.h"
#include "EngineCore/RenderViewManager.h"
#include "ButtonDefinitionXMLSerializer.h"

using namespace ScapeEngine;
using namespace ScapeEngine::AnalogInputId;
using namespace ScapeEngine::ButtonId;

template <> InputManager* Ogre::Singleton<InputManager>::msSingleton = 0;

// ----------------------------------------------------------------------------
InputManager::InputManager()
    : mInputListener(NULL), mHighestPriorityPressed(-1), mHasFocusedRenderView(false),
      mNumButtonsPressed(0), mSomeButtonStateJustChanged(false)
{
    Ogre::LogManager::getSingletonPtr()->logMessage(_T("Initializing InputManager singleton"));
    mInputPointer = new InputPointer(this);
}

// ----------------------------------------------------------------------------
InputManager::~InputManager()
{
    Ogre::LogManager::getSingletonPtr()->logMessage(_T("Deconstructing InputManager singleton"));

    delete mInputPointer;

    for (Buttons::iterator buttonIt = mButtons.begin(); buttonIt != mButtons.end(); ++buttonIt)
    {
        SAFE_DELETE(buttonIt->second);
    }
}

// ----------------------------------------------------------------------------
void InputManager::attachToWindow(InputListener* input)
{
    if (!mInputListener)
    {
        mInputListener = input;
    }
}

// ----------------------------------------------------------------------------
void InputManager::addButtonDefinition(const ButtonDefinition& buttonDefinition)
{
    if (!buttonDefinition.mDeviceButtons.empty())
    {
        mButtonDefinitions.push_front(buttonDefinition);
        getButton(buttonDefinition.mButtonId); // make sure it exists
    }
}

// ----------------------------------------------------------------------------
void InputManager::clearButtonDefinitions() { mButtonDefinitions.clear(); }

// ----------------------------------------------------------------------------
void InputManager::captureInput()
{
    if (mInputPointer)
        mInputPointer->startNewFrame();

    mSomeButtonStateJustChanged = false;

    if (mNumButtonsPressed == 0 || mFocusedRenderViewId == -1)
    {
        mFocusedRenderViewId = getEngineCore()->getRenderViewManager()->getViewIdAtRootSpacePosition(
            getInputPointer()->getCurrentState().getWindow2DPoint());
    }

    for (Buttons::iterator buttonIt = mButtons.begin(); buttonIt != mButtons.end(); ++buttonIt)
    {
        buttonIt->second->mJustPressed = false;
        buttonIt->second->mJustReleased = false;
        buttonIt->second->mOrdered = buttonIt->second->mOrdered && buttonIt->second->mPressed;
    }

    if (mInputListener)
        mInputListener->captureInput();
}

// ----------------------------------------------------------------------------
Button* InputManager::getButton(EButtonId buttonId)
{
    Button* button = mButtons[buttonId];
    if (button == NULL)
    {
        button = mButtons[buttonId] = new Button(buttonId);
    }
    return button;
}

// ----------------------------------------------------------------------------
int InputManager::getAnalogInput(EAnalogInputId analogInputId)
{
    if (mInputListener)
        return mInputListener->getAnalogInput(analogInputId);
    return 0;
}

// ----------------------------------------------------------------------------
int InputManager::getHighestPriorityPressed() const { return mHighestPriorityPressed; }

// ----------------------------------------------------------------------------
bool InputManager::hasSomeButtonStateJustChanged() const { return mSomeButtonStateJustChanged; }

// ----------------------------------------------------------------------------
RenderView* InputManager::getFocusedRenderView() const
{
    if (mHasFocusedRenderView)
    {
        return getEngineCore()->getRenderViewManager()->getRenderView(mFocusedRenderViewId);
    }
    return NULL;
}

// ----------------------------------------------------------------------------
void InputManager::loadButtonDefinitions()
{
    if (mButtonDefinitionDataAccessObject)
    {
        const ButtonDefinitionDataAccessObject::ButtonDefinitions def =
            mButtonDefinitionDataAccessObject->getButtonDefinitions();
        for (ButtonDefinitionDataAccessObject::ButtonDefinitions::const_iterator it = def.begin();
             it != def.end(); it++)
        {
            addButtonDefinition(*it);
            getButton(it->mButtonId)->setPriority(it->mPriority);
        }
    }
}

// ----------------------------------------------------------------------------
void InputManager::onPointerMove()
{
    if (mInputPointer)
        mInputPointer->update();
}

// ----------------------------------------------------------------------------
void InputManager::onDeviceButtonPressed(DeviceButtonId::EDeviceButtonId deviceButton)
{
    // printf("devbutton press: %d\n", deviceButton);
    mSomeButtonStateJustChanged = true;

    for (ButtonDefinitions::iterator defIt = mButtonDefinitions.begin(); defIt != mButtonDefinitions.end();
         ++defIt)
    {
        if (!mButtons[defIt->mButtonId]->mPressed)
        {
            bool pressed = true;
            for (ButtonDefinition::DeviceButtonList::const_iterator defDeviceButtonIt =
                     defIt->mDeviceButtons.begin();
                 defDeviceButtonIt != defIt->mDeviceButtons.end(); ++defDeviceButtonIt)
            {
                if (!mInputListener->isDeviceButtonPressed(*defDeviceButtonIt))
                {
                    pressed = false;
                    break;
                }
            }

            if (pressed)
            {
                Button* button = mButtons[defIt->mButtonId];
                button->mPressed = true;
                button->mJustPressed = true;
                button->mOrdered = deviceButton == *defIt->mDeviceButtons.begin();

                ++mNumButtonsPressed;

                mHighestPriorityPressed = Utils::max(mHighestPriorityPressed, button->getPriority());

                // printf("button %d pressed. sequenced: %d\n", (int)defIt->mButtonId, button->mOrdered);
            }
        }
    }
}

// ----------------------------------------------------------------------------
void InputManager::onDeviceButtonReleased(DeviceButtonId::EDeviceButtonId deviceButton)
{
    mSomeButtonStateJustChanged = true;
    // printf("devbutton release: %d\n", deviceButton);

    for (ButtonDefinitions::iterator defIt = mButtonDefinitions.begin(); defIt != mButtonDefinitions.end();
         ++defIt)
    {
        Button* button = mButtons[defIt->mButtonId];
        if (button->mPressed)
        {
            for (ButtonDefinition::DeviceButtonList::const_iterator defDeviceButtonIt =
                     defIt->mDeviceButtons.begin();
                 defDeviceButtonIt != defIt->mDeviceButtons.end(); ++defDeviceButtonIt)
            {
                if (deviceButton == *defDeviceButtonIt)
                {
                    button->mPressed = false;
                    button->mJustReleased = true;

                    mNumButtonsPressed--;

                    // printf("button %d released.\n", (int)defIt->mButtonId);
                    break;
                }
            }
        }
    }

    mHighestPriorityPressed = -1;
    for (Buttons::iterator buttonIt = mButtons.begin(); buttonIt != mButtons.end(); ++buttonIt)
    {
        if (buttonIt->second->mPressed)
        {
            mHighestPriorityPressed = Utils::max(mHighestPriorityPressed, buttonIt->second->getPriority());
        }
    }

    if (mNumButtonsPressed == 0)
    {
        mFocusedRenderViewId = -1;
    }
}

// ----------------------------------------------------------------------------
void InputManager::onRenderViewSetFocus(long viewId)
{
    if (mInputListener)
    {
        mInputListener->setActive(true);
    }
    mHasFocusedRenderView = true;
    mFocusedRenderViewId = viewId;
}

// ----------------------------------------------------------------------------
void InputManager::onRenderViewKillFocus(long viewId)
{
    if (viewId == mFocusedRenderViewId)
    {
        mHasFocusedRenderView = false;
    }
    if (mInputListener)
    {
        mInputListener->setActive(false);
    }
}

std::shared_ptr<ButtonDefinitionDataAccessObject> InputManager::getButtonDefinitionDataAccessObject()
{
    return mButtonDefinitionDataAccessObject;
}

void InputManager::setButtonDefinitionDataAccessObject(
    std::shared_ptr<ButtonDefinitionDataAccessObject> buttonDefinitionDataAccessObject)
{
    mButtonDefinitionDataAccessObject = buttonDefinitionDataAccessObject;
}

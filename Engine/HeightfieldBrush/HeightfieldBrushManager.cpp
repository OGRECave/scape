/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldBrush.h"
#include "HeightfieldBrushManager.h"
#include "HeightfieldBrushSettings.h"
#include "HeightfieldBrushState.h"
#include "HeightfieldOperation/HeightfieldOperation.h"
#include "HeightfieldOperation/HeightfieldOperationStack.h"
#include "Input/InputManager.h"
#include "Input/InputPointer.h"

using namespace ScapeEngine;

HeightfieldBrushManager::HeightfieldBrushManager() : mHeightfieldBrushSettings(), mHeightfieldBrushState()
{
    mHeightfieldBrushSettings.setInnerRadius(50.f);
    mHeightfieldBrushSettings.setOuterRadius(250.f);
}

HeightfieldBrushManager::~HeightfieldBrushManager()
{
    Brushes::iterator brushIt = mBrushes.begin(), brushItEnd = mBrushes.end();
    for (; brushIt != brushItEnd; ++brushIt)
    {
        delete brushIt->second;
    }
}

HeightfieldBrush* HeightfieldBrushManager::getCurrentBrush() const
{
    HeightfieldOperation* operation = getEngineCore()->getHeightfieldOperationStack()->getLastOperation();
    if (!operation || !operation->isActive())
    {
        operation = getEngineCore()->getHeightfieldOperationStack()->getNewOperation();
    }

    if (operation)
    {
        return operation->getBrush();
    }
    return NULL;
}

HeightfieldBrush* HeightfieldBrushManager::getBrush(const string& brushName)
{
    HeightfieldBrush* brush = mBrushes[brushName];
    if (brush == NULL)
    {
        brush = mBrushes[brushName] = new HeightfieldBrush();
    }
    // printf("%s %p\n", brushName.c_str(), brush);
    return brush;
}

HeightfieldBrushSettings& HeightfieldBrushManager::getHeightfieldBrushSettings()
{
    return mHeightfieldBrushSettings;
}

void HeightfieldBrushManager::tick()
{
    InputManager* inputManager = getEngineCore()->getInputManager();

    if (inputManager->getButton(ButtonId::BRUSH_CHANGERADIUS)->isPressed())
    {
        Ogre::Real sizeChangeRatio = expf(-0.6f * getEngineCore()->getTimeSinceLastFrame() *
                                          (inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY) -
                                           inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAX)));
        Ogre::Real newOuterRadius =
            std::max(0.5f, mHeightfieldBrushSettings.getOuterRadius() * sizeChangeRatio);
        mHeightfieldBrushSettings.setInnerRadius(newOuterRadius *
                                                 mHeightfieldBrushSettings.getInnerRadius() /
                                                 mHeightfieldBrushSettings.getOuterRadius());
        mHeightfieldBrushSettings.setOuterRadius(newOuterRadius);
    }

    Ogre::Real brushSizeHL = 2.00f;
    Ogre::Real sizeChangeRatio = expf(-getEngineCore()->getTimeSinceLastFrame() / brushSizeHL);
    if (inputManager->getButton(ButtonId::BRUSH_CHANGEFALLOFF)->isPressed())
    {
        Ogre::Real sizeChangeRatio = expf(-0.6f * getEngineCore()->getTimeSinceLastFrame() *
                                          (inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY) -
                                           inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAX)));
        sizeChangeRatio = Utils::clamp(mHeightfieldBrushSettings.getInnerRadius() * sizeChangeRatio, 0.5f,
                                       mHeightfieldBrushSettings.getOuterRadius() * 0.98f);
        mHeightfieldBrushSettings.setInnerRadius(sizeChangeRatio);
    }

    if (inputManager->getButton(ButtonId::BRUSH_CHANGERAMPPOWER)->isPressed())
    {
        Ogre::Real sizeChangeRatio = expf(-0.6f * getEngineCore()->getTimeSinceLastFrame() *
                                          -(inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY) -
                                            inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAX)));
        mHeightfieldBrushSettings.setRampPower(
            Utils::clamp(mHeightfieldBrushSettings.getRampPower() * sizeChangeRatio, 0.04f, 25.0f));
    }

    if (inputManager->getButton(ButtonId::BRUSH_GROW)->isPressed())
    {
        sizeChangeRatio *= std::min(1.0f, 0.5f * (mHeightfieldBrushSettings.getInnerRadius() +
                                                  mHeightfieldBrushSettings.getOuterRadius()) *
                                              sizeChangeRatio);
        mHeightfieldBrushSettings.setInnerRadius(mHeightfieldBrushSettings.getInnerRadius() /
                                                 sizeChangeRatio);
        mHeightfieldBrushSettings.setOuterRadius(mHeightfieldBrushSettings.getOuterRadius() /
                                                 sizeChangeRatio);
    }

    if (inputManager->getButton(ButtonId::BRUSH_SHRINK)->isPressed())
    {
        sizeChangeRatio /= std::min(1.0f, 0.5f * (mHeightfieldBrushSettings.getInnerRadius() +
                                                  mHeightfieldBrushSettings.getOuterRadius()) *
                                              sizeChangeRatio);
        mHeightfieldBrushSettings.setInnerRadius(mHeightfieldBrushSettings.getInnerRadius() *
                                                 sizeChangeRatio);
        mHeightfieldBrushSettings.setOuterRadius(mHeightfieldBrushSettings.getOuterRadius() *
                                                 sizeChangeRatio);
    }
}

HeightfieldBrushState& HeightfieldBrushManager::getHeightfieldBrushState()
{
    return mHeightfieldBrushState;
}

/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldBrush.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "Input/InputManager.h"
#include "Input/InputPointer.h"

#include <iostream>

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldBrush::HeightfieldBrush()
    : mPosition(0.0f, 0.0f, 0.0f), mOuterRadius(1.0f), mInnerRadius(0.0f), mRampPower(1.0f),
      mPrimaryActive(false), mSecondaryActive(false), mHeightfieldGeom(NULL)
{
}

// ----------------------------------------------------------------------------
void HeightfieldBrush::setInnerRadius(Ogre::Real radius)
{
    // mInnerRadius = radius;//std::max(radius, 0.001f);
    mInnerRadius = radius; // std::min(radius, mOuterRadius);
}

// ----------------------------------------------------------------------------
void HeightfieldBrush::setOuterRadius(Ogre::Real radius)
{
    mOuterRadius = radius; // std::max(mInnerRadius, radius);
}

// ----------------------------------------------------------------------------
void HeightfieldBrush::setRadius(Ogre::Real radius) { mInnerRadius = mOuterRadius = radius; }

// ----------------------------------------------------------------------------
void HeightfieldBrush::setPosition(const Ogre::Vector3& position) { mPosition = position; }

// ----------------------------------------------------------------------------
void HeightfieldBrush::setPressure(Ogre::Real pressure) { mPressure = pressure; }

// ----------------------------------------------------------------------------
void HeightfieldBrush::setRampPower(Ogre::Real rampPower) { mRampPower = rampPower; }

// ----------------------------------------------------------------------------
void HeightfieldBrush::tick()
{
    InputManager* inputManager = getEngineCore()->getInputManager();

    setPosition(inputManager->getInputPointer()->getCurrentState().get3DPoint());

    Ogre::Real pointerPressure = inputManager->getInputPointer()->getCurrentState().getPressure();
    // printf("%f\n", pointerPressure);
    if (pointerPressure < 0.0f)
    {
        pointerPressure = 1.0f;
    }

    pointerPressure = powf(pointerPressure, 2.0f);

    setPressure(pointerPressure);

    setHeightfieldGeom(inputManager->getInputPointer()->getCurrentState().get3DPointGeom());

    if (inputManager->getButton(ButtonId::BRUSH_CHANGERADIUS)->isPressed())
    {
        Ogre::Real sizeChangeRatio = expf(-0.6f * getEngineCore()->getTimeSinceLastFrame() *
                                          (inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY) -
                                           inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAX)));
        Ogre::Real newOuterRadius = std::max(0.5f, getOuterRadius() * sizeChangeRatio);
        setInnerRadius(newOuterRadius * getInnerRadius() / getOuterRadius());
        setOuterRadius(newOuterRadius);
    }

    Ogre::Real brushSizeHL = 2.00f;
    Ogre::Real sizeChangeRatio = expf(-getEngineCore()->getTimeSinceLastFrame() / brushSizeHL);
    if (inputManager->getButton(ButtonId::BRUSH_CHANGEFALLOFF)->isPressed())
    {
        Ogre::Real sizeChangeRatio = expf(-0.6f * getEngineCore()->getTimeSinceLastFrame() *
                                          (inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY) -
                                           inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAX)));
        sizeChangeRatio = Utils::clamp(getInnerRadius() * sizeChangeRatio, 0.5f, getOuterRadius() * 0.98f);
        setInnerRadius(sizeChangeRatio);

        // sizeChangeRatio = pow(sizeChangeRatio,
        // -inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY) +
        // inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAX));
        // Ogre::Real midRadius = 0.5f * (getOuterRadius() + getInnerRadius());
        // Ogre::Real delta = std::max(0.5f, std::min(0.5f * (getOuterRadius() - getInnerRadius()) *
        // sizeChangeRatio, midRadius - 0.5f));
        // setInnerRadius(midRadius - delta);
        // setOuterRadius(midRadius + delta);
    }

    if (inputManager->getButton(ButtonId::BRUSH_CHANGERAMPPOWER)->isPressed())
    {
        Ogre::Real sizeChangeRatio = expf(-0.6f * getEngineCore()->getTimeSinceLastFrame() *
                                          -(inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY) -
                                            inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAX)));
        setRampPower(Utils::clamp(getRampPower() * sizeChangeRatio, 0.04f, 25.0f));
        std::cout << "power: " << getRampPower();
    }

    if (inputManager->getButton(ButtonId::BRUSH_GROW)->isPressed())
    {
        sizeChangeRatio *= std::min(1.0f, 0.5f * (getInnerRadius() + getOuterRadius()) * sizeChangeRatio);
        setInnerRadius(getInnerRadius() / sizeChangeRatio);
        setOuterRadius(getOuterRadius() / sizeChangeRatio);
    }

    if (inputManager->getButton(ButtonId::BRUSH_SHRINK)->isPressed())
    {
        sizeChangeRatio /= std::min(1.0f, 0.5f * (getInnerRadius() + getOuterRadius()) * sizeChangeRatio);
        setInnerRadius(getInnerRadius() * sizeChangeRatio);
        setOuterRadius(getOuterRadius() * sizeChangeRatio);
    }

    /*
    if (inputManager->getButton(ButtonId::BRUSH_SOFTENEDGE)->isPressed())
    {
            Ogre::Real delta = (getOuterRadius() - getInnerRadius());
            Ogre::Real halfRatio = delta * (sizeChangeRatio - 1.0);
            setInnerRadius(getInnerRadius() + delta);
            setOuterRadius(getOuterRadius() - delta);
    }

    if (inputManager->getButton(ButtonId::BRUSH_HARDENEDGE)->isPressed())
    {
            Ogre::Real delta = (getOuterRadius() - getInnerRadius());
            Ogre::Real halfRatio = sizeChangeRatio - 1.0;
            setInnerRadius(getInnerRadius() - delta);
            setOuterRadius(getOuterRadius() + delta);
    }
    */

    //	//mPrimaryActive = getHeightfieldGeom() &&
    //inputManager->getButton(ButtonId::BRUSH_ACTIVE)->isPressed();
    // mSecondaryActive = getHeightfieldGeom() && !mPrimaryActive &&
    // inputManager->getButton(ButtonId::BRUSH_ACTIVE_ALT)->isPressed();
    mPrimaryActive = inputManager->getButton(ButtonId::BRUSH_ACTIVE)->isPressed();
    mSecondaryActive = !mPrimaryActive && inputManager->getButton(ButtonId::BRUSH_ACTIVE_ALT)->isPressed();
}

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

using namespace ScapeEngine;

HeightfieldBrush::HeightfieldBrush()
    : mPosition(0.0f, 0.0f, 0.0f), mOuterRadius(1.0f), mInnerRadius(0.0f), mRampPower(1.0f),
      mPrimaryActive(false), mSecondaryActive(false), mHeightfieldGeom(NULL)
{
}

void HeightfieldBrush::setInnerRadius(Ogre::Real radius)
{
    // mInnerRadius = radius;//std::max(radius, 0.001f);
    mInnerRadius = radius; // std::min(radius, mOuterRadius);
}

void HeightfieldBrush::setOuterRadius(Ogre::Real radius)
{
    mOuterRadius = radius; // std::max(mInnerRadius, radius);
}

void HeightfieldBrush::setPosition(const Ogre::Vector3& position) { mPosition = position; }

void HeightfieldBrush::setPressure(Ogre::Real pressure) { mPressure = pressure; }

void HeightfieldBrush::setRampPower(Ogre::Real rampPower) { mRampPower = rampPower; }

void HeightfieldBrush::tick()
{
    InputManager* inputManager = getEngineCore()->getInputManager();

    setPosition(inputManager->getInputPointer()->getCurrentState().get3DPoint());

    Ogre::Real pointerPressure = inputManager->getInputPointer()->getCurrentState().getPressure();
    if (pointerPressure < 0.0f)
    {
        pointerPressure = 1.0f;
    }

    pointerPressure = powf(pointerPressure, 2.0f);

    setPressure(pointerPressure);

    setHeightfieldGeom(inputManager->getInputPointer()->getCurrentState().get3DPointGeom());

    mPrimaryActive = inputManager->getButton(ButtonId::BRUSH_ACTIVE)->isPressed();
    mSecondaryActive = !mPrimaryActive && inputManager->getButton(ButtonId::BRUSH_ACTIVE_ALT)->isPressed();
}

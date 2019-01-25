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
    : mPrimaryActive(false), mSecondaryActive(false), mHeightfieldGeom(NULL)
{
}

void HeightfieldBrush::tick()
{
    InputManager* inputManager = getEngineCore()->getInputManager();

    setHeightfieldGeom(inputManager->getInputPointer()->getCurrentState().get3DPointGeom());

    mPrimaryActive = inputManager->getButton(ButtonId::BRUSH_ACTIVE)->isPressed();
    mSecondaryActive = !mPrimaryActive && inputManager->getButton(ButtonId::BRUSH_ACTIVE_ALT)->isPressed();
}

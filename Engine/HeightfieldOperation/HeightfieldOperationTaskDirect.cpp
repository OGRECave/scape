/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
  */

#include "HeightfieldOperationTaskDirect.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "PCH/stdafx.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldOperationTaskDirect::HeightfieldOperationTaskDirect(HeightfieldOperationBrush* operation,
                                                               const BrushInstance& brushInstance)
    : mOperation(operation), mBrushInstance(brushInstance), mCurrentState(STATE_PENDING),
      mUpdatedRect(Ogre::Rect(0, 0, 0, 0))
{
}

// ----------------------------------------------------------------------------
HeightfieldOperationTaskDirect::~HeightfieldOperationTaskDirect() {}

// ----------------------------------------------------------------------------
void HeightfieldOperationTaskDirect::tick()
{
    if (mCurrentState == STATE_PENDING)
    {
        tickPending();
    }

    if (mCurrentState == STATE_ACTIVE)
    {
        tickActive();
    }
}

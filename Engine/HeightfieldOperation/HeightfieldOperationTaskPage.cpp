/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
  */

#include "PCH/stdafx.h"
#include "HeightfieldOperationTaskPage.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"

using namespace ScapeEngine;


// ----------------------------------------------------------------------------
HeightfieldOperationTaskPage::HeightfieldOperationTaskPage(HeightfieldOperationBrush* operation, const BrushPageCoords pageCoords, const BrushInstances& brushInstances)
:	mOperation(operation),
	mPageCoords(pageCoords),
	mBrushInstances(brushInstances),
	mCurrentState(STATE_PENDING),
	mUpdatedRect(Ogre::Rect(0, 0, 0, 0))
{

}


// ----------------------------------------------------------------------------
HeightfieldOperationTaskPage::~HeightfieldOperationTaskPage()
{
}



// ----------------------------------------------------------------------------
void HeightfieldOperationTaskPage::tick()
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


// ----------------------------------------------------------------------------
bool HeightfieldOperationTaskPage::addBrushInstances(const BrushInstances& brushInstances)
{
	if (mCurrentState == STATE_PENDING)
	{
		mBrushInstances.insert(mBrushInstances.end(), brushInstances.begin(), brushInstances.end());
		return true;
	}
	return false;	
}



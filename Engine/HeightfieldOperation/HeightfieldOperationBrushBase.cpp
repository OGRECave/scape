/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
  */

#include "PCH/stdafx.h"
#include "HeightfieldOperationBrushBase.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"
#include "Input/InputManager.h"

#include "HeightfieldSerializer/HeightfieldSerializerIncremental.h"


using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldOperationBrushBase::HeightfieldOperationBrushBase()
:	mTempHeightfieldBuffer(NULL),
	mOriginalHeightfieldBuffer(NULL),
	mInOutHeightfieldBuffer(NULL),
	mIsStateActive(false),
	mHasBeenStateActive(false),
	mBrushName(_T("HeightfieldOperationBrushBase")),
	mHeightfieldGeom(NULL),
	mPathSpacing(0.0f),
	mLastTickMillis(0),
	mWasActiveStroke(false),
	mIsActiveStroke(false),
	mPencilMode(false)
{
	mDeltaBufferPtr.setNull();
}


// ----------------------------------------------------------------------------
HeightfieldOperationBrushBase::~HeightfieldOperationBrushBase()
{
	if (mOriginalHeightfieldBuffer)
	{
		mOriginalHeightfieldBuffer->getHeightfieldBufferSet()->destroy(mOriginalHeightfieldBuffer->getHandle());
		mOriginalHeightfieldBuffer = NULL;
	}

	if (mTempHeightfieldBuffer)
	{
		mTempHeightfieldBuffer->getHeightfieldBufferSet()->destroy(mTempHeightfieldBuffer->getHandle());
		mTempHeightfieldBuffer = NULL;
	}

	mDeltaBufferPtr.setNull();
}


// ----------------------------------------------------------------------------
bool HeightfieldOperationBrushBase::canInitiate(EState state)
{
	switch(state)
	{
	case STATE_DO:
		return mBrush && mBrush->isActive() && mBrush->getHeightfieldGeom() && (!mHasBeenStateActive || mState == STATE_UNDO);

	case STATE_UNDO:
		return mHasBeenStateActive && (mState == STATE_DO || mState == STATE_REDO);

	case STATE_REDO:
		return mHasBeenStateActive && mState == STATE_UNDO;

	default:
		return false;
	}
}

// ----------------------------------------------------------------------------
bool HeightfieldOperationBrushBase::isActive(EState state)
{
	return mIsStateActive && mState == state;
}

// ----------------------------------------------------------------------------
bool HeightfieldOperationBrushBase::isActive()
{
	return mIsStateActive;
}

// ----------------------------------------------------------------------------
bool HeightfieldOperationBrushBase::isDone(EState state)
{
	return !mIsStateActive && mHasBeenStateActive && mState == state;
}

// ----------------------------------------------------------------------------
void HeightfieldOperationBrushBase::initiate(EState state, HeightfieldBuffer* inOutHeightfieldBuffer)
{
	mInOutHeightfieldBuffer = inOutHeightfieldBuffer;

	switch(state)
	{
	case STATE_DO:
		initiateDo();
		break;

	case STATE_UNDO:
		initiateUndo();
		break;

	case STATE_REDO:
		initiateRedo();
		break;
	}
}


// ----------------------------------------------------------------------------
void HeightfieldOperationBrushBase::tick()
{
	if (mIsStateActive || !mHasBeenStateActive)
	{
		if (!mBrush)
		{
			mBrush = getEngineCore()->getHeightfieldBrushManager()->getBrush(mBrushName);
		}
		mBrush->tick();
	}

	if (mIsStateActive)
	{
		switch (mState)
		{
		case STATE_DO:
			tickDo();
			break;

		case STATE_UNDO:
			tickUndo();
			break;

		case STATE_REDO:
			tickRedo();
			break;
		}
	}
	else
	{
		tickInactive();
	}

	mLastTickMillis = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
}


// ----------------------------------------------------------------------------
void HeightfieldOperationBrushBase::initiateDo()
{
	assert(!mIsStateActive && canInitiate(STATE_DO));
	mHeightfieldGeom = NULL;
	mState = STATE_DO;
	mIsStateActive = true;
	mHasBeenStateActive = true;
	mWasActiveStroke = false;
	mIsActiveStroke = true;

	HeightfieldBufferSet* bufferSet = mInOutHeightfieldBuffer->getHeightfieldBufferSet();

	assert(mBrush->getHeightfieldGeom());

	if (mTempHeightfieldBuffer)
	{
		bufferSet->destroy(mTempHeightfieldBuffer->getHandle());
		mTempHeightfieldBuffer = NULL;
	}

	if (mOriginalHeightfieldBuffer)
	{
		bufferSet->destroy(mOriginalHeightfieldBuffer->getHandle());
		mOriginalHeightfieldBuffer = NULL;
	}

	string originalName = mInOutHeightfieldBuffer->getName() + _T("Original") + Ogre::StringConverter::toString((long)this);
	mOriginalHeightfieldBuffer = bufferSet->get(bufferSet->createFrom(originalName, mInOutHeightfieldBuffer->getHandle(), true));
}

// ----------------------------------------------------------------------------
HeightfieldBuffer* HeightfieldOperationBrushBase::getTempHeightfieldBuffer()
{
	if (!mTempHeightfieldBuffer)
	{
		assert(mInOutHeightfieldBuffer);
		HeightfieldBufferSet* bufferSet = mInOutHeightfieldBuffer->getHeightfieldBufferSet();
		string tempName = mInOutHeightfieldBuffer->getName() + _T("Temp") + Ogre::StringConverter::toString((long)this);
		mTempHeightfieldBuffer = bufferSet->get(bufferSet->createFrom(tempName, mInOutHeightfieldBuffer->getHandle(), true));
	}
	return mTempHeightfieldBuffer;
}



// ----------------------------------------------------------------------------
void HeightfieldOperationBrushBase::tickDo()
{
	assert(mState == STATE_DO && mIsStateActive);

	if (mBrush->isActive() && !mHeightfieldGeom)
	{
		mHeightfieldGeom = mBrush->getHeightfieldGeom();
	}

	// Calculate new brush instance data from any new brush activity
	if (mHeightfieldGeom)
	{
		brushInstancerTick();

		schedulerTick();
	}

	//printf("Base calculation frame time:\n");

	if (mHeightfieldGeom && 
		mBrush->getHeightfieldGeom() && 
		!mBrush->isActive() && 
		getBrushInstanceCount() == 0 &&
		isSchedulerDone())
	{
		finishDo();
		mIsStateActive = false;
		//printf("Finished HeightfieldOperationBrushBase %p\n", this);
		return;
	}
}



// ----------------------------------------------------------------------------
void HeightfieldOperationBrushBase::finishDo()
{
	assert(mState == STATE_DO && mIsStateActive);
	assert(mInOutHeightfieldBuffer);
	mDeltaBufferPtr = HeightfieldSerializer::serializeIncremental(mOriginalHeightfieldBuffer, mInOutHeightfieldBuffer);

	if (mOriginalHeightfieldBuffer)
	{
		mOriginalHeightfieldBuffer->getHeightfieldBufferSet()->destroy(mOriginalHeightfieldBuffer->getHandle());
		mOriginalHeightfieldBuffer = NULL;
	}

	if (mTempHeightfieldBuffer)
	{
		mTempHeightfieldBuffer->getHeightfieldBufferSet()->destroy(mTempHeightfieldBuffer->getHandle());
		mTempHeightfieldBuffer = NULL;
	}
}


// ----------------------------------------------------------------------------
void HeightfieldOperationBrushBase::initiateUndo()
{
	assert(!mIsStateActive && canInitiate(STATE_UNDO));
	mState = STATE_UNDO;
	mIsStateActive = true;
	mHasBeenStateActive = true;
}


// ----------------------------------------------------------------------------
void HeightfieldOperationBrushBase::tickUndo()
{
	assert(mInOutHeightfieldBuffer);
	HeightfieldSerializer::deserializeIncrementalPrevious(mDeltaBufferPtr, mInOutHeightfieldBuffer);

	mIsStateActive = false;
}


// ----------------------------------------------------------------------------
void HeightfieldOperationBrushBase::initiateRedo()
{
	assert(!mIsStateActive && canInitiate(STATE_REDO));
	mState = STATE_REDO;
	mIsStateActive = true;
	mHasBeenStateActive = true;
}

// ----------------------------------------------------------------------------
void HeightfieldOperationBrushBase::tickRedo()
{
	assert(mInOutHeightfieldBuffer);
	HeightfieldSerializer::deserializeIncrementalNext(mDeltaBufferPtr, mInOutHeightfieldBuffer);

	mIsStateActive = false;
}


// ----------------------------------------------------------------------------
void HeightfieldOperationBrushBase::tickInactive()
{
}

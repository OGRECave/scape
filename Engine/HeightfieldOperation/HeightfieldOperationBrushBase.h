/**
 * ScapeEngine::HeightfieldOperation class
 *
 * An abstract super class for an HeightfieldPage operation that is designed to combine policies for its
 * behavior
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONBASE_H__
#define __HEIGHTFIELDOPERATIONBASE_H__

#include "ScapeEnginePrerequisites.h"

#include "HeightfieldOperationBrush.h"
#include "HeightfieldOperationPolicySchedulerTaskDirect.h"
#include "HeightfieldOperationPolicySchedulerTaskPageBatch.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine
{

class HeightfieldOperationBrushBase : public HeightfieldOperationBrush,
                                      public HeightfieldOperationPolicyScheduler,
                                      public HeightfieldOperationPolicyBrushInstancer
{
public:
    virtual ~HeightfieldOperationBrushBase();

    virtual void tick();

    virtual bool canInitiate(EState state);
    virtual bool isActive();
    virtual bool isActive(EState state);
    virtual bool isDone(EState state);
    virtual void initiate(EState state, HeightfieldBuffer* inOutHeightfieldBuffer);

    virtual HeightfieldBuffer* getInOutHeightfieldBuffer() { return mInOutHeightfieldBuffer; }
    virtual HeightfieldBuffer* getTempHeightfieldBuffer();

    bool isPencilMode() { return mPencilMode; }

protected:
    EState mState;
    bool mIsStateActive;
    bool mHasBeenStateActive;
    bool mWasActiveStroke;
    bool mIsActiveStroke;

    bool mPencilMode;
    Ogre::Real mPathSpacing;

    class HeightfieldBuffer* mOriginalHeightfieldBuffer;
    class HeightfieldBuffer* mInOutHeightfieldBuffer;
    class HeightfieldBuffer* mTempHeightfieldBuffer;

    Utils::SerialMemoryBufferPtr mDeltaBufferPtr;

    string mBrushName;
    HeightfieldGeom* mHeightfieldGeom;

    HeightfieldOperationBrushBase();

    // Ogre::Real mStrength;

    unsigned long mLastTickMillis;

    virtual void initiateDo();
    virtual void initiateUndo();
    virtual void initiateRedo();
    virtual void tickDo();
    virtual void tickUndo();
    virtual void tickRedo();
    virtual void tickInactive();
    virtual void finishDo();
};
}

#endif // __HEIGHTFIELDOPERATIONBASE_H__

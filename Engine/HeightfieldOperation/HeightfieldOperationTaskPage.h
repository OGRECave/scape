/**
 * ScapeEngine::HeightfieldOperationTaskPage class
 *
 * The HeightfieldBufferPage processor class, controlled by HeightfieldOperation
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONPAGE_H__
#define __HEIGHTFIELDOPERATIONPAGE_H__

#include "ScapeEnginePrerequisites.h"

#include "HeightfieldOperationBrush.h"

namespace ScapeEngine
{

class HeightfieldOperationTaskPage
{
public:
    HeightfieldOperationTaskPage(HeightfieldOperationBrush* operation, const BrushPageCoords pageCoords,
                                 const BrushInstances& brushInstances);
    virtual ~HeightfieldOperationTaskPage();

    inline const BrushPageCoords getPageCoords() { return mPageCoords; }

    bool addBrushInstances(const BrushInstances& brushInstances);

    virtual void tick();

    bool isActive() { return mCurrentState == STATE_ACTIVE; }
    bool isPending() { return mCurrentState == STATE_PENDING; }
    bool isCompleted() { return mCurrentState == STATE_COMPLETED; }

    const Ogre::Rect& getUpdatedRect() { return mUpdatedRect; }

protected:
    enum State
    {
        STATE_PENDING,
        STATE_ACTIVE,
        STATE_COMPLETED,
    } mCurrentState;

    Ogre::Rect mUpdatedRect;

    BrushInstances mBrushInstances;
    BrushPageCoords mPageCoords;
    HeightfieldOperationBrush* mOperation;

    virtual void tickPending() = 0;
    virtual void tickActive() = 0;
};
}

#endif // __HEIGHTFIELDOPERATIONPAGE_H__

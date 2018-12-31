/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldOperationStack.h"
#include "HeightfieldOperationFactory.h"
#include "HeightfieldOperation.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldGeom/HeightfieldGeomManager.h"
#include "Input/InputManager.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldOperationStack::HeightfieldOperationStack() : mLastOperationIndex(-1), mNewOperation(NULL)
{
    wantsPreFrameTick();
    getLastOperation();
}

// ----------------------------------------------------------------------------
HeightfieldOperationStack::~HeightfieldOperationStack() { clearAll(); }

// ----------------------------------------------------------------------------
HeightfieldOperation* HeightfieldOperationStack::getLastOperation()
{
    return getOperation(mLastOperationIndex);
}

// ----------------------------------------------------------------------------
bool HeightfieldOperationStack::setNewOperationClassName(const string& className)
{
    if (getEngineCore()->getHeightfieldOperationFactory()->isRegistered(className))
    {
        if (mNewOperationClassName != className)
        {
            mNewOperationClassName = className;
            SAFE_DELETE(mNewOperation);
        }
        return true;
    }
    else
    {
        mNewOperationClassName = Utils::emptyString;
        SAFE_DELETE(mNewOperation);
        return false;
    }
}

// ----------------------------------------------------------------------------
HeightfieldOperation* HeightfieldOperationStack::getNewOperation()
{
    if (!mNewOperation && mNewOperationClassName != Utils::emptyString)
    {
        mNewOperation = getEngineCore()->getHeightfieldOperationFactory()->create(mNewOperationClassName);
    }
    return mNewOperation;
}

// ----------------------------------------------------------------------------
HeightfieldOperation* HeightfieldOperationStack::getOperation(size_t index)
{
    if (index >= 0 && index < mOperationHistory.size())
    {
        return mOperationHistory[index];
    }
    return NULL;
}

// ----------------------------------------------------------------------------
bool HeightfieldOperationStack::doOperation()
{
    if (mNewOperation && mNewOperation->canInitiate(HeightfieldOperation::STATE_DO))
    {
        HeightfieldGeom* geom = getEngineCore()->getHeightfieldGeomManager()->getCurrentHeightfieldGeom();
        assert(geom && geom->getHeightfieldBuffer() && _T("HeightfieldOperationStack::getNewOperation() : ")
                                                       _T("Needs a current heighttfieldbuffer to bind an ")
                                                       _T("operation to"));

        ++mLastOperationIndex;
        for (size_t index = mLastOperationIndex; index < mOperationHistory.size(); ++index)
        {
            delete getOperation(index);
        }
        mOperationHistory.resize(mLastOperationIndex + 1);
        mOperationHistory[mLastOperationIndex] = mNewOperation;
        mNewOperation = NULL;

        mOperationHistory[mLastOperationIndex]->initiate(HeightfieldOperation::STATE_DO,
                                                         geom->getHeightfieldBuffer());
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------
bool HeightfieldOperationStack::undoOperation()
{
    for (int newIndex = mLastOperationIndex; newIndex >= 0; --newIndex)
    {
        if (getOperation(newIndex)->canInitiate(HeightfieldOperation::STATE_UNDO))
        {
            HeightfieldGeom* geom =
                getEngineCore()->getHeightfieldGeomManager()->getCurrentHeightfieldGeom();
            assert(geom && geom->getHeightfieldBuffer() && _T("HeightfieldOperationStack::getNewOperation(")
                                                           _T(") : Needs a current heighttfieldbuffer to ")
                                                           _T("bind an operation to"));

            getOperation(newIndex)->initiate(HeightfieldOperation::STATE_UNDO,
                                             geom->getHeightfieldBuffer());
            mLastOperationIndex = newIndex;
            return true;
        }
    }
    return false;
}

// ----------------------------------------------------------------------------
bool HeightfieldOperationStack::redoOperation()
{
    if (getLastOperation())
    {
        if (getLastOperation()->canInitiate(HeightfieldOperation::STATE_REDO))
        {
            HeightfieldGeom* geom =
                getEngineCore()->getHeightfieldGeomManager()->getCurrentHeightfieldGeom();
            assert(geom && geom->getHeightfieldBuffer() && _T("HeightfieldOperationStack::getNewOperation(")
                                                           _T(") : Needs a current heighttfieldbuffer to ")
                                                           _T("bind an operation to"));

            getLastOperation()->initiate(HeightfieldOperation::STATE_REDO, geom->getHeightfieldBuffer());
            return true;
        }
    }
    return false;
}

// ----------------------------------------------------------------------------
void HeightfieldOperationStack::onPreFrameTick()
{
    bool active = false;
    if (getLastOperation())
    {
        getLastOperation()->tick();
        if (getLastOperation()->isActive())
        {
            active = true;
        }
    }
    if (!active && getNewOperation() && getLastOperation() != getNewOperation())
    {
        getNewOperation()->tick();
        if (getNewOperation()->isActive())
        {
            active = true;
        }
    }

    if (!active)
    {
        while (getLastOperation() && mLastOperationIndex + 1 < (int)mOperationHistory.size() &&
               (getLastOperation()->isDone(HeightfieldOperation::STATE_DO) ||
                getLastOperation()->isDone(HeightfieldOperation::STATE_REDO)))
        {
            ++mLastOperationIndex;
        }

        if (!active && getEngineCore()->getInputManager()->getButton(ButtonId::UNDO)->isJustPressed())
        {
            if (undoOperation())
            {
                active = true;
                printf("undo operation");
            }
        }

        if (!active && getEngineCore()->getInputManager()->getButton(ButtonId::REDO)->isJustPressed())
        {
            if (redoOperation())
            {
                active = true;
                printf("redo operation");
            }
        }

        if (!active && getNewOperation() && getNewOperation()->canInitiate(HeightfieldOperation::STATE_DO))
        {
            if (doOperation())
            {
                active = true;
                printf("do operation");
            }
        }
    }
}

// ----------------------------------------------------------------------------
void HeightfieldOperationStack::clearAll()
{
    for (size_t index = 0; index < mOperationHistory.size(); ++index)
    {
        delete getOperation(index);
    }
    if (getLastOperation() != mNewOperation)
    {
        SAFE_DELETE(mNewOperation);
    }
    mOperationHistory.clear();
    mLastOperationIndex = -1;
}
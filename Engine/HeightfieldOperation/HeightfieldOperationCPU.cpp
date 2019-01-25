/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
  */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldOperationCPU.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"
#include "Input/InputManager.h"

#include "HeightfieldSerializer/HeightfieldSerializerIncremental.h"

using namespace ScapeEngine;

HeightfieldOperationCPU::HeightfieldOperationCPU()
    : HeightfieldOperationBrush(), mOriginalHeightfieldBuffer(NULL), mInOutHeightfieldBuffer(NULL),
      mIsStateActive(false), mHasBeenStateActive(false), mBrushName(_T("HeightfieldOperationCPU")),
      mHeightfieldGeom(NULL), mPathSpacing(0.0f), mPencilMode(false)
{
    mDeltaBufferPtr.reset();
}

HeightfieldOperationCPU::~HeightfieldOperationCPU()
{
    if (mOriginalHeightfieldBuffer)
    {
        mOriginalHeightfieldBuffer->getHeightfieldBufferSet()->destroy(
            mOriginalHeightfieldBuffer->getHandle());
        mOriginalHeightfieldBuffer = NULL;
    }

    mDeltaBufferPtr.reset();
}

bool HeightfieldOperationCPU::canInitiate(EState state)
{
    switch (state)
    {
    case STATE_DO:
        return mBrush && mBrush->isActive() && mBrush->getHeightfieldGeom() &&
               (!mHasBeenStateActive || mState == STATE_UNDO);

    case STATE_UNDO:
        return mHasBeenStateActive && (mState == STATE_DO || mState == STATE_REDO);

    case STATE_REDO:
        return mHasBeenStateActive && mState == STATE_UNDO;

    default:
        return false;
    }
}

bool HeightfieldOperationCPU::isActive(EState state) { return mIsStateActive && mState == state; }

bool HeightfieldOperationCPU::isActive() { return mIsStateActive; }

bool HeightfieldOperationCPU::isDone(EState state)
{
    return !mIsStateActive && mHasBeenStateActive && mState == state;
}

void HeightfieldOperationCPU::initiate(EState state, HeightfieldBuffer* inOutHeightfieldBuffer)
{
    mInOutHeightfieldBuffer = inOutHeightfieldBuffer;

    switch (state)
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

void HeightfieldOperationCPU::tick()
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
}

void HeightfieldOperationCPU::initiateDo()
{
    assert(!mIsStateActive && canInitiate(STATE_DO));
    mHeightfieldGeom = NULL;
    mState = STATE_DO;
    mIsStateActive = true;
    mHasBeenStateActive = true;

    if (mOriginalHeightfieldBuffer)
    {
        mOriginalHeightfieldBuffer->getHeightfieldBufferSet()->destroy(
            mOriginalHeightfieldBuffer->getHandle());
        mOriginalHeightfieldBuffer = NULL;
    }

    assert(mBrush->getHeightfieldGeom());
    assert(mInOutHeightfieldBuffer);
    mInOutHeightfieldBuffer = mBrush->getHeightfieldGeom()->getHeightfieldBuffer();

    HeightfieldBufferSet* bufferSet = mInOutHeightfieldBuffer->getHeightfieldBufferSet();
    string originalName =
        mInOutHeightfieldBuffer->getName() + _T("Original") + Ogre::StringConverter::toString((long)this);
    mOriginalHeightfieldBuffer =
        bufferSet->get(bufferSet->createFrom(originalName, mInOutHeightfieldBuffer->getHandle(), true));
}

void HeightfieldOperationCPU::tickDo()
{
    assert(mState == STATE_DO && mIsStateActive);

    if (mBrush->isActive() && !mHeightfieldGeom)
    {
        mHeightfieldGeom = mBrush->getHeightfieldGeom();
        mPathNextParam = 0.0f;
        mBrushPathNodes.clear();
    }

    if (mPencilMode)
    {
        if (mHeightfieldGeom && mHeightfieldGeom == mBrush->getHeightfieldGeom())
        {
            if (mBrush->isActive())
            {
                BrushPathNode node;
                node.position =
                    mBrush->getHeightfieldGeom()->getLocalPosition(mHeightfieldBrushState.getPosition());
                node.pressure = mHeightfieldBrushState.getPressure();
                node.time = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
                node.primary = mBrush->isPrimaryActive();
                mBrushPathNodes.push_back(node);
            }

            unsigned long millisStart = Ogre::Root::getSingleton().getTimer()->getMilliseconds();

            while (mBrushPathNodes.size() >= 4)
            {
                BrushPathNodes::iterator nodesIt = mBrushPathNodes.begin();

                BrushPathNode& node1 = *nodesIt++;
                BrushPathNode& node2 = *nodesIt++;
                BrushPathNode& node3 = *nodesIt++;
                BrushPathNode& node4 = *nodesIt++;

                Ogre::Matrix4 splineVectors(node1.position.x, node2.position.x, node3.position.x,
                                            node4.position.x, node1.position.y, node2.position.y,
                                            node3.position.y, node4.position.y, node1.position.z,
                                            node2.position.z, node3.position.z, node4.position.z,
                                            node1.pressure, node2.pressure, node3.pressure, node4.pressure);

                Ogre::Vector3 subPoint1 = node2.position;
                Ogre::Vector3 subPoint2 =
                    Ogre::Vector3((splineVectors * Utils::getCatmullRomeSplineWeights(0.25f)).ptr());
                Ogre::Vector3 subPoint3 =
                    Ogre::Vector3((splineVectors * Utils::getCatmullRomeSplineWeights(0.5f)).ptr());
                Ogre::Vector3 subPoint4 =
                    Ogre::Vector3((splineVectors * Utils::getCatmullRomeSplineWeights(0.75f)).ptr());
                Ogre::Vector3 subPoint5 = node3.position;

                Ogre::Real distance =
                    std::max(1.0f, subPoint1.distance(subPoint2) + subPoint2.distance(subPoint3) +
                                       subPoint3.distance(subPoint4) + subPoint4.distance(subPoint5));

                distance = std::max(0.1f, node2.position.distance(node3.position));
                Ogre::Real stepSize =
                    std::max(1.0f, mPathSpacing * (mHeightfieldBrushSettings.getOuterRadius() -
                                                   mHeightfieldBrushSettings.getInnerRadius()));
                Ogre::Real stepStrength = stepSize * std::max(0.1f, 0.001f * (node3.time - node2.time));

                Ogre::Vector3 positionCurrent;
                Ogre::Real strengthCurrent;
                if (mPathNextParam < distance)
                {
                    Ogre::Vector4 weights = Utils::getCatmullRomeSplineWeights(mPathNextParam / distance);
                    Ogre::Vector4 interpolation = splineVectors * weights;
                    positionCurrent = Ogre::Vector3(interpolation.ptr());
                    strengthCurrent = stepStrength * interpolation.w * interpolation.w;

                    if (node2.primary)
                    {
                        applyPrimary(positionCurrent, strengthCurrent);
                    }
                    else
                    {
                        applySecondary(positionCurrent, strengthCurrent);
                    }
                    mPathNextParam += stepSize;

                    if (Ogre::Root::getSingleton().getTimer()->getMilliseconds() - millisStart > 100)
                    {
                        break;
                    }
                }
                else
                {
                    mPathNextParam -= distance;
                    mBrushPathNodes.pop_front();
                }
            }
        }
    }
    else if (mBrush->isActive() && mBrush->getHeightfieldGeom())
    {
        mBrushPathNodes.clear();
        Ogre::Vector3 brushPosition =
            mBrush->getHeightfieldGeom()->getLocalPosition(mHeightfieldBrushState.getPosition());
        Ogre::Real pressure = mHeightfieldBrushState.getPressure();

        if (mBrush->isPrimaryActive())
        {
            applyPrimary(brushPosition, pressure * pressure);
        }
        else
        {
            applySecondary(brushPosition, pressure * pressure);
        }
    }

    if (getEngineCore()->getInputManager()->getButton(ButtonId::OPERATION_CANCEL)->isJustPressed())
    {
        mBrushPathNodes.clear();
    }

    if (mHeightfieldGeom && mBrushPathNodes.size() < 4 && !mBrush->isActive() &&
        mBrush->getHeightfieldGeom())
    {
        finishDo();
        mIsStateActive = false;
        return;
    }
}

void HeightfieldOperationCPU::finishDo()
{
    assert(mState == STATE_DO && mIsStateActive);
    assert(mInOutHeightfieldBuffer);
    mDeltaBufferPtr =
        HeightfieldSerializer::serializeIncremental(mOriginalHeightfieldBuffer, mInOutHeightfieldBuffer);

    if (mOriginalHeightfieldBuffer)
    {
        mOriginalHeightfieldBuffer->getHeightfieldBufferSet()->destroy(
            mOriginalHeightfieldBuffer->getHandle());
        mOriginalHeightfieldBuffer = NULL;
    }
}

void HeightfieldOperationCPU::initiateUndo()
{
    assert(!mIsStateActive && canInitiate(STATE_UNDO));
    mState = STATE_UNDO;
    mIsStateActive = true;
    mHasBeenStateActive = true;
}

void HeightfieldOperationCPU::tickUndo()
{
    assert(mInOutHeightfieldBuffer);
    HeightfieldSerializer::deserializeIncrementalPrevious(mDeltaBufferPtr, mInOutHeightfieldBuffer);

    mIsStateActive = false;
}

void HeightfieldOperationCPU::initiateRedo()
{
    assert(!mIsStateActive && canInitiate(STATE_REDO));
    mState = STATE_REDO;
    mIsStateActive = true;
    mHasBeenStateActive = true;
}

void HeightfieldOperationCPU::tickRedo()
{
    assert(mInOutHeightfieldBuffer);
    HeightfieldSerializer::deserializeIncrementalNext(mDeltaBufferPtr, mInOutHeightfieldBuffer);

    mIsStateActive = false;
}

void HeightfieldOperationCPU::tickInactive()
{
    // if (mBrush->isActive() && canInitiate(STATE_DO))
    //{
    // initiate(STATE_DO);
    //}
}

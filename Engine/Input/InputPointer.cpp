/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "InputPointer.h"
#include "InputManager.h"
#include "AnalogInputId.h"
#include "EngineCore/RenderViewManager.h"
#include "EngineCore/RenderView.h"
#include "EngineCore/CameraController.h"
#include "HeightfieldGeom/HeightfieldGeomManager.h"

using namespace ScapeEngine;

InputPointer::InputPointer(InputManager* inputManager)
    : mInputManager(inputManager), mCurrentState(0, 0, -1.0f)
{
}

std::pair<RenderView*, Ogre::Vector2> InputPointer::PointerState::getRenderView2DPoint() const
{
    if (mIsFocusedRenderViewDirty)
    {
        mFocusedRenderView = getEngineCore()->getInputManager()->getFocusedRenderView();

        mFocusedRenderView2DPoint = Ogre::Vector2(0.0f, 0.0f);
        if (mFocusedRenderView)
        {
            mFocusedRenderView->fromRootToNormalizedLocalSpace(mWindow2DPoint, mFocusedRenderView2DPoint);
        }

        mIsFocusedRenderViewDirty = false;
    }

    return std::pair<RenderView*, Ogre::Vector2>(mFocusedRenderView, mFocusedRenderView2DPoint);
}

const Ogre::Vector3& InputPointer::PointerState::get3DPoint() const
{
    getRenderView2DPoint();

    if (mIs3DPointDirty)
    {
        if (mFocusedRenderView)
        {
            Ogre::Vector3 cameraPosition = mFocusedRenderView->getCameraSceneNode()->getPosition();
            std::pair<RenderView*, Ogre::Vector2> renderView2DPoint = getRenderView2DPoint();
            if (renderView2DPoint.first)
            {
                Ogre::Vector3 pointerDirection =
                    renderView2DPoint.first->getCameraController()
                        ->getViewportPickRayDirection(
                            Ogre::Vector3(renderView2DPoint.second.x, renderView2DPoint.second.y, 0))
                        .toOgreVector3();
                Ogre::Ray intersectionRay(cameraPosition, pointerDirection);
                m3DPoint = cameraPosition + 1000 * pointerDirection;
                getEngineCore()->getHeightfieldGeomManager()->getClosestHeightfieldGeomRayIntersection(
                    intersectionRay, *mFocusedRenderView->getCamera()->getSceneManager(), m3DPointGeom,
                    m3DPoint);
            }
        }
        else
        {
            m3DPoint = Ogre::Vector3(-1.E6f);
        }

        mIs3DPointDirty = false;
    }

    return m3DPoint;
}

HeightfieldGeom* InputPointer::PointerState::get3DPointGeom() const
{
    get3DPoint();
    return m3DPointGeom;
}

void InputPointer::startNewFrame() {}

void InputPointer::update()
{
    Ogre::Real pressure = 0.001f * mInputManager->getAnalogInput(AnalogInputId::POINTER_PRESSURE);
    if (pressure < 0.0f)
    {
        pressure = -1.0f;
    }

    mCurrentState = PointerState(mInputManager->getAnalogInput(AnalogInputId::POINTER_X),
                                 mInputManager->getAnalogInput(AnalogInputId::POINTER_Y), pressure);

    mFrameStateTrailHistory.push_back(mCurrentState);
    if (mFrameStateTrailHistory.size() > 100)
    {
        mFrameStateTrailHistory.pop_front();
    }
}

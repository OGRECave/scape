/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "CameraController.h"
#include "Input/InputManager.h"
#include "Input/InputPointer.h"
#include "RenderView.h"
#include "HeightfieldGeom/HeightfieldGeom.h" //TODO GdCarpentier 11/6/2007 : remove

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
CameraController::CameraController()
    : mRenderView(NULL), mStartGrabRayIntersectionPosition(0.0, 0.0, 0.0), mCurrent2DGrabPosition(0.0, 0.0),
      mCameraTransform(), mCameraEulerAngles(0.0, 0.0, 0.0), mPointerGrabActive(false)
{
    wantsFrameTick();
}

// ----------------------------------------------------------------------------
CameraController::~CameraController() {}

// ----------------------------------------------------------------------------
void CameraController::attach(RenderView* renderView)
{
    mRenderView = renderView;

    Ogre::Vector3 cameraPosition = mRenderView->getCameraSceneNode()->getPosition();
    Ogre::Quaternion cameraOrientation = mRenderView->getCameraSceneNode()->getOrientation();

    mCameraEulerAngles =
        DVector3(cameraOrientation.getPitch(true).valueRadians(), cameraOrientation.getYaw().valueRadians(),
                 cameraOrientation.getRoll().valueRadians());
    mCameraTransform.fromEulerAngles(mCameraEulerAngles, cameraPosition);

    mPointerGrabActive = false;
}

// ----------------------------------------------------------------------------
void CameraController::detach() { mRenderView = NULL; }

// ----------------------------------------------------------------------------
DVector3 CameraController::getViewportPickRayDirection(const DVector3& screenPosition)
{
    DMatrix4 invProjMatrix(mRenderView->getCamera()->getProjectionMatrix());
    invProjMatrix.inverse();

    DMatrix4 invCombinedMatrix = (mCameraTransform * invProjMatrix);

    double normScreenX = (2.0f * screenPosition.x) - 1.0f;
    double normScreenY = 1.0f - (2.0f * screenPosition.y);

    DVector3 nearPoint(normScreenX, normScreenY, -1.0);
    DVector3 midPoint(normScreenX, normScreenY, 0.0);

    nearPoint = invCombinedMatrix * nearPoint;
    midPoint = invCombinedMatrix * midPoint;

    DVector3 direction = (midPoint - nearPoint);
    direction.normalise();
    return direction;
}

// ----------------------------------------------------------------------------
void CameraController::onFrameTick()
{
    InputManager* inputManager = getEngineCore()->getInputManager();

    bool camPanTiltButtonPressed = inputManager->getButton(ButtonId::CAMERA_POINTERCAMPANTILT)->isPressed();
    bool grabTruckButtonPressed = inputManager->getButton(ButtonId::CAMERA_POINTERGRABTRUCK)->isPressed();
    bool grabPanTiltButtonPressed =
        inputManager->getButton(ButtonId::CAMERA_POINTERGRABPANTILT)->isPressed();
    bool grabDollyButtonPressed = inputManager->getButton(ButtonId::CAMERA_POINTERGRABDOLLY)->isPressed();

    bool cameraInputActive;
    cameraInputActive = camPanTiltButtonPressed || grabTruckButtonPressed || grabPanTiltButtonPressed ||
                        grabDollyButtonPressed;
    cameraInputActive = cameraInputActive || inputManager->getButton(ButtonId::CAMERA_KEYLEFT)->isPressed();
    cameraInputActive =
        cameraInputActive || inputManager->getButton(ButtonId::CAMERA_KEYRIGHT)->isPressed();
    cameraInputActive = cameraInputActive || inputManager->getButton(ButtonId::CAMERA_KEYUP)->isPressed();
    cameraInputActive = cameraInputActive || inputManager->getButton(ButtonId::CAMERA_KEYDOWN)->isPressed();
    cameraInputActive =
        cameraInputActive || inputManager->getButton(ButtonId::CAMERA_KEYFORWARD)->isPressed();
    cameraInputActive =
        cameraInputActive || inputManager->getButton(ButtonId::CAMERA_KEYBACKWARD)->isPressed();
    cameraInputActive =
        cameraInputActive || inputManager->getAnalogInput(AnalogInputId::POINTER_MOVEZ) != 0;

    bool pointerGrabActive = grabTruckButtonPressed || grabPanTiltButtonPressed || grabDollyButtonPressed;
    bool justPointerGrabActive = pointerGrabActive && !mPointerGrabActive;
    mPointerGrabActive = pointerGrabActive;

    std::pair<RenderView*, Ogre::Vector2> focusedRenderView2DPoint =
        inputManager->getInputPointer()->getCurrentState().getRenderView2DPoint();
    if (focusedRenderView2DPoint.first == mRenderView)
    {
        Ogre::Vector3 point = inputManager->getInputPointer()->getCurrentState().get3DPoint();

        if (cameraInputActive)
        {
            if (justPointerGrabActive)
            {
                mCurrent2DGrabPosition =
                    DVector2(focusedRenderView2DPoint.second.x, focusedRenderView2DPoint.second.y);
                mStartGrabRayIntersectionPosition = point;
            }

            if (camPanTiltButtonPressed || grabPanTiltButtonPressed)
                doPanTilt();

            // Start with trucking when the 2d mouse grab is used
            if (pointerGrabActive)
                doGrabTruck(grabTruckButtonPressed && !grabDollyButtonPressed);

            // Update using standard mouse and keyboard truck, dolly, pan and tilt
            updateTruck();
            updateDolly();

            // Do pointer-based dollying
            if (grabDollyButtonPressed)
                doGrabDolly();

            // As the non-grabbing camera updates might have introduced an grab error, start
            // correcting the camera rotation so the grabbed point is aligned again.
            if (grabPanTiltButtonPressed || grabDollyButtonPressed)
                doGrabPanTilt();

            // As the tilt angle is bound, correct any remaining grab error using camera translation
            if (justPointerGrabActive)
                doGrabTruck(false);

            // Update the ogre camera's position and orientation
        }
    }

    Ogre::SceneNode* cameraSceneNode = mRenderView->getCameraSceneNode();
    cameraSceneNode->setOrientation(Ogre::Quaternion(mCameraTransform.toOgreMatrix4().linear()));
    cameraSceneNode->setPosition(mCameraTransform.col3.asVector3().toOgreVector3());
}

// ----------------------------------------------------------------------------
void CameraController::doGrabTruck(bool update2DGrabPoint)
{
    InputManager* inputManager = getEngineCore()->getInputManager();
    const DVector3& cameraDirection = -mCameraTransform.col2.asVector3();
    const DVector3& cameraPosition = mCameraTransform.col3.asVector3();

    if (update2DGrabPoint)
    {
        Ogre::Vector2 neutralPosition(0.0, 0.0);
        mRenderView->fromRootToNormalizedLocalSpace(IVector2::ZERO, neutralPosition);

        IVector2 deltaUnits(inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAX),
                            inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY));

        Ogre::Vector2 pointerMovementPosition;
        mRenderView->fromRootToNormalizedLocalSpace(deltaUnits, pointerMovementPosition);

        mCurrent2DGrabPosition = mCurrent2DGrabPosition + (pointerMovementPosition - neutralPosition);
    }

    DVector3 pointerDirection =
        getViewportPickRayDirection(DVector3(mCurrent2DGrabPosition.x, mCurrent2DGrabPosition.y, 0));

    // calculate intersection point between mouse pick ray and the plane that is perpendicular
    // to the camera's direction and intersects the grabbed 3d point. Then, the grab error is
    // the difference between this ray-plane intersection point and the initially grabbed point.
    DVector3 deltaGrab = (cameraPosition - mStartGrabRayIntersectionPosition);
    double dotNom = cameraDirection.dot(deltaGrab);
    double dotDenom = -cameraDirection.dot(pointerDirection);
    DVector3 intersection = cameraPosition + pointerDirection * (dotNom / dotDenom);
    DVector3 error = intersection - mStartGrabRayIntersectionPosition;

    mCameraTransform.col3.asVector3() = cameraPosition - error;

    /* //TODO GdCarpentier 7/16/2007 : remove
            static Ogre::SceneNode* node = NULL;
            if (node == NULL)
            {
                    node =
       getEngineCore()->getSceneManager()->getRootSceneNode()->createChildSceneNode(_T("node"));
                    Ogre::Entity* entity = getSceneManager()->createEntity(_T("sphere"), _T("sphere.mesh"));
                    node->attachObject(entity);
            }

            node->setPosition(pointerIntersectionPoint.toOgreVector3());
            float scale = 0.0001f * (float)(pointerIntersectionPoint - cameraPosition).length();
            node->setScale(scale,scale,scale);
    */
}

// ----------------------------------------------------------------------------
void CameraController::doGrabPanTilt()
{
    const DVector3& cameraPosition = mCameraTransform.col3.asVector3();
    DVector3 grabDirection = (mStartGrabRayIntersectionPosition - cameraPosition);
    DVector3 pointerDirection =
        getViewportPickRayDirection(DVector3(mCurrent2DGrabPosition.x, mCurrent2DGrabPosition.y, 0));

    DVector3 correctionEulerAngles =
        Utils::getYawPitchFromVectorDifference(pointerDirection, grabDirection);
    mCameraEulerAngles.y += correctionEulerAngles.y;
    mCameraEulerAngles.x += correctionEulerAngles.x;

    mCameraEulerAngles = normalizeEulerAngles(mCameraEulerAngles);
    mCameraTransform.fromEulerAngles(mCameraEulerAngles, cameraPosition);
}

// ----------------------------------------------------------------------------
void CameraController::doGrabDolly()
{
    InputManager* inputManager = getEngineCore()->getInputManager();

    float scale = 0.005f;
    float ratio = exp(scale * inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY));
    mCameraTransform.col3.asVector3() =
        mStartGrabRayIntersectionPosition +
        (mCameraTransform.col3.asVector3() - mStartGrabRayIntersectionPosition) * ratio;
}

// ----------------------------------------------------------------------------
void CameraController::doPanTilt()
{
    InputManager* inputManager = getEngineCore()->getInputManager();
    const DVector3& cameraPosition = mCameraTransform.col3.asVector3();

    double scale = 0.0025f;

    mCameraEulerAngles.y += -scale * inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAX);
    mCameraEulerAngles.x += -scale * inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY);

    mCameraEulerAngles = normalizeEulerAngles(mCameraEulerAngles);
    mCameraTransform.fromEulerAngles(mCameraEulerAngles, cameraPosition);
}

// ----------------------------------------------------------------------------
void CameraController::updateTruck()
{
    InputManager* inputManager = getEngineCore()->getInputManager();
    float getTimeSinceLastFrame = getEngineCore()->getTimeSinceLastFrame();

    float moveSpeed = 500.0f; // FIXME: store in a setting (was 200.0f)

    if (inputManager->getButton(ButtonId::CAMERA_KEYLEFT)->isPressed(false))
    {
        mCameraTransform.col3 =
            mCameraTransform.col3 - moveSpeed * mCameraTransform.col0 * getTimeSinceLastFrame;
    }

    if (inputManager->getButton(ButtonId::CAMERA_KEYRIGHT)->isPressed(false))
    {
        mCameraTransform.col3 =
            mCameraTransform.col3 + moveSpeed * mCameraTransform.col0 * getTimeSinceLastFrame;
    }

    if (inputManager->getButton(ButtonId::CAMERA_KEYUP)->isPressed(false))
    {
        mCameraTransform.col3 =
            mCameraTransform.col3 + moveSpeed * mCameraTransform.col1 * getTimeSinceLastFrame;
    }

    if (inputManager->getButton(ButtonId::CAMERA_KEYDOWN)->isPressed(false))
    {
        mCameraTransform.col3 =
            mCameraTransform.col3 - moveSpeed * mCameraTransform.col1 * getTimeSinceLastFrame;
    }
}

// ----------------------------------------------------------------------------
void CameraController::updateDolly()
{
    InputManager* inputManager = getEngineCore()->getInputManager();
    float getTimeSinceLastFrame = getEngineCore()->getTimeSinceLastFrame();

    float moveSpeed = 500.0f; // FIXME: store in a setting (was 200.0f)

    if (inputManager->getButton(ButtonId::CAMERA_KEYFORWARD)->isPressed(false))
    {
        mCameraTransform.col3 =
            mCameraTransform.col3 - moveSpeed * mCameraTransform.col2 * getTimeSinceLastFrame;
    }

    if (inputManager->getButton(ButtonId::CAMERA_KEYBACKWARD)->isPressed(false))
    {
        mCameraTransform.col3 =
            mCameraTransform.col3 + moveSpeed * mCameraTransform.col2 * getTimeSinceLastFrame;
    }

    if (inputManager->getAnalogInput(AnalogInputId::POINTER_MOVEZ) != 0)
    {
        float move = -0.003f * moveSpeed * inputManager->getAnalogInput(AnalogInputId::POINTER_MOVEZ);
        mCameraTransform.col3 = mCameraTransform.col3 + move * mCameraTransform.col2 * 0.1;
    }
}

// ----------------------------------------------------------------------------
DVector3 CameraController::normalizeEulerAngles(const DVector3& eulerAngles)
{
    DVector3 result = eulerAngles;

    result.x = Utils::clamp<double>(result.x, -Ogre::Math::HALF_PI, Ogre::Math::HALF_PI);

    while (result.y > Ogre::Math::PI)
        result.y -= Ogre::Math::TWO_PI;
    while (result.y < -Ogre::Math::PI)
        result.y += Ogre::Math::TWO_PI;

    result.z = 0;

    return result;
}

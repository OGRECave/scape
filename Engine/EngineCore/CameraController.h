/**
 * ScapeEngine::CameraController class
 *
 * Interprets mouse and keyboard camera control input and modifies its attached
 * camera translation and orientation accordingly.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __CAMERACONTROLLER_H__
#define __CAMERACONTROLLER_H__


#include "Tickable.h"
#include "Utils/Utils.h"


namespace ScapeEngine 
{
	
	class CameraController : public Tickable
	{

	public:
		// Create unattached camera controller
		CameraController();

		~CameraController();

		// Attach this camera controller to the given renderview, including
		// the renderview's Ogre::Camera
		void attach(class RenderView* renderView);

		// Seattach this camera controller from the attached rendervier
		void detach();

		// Update internal states, camera position and orientation
		virtual void onFrameTick();

		// Get pick ray for given 2D normalized ([0-1],[0-1] screen coordinate
		DVector3 getViewportPickRayDirection(const DVector3 &screenPosition);

	private:
		// Ogre::RenderView this controller is attached to
		class RenderView*	mRenderView;

		// Initial 3D position of mouse-grabbed point used for grab-trucking and grab-pantilting
		DVector3			mStartGrabRayIntersectionPosition;

		// Unclamped 2D position relative to topleft corner of attached view
		DVector2			mCurrent2DGrabPosition;

		// Double-precision camera transform (orientation and translation)
		DMatrix4			mCameraTransform;

		// Euler-angle representation of camera orientation
		DVector3			mCameraEulerAngles;

		// Is point currently being grabbed by pointer?
		bool				mPointerGrabActive;

		// Simple pointer-controlled camera rotation
		void doPanTilt();

		// Pointer-controlled camera translation based on grabbed point
		void doGrabTruck(bool update2DGrabPoint);

		// Pointer-controlled camera rotation around grabbed point
		void doGrabPanTilt();

		// Pointer-controlled camera dollying (zoom-translation)
		void doGrabDolly();

		// Keyboard-controlled camera translation
		void updateTruck();

		// Keyboard-controlled camera dolly (zoom-translation)
		void updateDolly();

		// Normalize and clamp given euler angles
		static DVector3 normalizeEulerAngles(const DVector3& eulerAngles);
	};

}

#endif // __CAMERACONTROLLER_H__
/**
 * ScapeEngine::RenderView class
 *
 * Combines an Ogre RenderView, camera, getViewport(), a parent window's rectangle and a
 * camera controlling CameraController with eachother. Can be either deattached
 * or attached to a parent window.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __RENDERVIEW_H__
#define __RENDERVIEW_H__

#include "OgreCommon.h"
#include "Utils/Utils.h"
#include "EngineCore/UIElementClasses.h"

namespace ScapeEngine {

    class RenderView : public UIElementPresetContainerSimple
    {
        DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(RenderView)

    public:
        // Create unattached renderview
        RenderView(long viewId);

        // Get globally unique view identifier
        inline long viewId() const { return mViewId; }
        // Get attached camera
        inline class Ogre::Camera* getCamera() const { return mCamera; }
        // Get attached getViewport()
        inline class Ogre::Viewport* getViewport() const { return mViewport; }
        // Get attached embedded getRenderWindow()
        inline class Ogre::RenderWindow* getRenderWindow() const { return mRenderWindow; }
        // Get camera controller
        inline class CameraController* getCameraController() const { return mCameraController; }
        // Attach this renderview to a parent window, creating a new camera, getViewport() and getRenderWindow().
        // Also create a camera controller if this RenderView didn't have one already
        void attach(const string& windowHandle, int left, int top, int width, int height);

        // Deattach renderview. Destroys getRenderWindow(), camera and getViewport(). The camera controller will
        // *not* be destroyed.
        void detach();

        // Destroy renderview. Same as detach(), except it destroys camera controller too.
        void destroy();

        // Let the renderview know its parent window has been moved or resized
        void onMovedOrResized(int left, int top, int width, int height);

        // Helper function that calculates a normalized coordinate ([0-1],[0-1]) from a
        // root window relative pixel coordinate
        bool fromRootToNormalizedLocalSpace(const IVector2& in, Ogre::Vector2& out) const;

        // update statistics and render to view
        virtual void update();

        Ogre::Real getPixelSize() { return mPixelSize; }
        bool getRenderWireframe() { return mRenderWireframe; }
    protected:
        // View identifier
        long mViewId;

        // Attached render window
        class Ogre::RenderWindow* mRenderWindow;

        // Attached camera
        class Ogre::Camera* mCamera;

        // Attached viewport
        class Ogre::Viewport* mViewport;

        // class Ogre::Overlay* mOverlay;

        // Attached camera controller
        class CameraController* mCameraController;

        // Topleft and bottomright coordinate of the getViewport(), relative to the root window
        Ogre::Rect mViewportRect;

        Ogre::Real mPixelSize;
        bool mRenderWireframe;
        bool mRenderDebugInfo;

        bool mDirtyWireframe;
        bool mDirtyRect;

        virtual bool initPersistentElementValueMap(StringStringMap& map);
        virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
        virtual string setUIElementPropertyValue(const string& elementName, const string& value);
    };
}

#endif // __RENDERVIEW_H__
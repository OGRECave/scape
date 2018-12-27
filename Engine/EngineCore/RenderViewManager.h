/**
 * ScapeEngine::RenderViewManager class
 *
 * Singleton class that manages all RenderViews.
 * Use this manager to attach and deattach any RenderView.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __RENDERVIEWMANAGER_H__
#define __RENDERVIEWMANAGER_H__

#include "ScapeEnginePrerequisites.h"

#include "OgreCommon.h"
#include "Utils/Utils.h"

namespace ScapeEngine
{

class RenderViewManager
{

public:
    RenderViewManager();

    // Destructs the manager, cleaning up all remaining render views
    ~RenderViewManager();

    // Create a new render view, embedded in the given parent window
    void createRenderView(int viewId, const string& windowHandle, int left, int top, int width, int height);

    // Deattach an existing and attached renderview
    void detachRenderView(int viewId);

    // Let the renderview manager know one of the render windows has been moved or resized
    void onRenderViewMovedOrResized(int viewId, int left, int top, int width, int height);

    // Get the view identifier of the view under the given position, relative to the root window
    long getViewIdAtRootSpacePosition(const IVector2& point);

    class RenderView* getRenderView(int viewId);

    int getCurrentRenderViewId() { return mCurrentRenderViewId; } // renderview the pointer was last seen in

    void updateAll();

protected:
    // View identifier -> renderview object pointer map type
    typedef std::map<long, class RenderView*> RenderViews;

    // View identifier -> renderview object pointer map
    RenderViews mRenderViews;

    int mCurrentRenderViewId;
};
}

#endif // __RENDERVIEWMANAGER_H__
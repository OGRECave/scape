/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "RenderViewManager.h"
#include "Input/InputManager.h"
#include "Input/InputPointer.h"
#include "PCH/stdafx.h"
#include "RenderView.h"

using namespace ScapeEngine;

template <> RenderViewManager* Ogre::Singleton<RenderViewManager>::msSingleton = 0;

// ----------------------------------------------------------------------------
RenderViewManager::RenderViewManager() : mCurrentRenderViewId(-1) {}

// ----------------------------------------------------------------------------
RenderViewManager::~RenderViewManager()
{
    for (std::pair<long, RenderView*> renderViewPair : mRenderViews)
    {
        RenderView* renderView = renderViewPair.second;
        renderView->destroy();
        delete renderView;
    }
    mRenderViews.clear();
}

// ----------------------------------------------------------------------------
RenderView* RenderViewManager::getRenderView(int viewId)
{
    RenderViews::iterator renderViewsIt = mRenderViews.find(viewId);
    if (renderViewsIt != mRenderViews.end())
    {
        return renderViewsIt->second;
    }
    return NULL;
}

// ----------------------------------------------------------------------------
void RenderViewManager::createRenderView(int viewId, const string& windowHandle, int left, int top,
                                         int width, int height)
{
    RenderView* renderView;
    RenderViews::iterator renderViewsIt = mRenderViews.find(viewId);
    if (renderViewsIt == mRenderViews.end())
    {
        renderView = new RenderView(viewId);
        mRenderViews[viewId] = renderView;
    }
    else
    {
        renderView = renderViewsIt->second;
        assert(renderView->getRenderWindow() == NULL);
        assert(renderView->getViewport() == NULL);
    }

    renderView->attach(windowHandle, left, top, width, height);

    if (mCurrentRenderViewId == -1)
    {
        mCurrentRenderViewId = viewId;
    }
}

// ----------------------------------------------------------------------------
void RenderViewManager::detachRenderView(int viewId)
{
    RenderView* renderView = mRenderViews[viewId];
    renderView->detach();

    if (mCurrentRenderViewId == viewId)
    {
        mCurrentRenderViewId = -1;
    }
}

// ----------------------------------------------------------------------------
void RenderViewManager::onRenderViewMovedOrResized(int viewId, int left, int top, int width, int height)
{
    RenderView* renderView = mRenderViews[viewId];
    renderView->onMovedOrResized(left, top, width, height);
}

// ----------------------------------------------------------------------------
long RenderViewManager::getViewIdAtRootSpacePosition(const IVector2& point)
{
    for (std::pair<long, RenderView*> renderViewPair : mRenderViews)
    {
        RenderView* renderView = renderViewPair.second;
        Ogre::Vector2 out;
        if (renderView->fromRootToNormalizedLocalSpace(point, out))
        {
            return renderViewPair.first;
        }
    }
    return -1;
}

// ----------------------------------------------------------------------------
void RenderViewManager::updateAll()
{
    for (std::pair<long, RenderView*> renderViewPair : mRenderViews)
    {
        renderViewPair.second->update();
    }

    std::pair<RenderView*, Ogre::Vector2> currentView =
        getEngineCore()->getInputManager()->getInputPointer()->getCurrentState().getRenderView2DPoint();
    if (currentView.first)
    {
        mCurrentRenderViewId = currentView.first->viewId();
    }
}

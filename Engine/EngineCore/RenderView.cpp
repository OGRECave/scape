/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "RenderView.h"

#include "Input/InputManager.h"
#include "CameraController.h"
#include "Utils/Utils.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(RenderView)


// ----------------------------------------------------------------------------
enum EPropertyId
{
	PROPERTYID_PIXELSIZE,
	PROPERTYID_WIREFRAME,
	PROPERTYID_DEBUGINFO,
};


// ----------------------------------------------------------------------------
bool RenderView::initPersistentElementValueMap(StringStringMap& map) 
{
	setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString)); 
	return true;
}


// ----------------------------------------------------------------------------
bool RenderView::initPersistentElementStringEnumMap(StringEnumMap& map) 
{
	// lighting properties
	ADD_STRINGENUM(map, PROPERTYID_, PIXELSIZE);
	ADD_STRINGENUM(map, PROPERTYID_, WIREFRAME);
	ADD_STRINGENUM(map, PROPERTYID_, DEBUGINFO);

	return true;
}


// ----------------------------------------------------------------------------
string RenderView::setUIElementPropertyValue(const string& elementName, const string& value) 
{
	EPropertyId propertyId = (EPropertyId)Utils::findEnumFromStringEnumMap(getPersistentElementStringEnumMap(), elementName);
	Ogre::Real realValue = Ogre::StringConverter::parseReal(value);
	bool boolValue = Ogre::StringConverter::parseBool(value);

	string outValue;

	switch (propertyId)
	{
	case PROPERTYID_PIXELSIZE: {realValue = Utils::clamp(realValue, 1.f, 100.0f); mPixelSize = realValue; outValue = Ogre::StringConverter::toString(realValue); if (mRenderWindow) {mDirtyRect = true;} break;}
	case PROPERTYID_WIREFRAME: {mRenderWireframe = boolValue; outValue = Ogre::StringConverter::toString(boolValue); mDirtyWireframe = true; break;}
	case PROPERTYID_DEBUGINFO: {mRenderDebugInfo = boolValue; outValue = Ogre::StringConverter::toString(boolValue); break;}

	default: {Ogre::LogManager::getSingleton().logMessage(_T("RenderView's setUIElementPropertyValue(): unknown elementName '") + elementName + _T("'")); break;}
	}

	getPersistentElementValueMap()[elementName] = outValue;

	return outValue;
}




// ----------------------------------------------------------------------------
RenderView::RenderView(long viewId) :
	mViewId(viewId),
	mRenderWindow(NULL),
	mCamera(NULL),
	mViewport(NULL),
	mCameraController(NULL),
	//mOverlay(NULL),
	mDirtyRect(false),
	mDirtyWireframe(true)
{
	LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();
}


// ----------------------------------------------------------------------------
void RenderView::attach(const string &windowHandle, int left, int top, int width, int height)
{
	assert(mRenderWindow == NULL);
	assert(mViewport == NULL);

	// --------------------
	// Create a new parameters list according to compiled OS
	Ogre::NameValuePairList params;
    params[ "vsync" ] = "true";

	if (windowHandle.length())
	{
		params[_T("externalWindowHandle")] = windowHandle;
	}

	std::ostringstream windowName;
    windowName << _T("RenderView") << mViewId << _T("@") << windowHandle;

	std::ostringstream cameraName;
    cameraName << _T("Camera") << mViewId << _T("@") << windowHandle;

	mRenderWindow = Ogre::Root::getSingleton().createRenderWindow(windowName.str(), width, height, false, &params);
	//mRenderWindow->resize(width, height);

	getEngineCore()->loadSceneManager();
	Ogre::SceneManager* sceneManager = getEngineCore()->getSceneManager();

	mViewportRect = Ogre::Rect(left, top, left + width - 1, top + height - 1);

	if (mCamera == NULL)
	{
		mCamera = sceneManager->createCamera(cameraName.str());
	}

	mCamera->setAspectRatio(Ogre::Real(width) / Ogre::Real(height));

	mCamera->setNearClipDistance( 1 );
	mCamera->setFarClipDistance( 10000 );
	//mCamera->setPolygonMode(Ogre::PM_WIREFRAME);

	mCamera->setPosition(2048,2000,2048);
	mCamera->lookAt(0,0,0);

	Ogre::ColourValue fadeColour(0.8,0.8,0.9);

	mViewport = mRenderWindow->addViewport(mCamera); 
	mViewport->setBackgroundColour(fadeColour);
	//if (Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	//{
	//	mCamera->setFarClipDistance(0);
	//}

	//mOverlay = Ogre::OverlayManager::getSingleton().getByName("DebugOverlay");
	//if (mOverlay)
	//{
	//	mOverlay->show();
	//}

	if (mCameraController == NULL)
	{
		mCameraController = new CameraController();
	}
	mCameraController->attach(this);

	mCameraController->onFrameTick();

	getEngineCore()->loadScene();
}

// ----------------------------------------------------------------------------
void RenderView::onMovedOrResized(int left, int top, int width, int height)
{
	if (mRenderWindow)
	{
		int windowWidth = (int)(width / mPixelSize);
		int windowHeight = (int)(height / mPixelSize);
		mRenderWindow->resize(windowWidth, windowHeight);
		mRenderWindow->windowMovedOrResized(); // TODO
		if (mPixelSize != 1.0f)
		{
			mRenderWindow->resize(width, height);
		}
		mCamera->setAspectRatio(Ogre::Real(width) / Ogre::Real(height));
		mViewportRect = Ogre::Rect(left, top, left + width - 1, top + height - 1);
	}
}


// ----------------------------------------------------------------------------
void RenderView::detach()
{
	if (mRenderWindow != NULL && mViewport != NULL)
	{
		mRenderWindow->removeViewport(mViewport->getZOrder());
		if (mRenderWindow->isPrimary())
		{
			Ogre::Root::getSingleton().getRenderSystem()->detachRenderTarget(mRenderWindow->getName());
		}
		else
		{
			Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTarget(mRenderWindow->getName());
		}
	}
	mRenderWindow = NULL;
	mViewport = NULL;
}


// ----------------------------------------------------------------------------
void RenderView::destroy()
{
	//detach();
	SAFE_DISPOSE(mCameraController);
}


// ----------------------------------------------------------------------------
bool RenderView::fromRootToNormalizedLocalSpace(const IVector2 &in, Ogre::Vector2 &out) const
{
	out.x = (Ogre::Real)(in.x - mViewportRect.left) / (mViewportRect.right - mViewportRect.left);
	out.y = (Ogre::Real)(in.y - mViewportRect.top) / (mViewportRect.bottom - mViewportRect.top);
	return out.x >= 0 && out.x <= 1.0f && out.y >= 0 && out.y <= 1.0f;
}



// ----------------------------------------------------------------------------
void RenderView::update()
{
	if (mDirtyWireframe)
	{
		if (mCamera)
		{
			Ogre::PolygonMode mode = (mRenderWireframe ? Ogre::PM_WIREFRAME : Ogre::PM_SOLID);
			mCamera->setPolygonMode(mode);
		}
		mDirtyWireframe = false;
	}

#if 0
	if (mRenderDebugInfo)
	{
		// update stats when necessary
		try {
			Ogre::OverlayElement* guiAvg = Ogre::OverlayManager::getSingleton().getOverlayElement("DebugOverlay/AverageFps");
			Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("DebugOverlay/NumTris");
			Ogre::OverlayElement* guiBatches = Ogre::OverlayManager::getSingleton().getOverlayElement("DebugOverlay/NumBatches");
			Ogre::OverlayElement* guiDbg = Ogre::OverlayManager::getSingleton().getOverlayElement("DebugOverlay/DebugText");

			const Ogre::RenderTarget::FrameStats& stats = mRenderWindow->getStatistics();

			guiAvg->setCaption(_T(		"Average FPS   : ") + Ogre::StringConverter::toString(stats.avgFPS));
			guiTris->setCaption(_T(		"Triangle Count: ") + Ogre::StringConverter::toString(stats.triangleCount));
			guiBatches->setCaption(_T(	"Batch Count   : ") + Ogre::StringConverter::toString(stats.batchCount));
			guiDbg->setCaption(_T(""));
		}
		catch(...) { /* ignore */ }
		mOverlay->show();
	}
	else
	{
		mOverlay->hide();
	}
#endif

	mRenderWindow->update();

	if (mDirtyRect)
	{
		mDirtyRect = false;
		int width = mViewportRect.right - mViewportRect.left + 1;
		int height = mViewportRect.bottom - mViewportRect.top + 1;
		onMovedOrResized(mViewportRect.left, mViewportRect.top, width, height);
	}


	//renderViewPair.second->getCamera()->setPolygonMode(Ogre::PM_WIREFRAME);

}

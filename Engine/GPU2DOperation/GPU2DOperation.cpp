/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "GPU2DOperation.h"
#include "GPU2DOperationQuad.h"
#include "OgreSimpleRenderable.h"
#include "OgreHardwareBufferManager.h"


using namespace ScapeEngine;


// ----------------------------------------------------------------------------
GPU2DOperation::GPU2DOperation()
:	mViewport(NULL),
	mCamera(NULL),
	mMultiRenderTarget(NULL),
	mDirtyRegion(0, 0, 0, 0)
{
	mSceneManager = Ogre::Root::getSingleton().createSceneManager(_T("DefaultSceneManager"));
}


// ----------------------------------------------------------------------------
GPU2DOperation::~GPU2DOperation()
{
	mRenderTargetTextures.clear();
	mMultiRenderTarget = NULL;
	if (mCamera) {mSceneManager->destroyCamera(mCamera); mCamera = NULL;}
	if (mViewport) {mViewport->getTarget()->removeViewport(0); mViewport = NULL;}
	if (mSceneManager) {Ogre::Root::getSingleton().destroySceneManager(mSceneManager); mSceneManager = NULL;}
	mGPU2DOperationQuads.clear();
}


// ----------------------------------------------------------------------------
void GPU2DOperation::bindMultiRenderTarget(Ogre::TexturePtr renderTargetTexture)
{
	assert(!renderTargetTexture.isNull());

	static int mrttId = 0;

	if (!mMultiRenderTarget)
	{
		mMultiRenderTarget = Ogre::Root::getSingleton().getRenderSystem()->createMultiRenderTarget(_T("GPU2DOperationMRTT")  + Ogre::StringConverter::toString(++mrttId));
		mMultiRenderTarget->setAutoUpdated(false);
		mMultiRenderTarget->bindSurface(0, renderTargetTexture->getBuffer()->getRenderTarget());
		bindSingleRenderTarget(mMultiRenderTarget);
	}
	else
	{
		mMultiRenderTarget->bindSurface(mRenderTargetTextures.size(), renderTargetTexture->getBuffer()->getRenderTarget());
	}

	mRenderTargetTextures.push_back(renderTargetTexture);
}


// ----------------------------------------------------------------------------
void GPU2DOperation::bindSingleRenderTarget(Ogre::RenderTarget* renderTarget)
{
	assert(renderTarget);

	if (mCamera) {mSceneManager->destroyCamera(mCamera); mCamera = NULL;}
	if (mViewport) {mViewport->getTarget()->removeViewport(0); mViewport = NULL;}

	static int cameraId = 0;
	renderTarget->setAutoUpdated(false);

	mCamera = mSceneManager->createCamera(_T("GPU2DOperationCamera") + Ogre::StringConverter::toString(++cameraId));
	mCamera->setAspectRatio((Ogre::Real)renderTarget->getWidth() / renderTarget->getHeight());
	mCamera->setNearClipDistance(0.5);
	mCamera->setFarClipDistance(5.0f);

	mViewport = renderTarget->addViewport(mCamera);
	mViewport->setClearEveryFrame(false);
	mViewport->setOverlaysEnabled(false);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 0.0f));
	mViewport->setShadowsEnabled(false);

	setCameraPosition(0, 0);
}



// ----------------------------------------------------------------------------
void GPU2DOperation::setCameraPosition(int x, int y)
{
	assert(mCamera);
	assert(mViewport);
	Ogre::RenderSystem* renderSystem = Ogre::Root::getSingleton().getRenderSystem();	
	int viewportWidth = mViewport->getActualWidth();
	int viewportHeight = mViewport->getActualHeight();
	Ogre::Real pixelOffsetX = renderSystem->getHorizontalTexelOffset();
	Ogre::Real pixelOffsetY = renderSystem->getVerticalTexelOffset();

	Ogre::Matrix4 viewTransform(
		2.0f/viewportWidth,		 0.0f,					0.0f,		-1.0f+2.0f*(pixelOffsetX - x)/viewportWidth,
		0.0f,					-2.0f/viewportHeight,	0.0f,		 1.0f-2.0f*(pixelOffsetY - y)/viewportHeight,
		0.0f,					 0.0f,					0.0f,		-1.0f,
		0.0f,					 0.0f,					0.0f,		 1.0f);

	mCamera->setCustomViewMatrix(true, viewTransform);
}


// ----------------------------------------------------------------------------
void GPU2DOperation::render(bool clearFirst)
{
	assert(mMultiRenderTarget);
	assert(mViewport);

	mViewport->setClearEveryFrame(clearFirst);

	std::list<GPU2DOperationQuadPtr>::iterator quadIt, quadItEnd = mGPU2DOperationQuads.end();
	Ogre::ushort zOrder = 0;
	for (quadIt = mGPU2DOperationQuads.begin(); quadIt != quadItEnd; ++quadIt)
	{
		(*quadIt)->prepareForRender(zOrder++);
	}

	// check if target isn't locked
	std::list<Ogre::TexturePtr>::iterator textureIt, textureItEnd = mRenderTargetTextures.end();
	for (textureIt = mRenderTargetTextures.begin(); textureIt != textureItEnd; ++textureIt)
	{
		assert(!textureIt->get()->getBuffer()->isLocked());
	}

	mViewport->getTarget()->update();}


// ----------------------------------------------------------------------------
void GPU2DOperation::blitToPixelBox(const Ogre::PixelBox& pixelBox)
{
	mRenderTargetTextures.begin()->get()->getBuffer()->blitToMemory(pixelBox);
}


// ----------------------------------------------------------------------------
void GPU2DOperation::clearQuads()
{
	mGPU2DOperationQuads.clear();
	mSceneManager->clearScene();
}


// ----------------------------------------------------------------------------
void GPU2DOperation::addQuad(GPU2DOperationQuadPtr quad)
{
	mGPU2DOperationQuads.push_back(quad);
	Ogre::SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode();
	node->attachObject(quad->getMovableObject());
}

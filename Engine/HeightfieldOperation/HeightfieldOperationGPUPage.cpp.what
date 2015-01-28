/**
  * G.J.P. de Carpentier, 2007 
  */

#include "PCH/stdafx.h"
#include "HeightfieldOperationGPUPage.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "GPU2DOperation/GPU2DOperationQuad.h"
#include "GPU2DOperation/GPU2DOperationQuadCustom.h"
#include "GPU2DOperation/GPU2DOperationQuadBrush.h"
#include "GPU2DOperation/GPU2DOperation.h"
#include "GPU2DOperation/GPU2DOperationManager.h"
#include "HeightfieldOperationGPUPage.h"
#include "HeightfieldBrush/HeightfieldBrush.h"

//#define DO_PROFILING

#ifdef DO_PROFILING
#define PROFILE(a) a
#else
#define PROFILE(a)
#endif
//PROFILE(unsigned long lastMillis = Ogre::Root::getSingleton().getTimer()->getMilliseconds();)
//PROFILE(int delta = Ogre::Root::getSingleton().getTimer()->getMilliseconds() - lastMillis; lastMillis += delta; printf("contextsetup: %d\n", delta);)

#define MAXRENDERTARGETS 4 //NOTE: must be 1 for OpenGL


using namespace ScapeEngine;


// ----------------------------------------------------------------------------
HeightfieldOperationGPUPage::HeightfieldOperationGPUPage(HeightfieldOperationBrush* operation, const BrushPageCoords pageCoords, const BrushInstances& brushInstances)
:	HeightfieldOperationTaskPage(operation, pageCoords, brushInstances),
	mCurrentGPU2DOperationPtr(GPU2DOperationPtr()),
	mPreviousGPU2DOperationPtr(GPU2DOperationPtr())
{
}

// ----------------------------------------------------------------------------
void HeightfieldOperationGPUPage::tickPending()
{
	if (getEngineCore()->getGPU2DOperationManager()->getActiveGPU2DOperationCount() < MAXRENDERTARGETS)
	{
		printf("HeightfieldOperationGPUPage brush instances: %d\n", mBrushInstances.size());
		mCurrentState = STATE_ACTIVE;
	}			
}


// ----------------------------------------------------------------------------
void HeightfieldOperationGPUPage::tickActive()
{
#if 0
    //	HeightfieldBufferPage* page = mHeightfieldBuffer->getPage(mPageCoords.first, mPageCoords.second, HeightfieldBuffer::PAGEACCESSMODE_READWRITE);
	HeightfieldBufferPage* inPage = mOperation->getInOutHeightfieldBuffer()->getPage(mPageCoords.first, mPageCoords.second, HeightfieldBuffer::PAGEACCESSMODE_READONLY);

	if (!inPage) return;

	Ogre::Rect pageRect = inPage->getAbsoluteRect();

	if (mQuadPtr.isNull())
	{
		mQuadPtr = GPU2DOperationQuadBrushPtr(new GPU2DOperationQuadBrush());
		mQuadPtr->setRenderRect(pageRect);
		mQuadPtr->setHeightRange(inPage->getFirstHeightfieldBuffer()->getHeightRangeMin(), inPage->getFirstHeightfieldBuffer()->getHeightRangeMax());
		mQuadPtr->setBrushShapePower(0.1f);
	}
	
	Ogre::TexturePtr previousTexturePtr;

	if (mCurrentGPU2DOperationPtr.isNull())
	{
		mCurrentGPU2DOperationPtr = getEngineCore()->getGPU2DOperationManager()->getGPU2DOperation(
			inPage->getAbsoluteRect(), 
			inPage->getHeightTexture()->getFormat());

		// important to make sure all quad textures are unlocked!
		inPage->getRawHeightData();
		inPage->unlockAll();
		previousTexturePtr = inPage->getHeightTexture();
	}
	else
	{
		if (mPreviousGPU2DOperationPtr.isNull())
		{
			mPreviousGPU2DOperationPtr = getEngineCore()->getGPU2DOperationManager()->getGPU2DOperation(
				inPage->getAbsoluteRect(), 
				inPage->getHeightTexture()->getFormat());
		}

		GPU2DOperationPtr swapTemp = mPreviousGPU2DOperationPtr;
		mPreviousGPU2DOperationPtr = mCurrentGPU2DOperationPtr;
		mCurrentGPU2DOperationPtr = swapTemp;

		previousTexturePtr = mPreviousGPU2DOperationPtr->getRenderTargetTexture();
	}

	mQuadPtr->setPreviousTexture(previousTexturePtr);

	int quadBrushInstance = 0;
	while (quadBrushInstance < mQuadPtr->getSupportedInstanceCount() && !mBrushInstances.empty())
	{
		BrushInstance& brushInstance = mBrushInstances.front();

		Ogre::Vector3 position = brushInstance.position;
		//position.y = (position.y - page->getFirstHeightfieldBuffer()->getHeightRangeMin()) / (page->getFirstHeightfieldBuffer()->getHeightRangeMax() - page->getFirstHeightfieldBuffer()->getHeightRangeMin());
		//position.y = 0.0f;
		Ogre::Real strength = brushInstance.strength * mOperation->getStrength() * 0.001f;

		mQuadPtr->setBrushInstance(quadBrushInstance, position, strength);
		mQuadPtr->setBrushInnerRadius(brushInstance.innerRadius);
		mQuadPtr->setBrushOuterRadius(brushInstance.outerRadius);

		Ogre::Rect newUpdateRect(
			(long)(position.x - brushInstance.outerRadius - 2), 
			(long)(position.z - brushInstance.outerRadius - 2), 
			(long)(position.x + brushInstance.outerRadius + 2), 
			(long)(position.z + brushInstance.outerRadius + 2));

		mUpdatedRect = Utils::intersectTRect(pageRect, Utils::unionTRect(mUpdatedRect, newUpdateRect));

		mBrushInstances.pop_front();
		++quadBrushInstance;
	}

	if (quadBrushInstance > 0)
	{
		mCurrentGPU2DOperationPtr->addQuad(mQuadPtr);
		mCurrentGPU2DOperationPtr->render(false);
		mCurrentGPU2DOperationPtr->clearQuads();
	}

	if (mBrushInstances.empty())
	{
		if (!mCurrentGPU2DOperationPtr.isNull())
		{
			HeightfieldBufferPage* outPage = mOperation->getTempHeightfieldBuffer()->getPage(mPageCoords.first, mPageCoords.second, HeightfieldBuffer::PAGEACCESSMODE_WRITE_DISCARD);

			Ogre::PixelBox box = outPage->getRawHeightData();
			box.format = outPage->getHeightTexture()->getFormat();
			mCurrentGPU2DOperationPtr->blitToPixelBox(box);

			outPage->increaseVersion();
			//page->getFirstHeightfieldBuffer()->updateGeoms(mUpdatedRect);
			//mUpdatedRect = Ogre::Rect(0, 0, 0, 0);
		}

		mCurrentGPU2DOperationPtr.setNull();
		mPreviousGPU2DOperationPtr.setNull();
		mQuadPtr.setNull();

		mCurrentState = STATE_COMPLETED;
    }
#endif
}


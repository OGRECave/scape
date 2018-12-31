/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
  */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldOperationTaskPageGPUBrush.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "GPU2DOperation/GPU2DOperationQuad.h"
#include "GPU2DOperation/GPU2DOperationQuadCustom.h"
#include "GPU2DOperation/GPU2DOperationQuadBrush.h"
#include "GPU2DOperation/GPU2DOperation.h"
#include "GPU2DOperation/GPU2DOperationManager.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include <iostream>

#define MAXRENDERTARGETS 4 // NOTE: must be 1 for OpenGL

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldOperationTaskPageGPUBrush::HeightfieldOperationTaskPageGPUBrush(
    const string& brushMaterialName, const string& blendMaterialName, int supportedBrushInstanceCount,
    bool onePass, HeightfieldOperationTaskPageGPUListener* quadListener,
    HeightfieldOperationBrush* operation, const BrushPageCoords pageCoords,
    const BrushInstances& brushInstances)
    : HeightfieldOperationTaskPageGPU(quadListener, operation, pageCoords, brushInstances),
      mCurrentGPU2DOperationPtr(GPU2DOperationPtr()), mPreviousGPU2DOperationPtr(GPU2DOperationPtr()),
      mBrushMaterialName(brushMaterialName), mBlendMaterialName(blendMaterialName),
      mSupportedBrushInstanceCount(supportedBrushInstanceCount), mOnePass(onePass), mMaskDone(false),
      mBorderWidth(0), mBorderHeight(0)
{
}

// ----------------------------------------------------------------------------
void HeightfieldOperationTaskPageGPUBrush::setBorderSize(int width, int height)
{
    mBorderWidth = width;
    mBorderHeight = height;
}

// ----------------------------------------------------------------------------
void HeightfieldOperationTaskPageGPUBrush::tickPending()
{
    if (getEngineCore()->getGPU2DOperationManager()->getActiveGPU2DOperationCount() < MAXRENDERTARGETS)
    {
        mCurrentState = STATE_ACTIVE;
    }
}

// ----------------------------------------------------------------------------
void HeightfieldOperationTaskPageGPUBrush::tickActive()
{
    bool hasRendered = false;
    HeightfieldBufferPage* inPage = mOperation->getInOutHeightfieldBuffer()->getPage(
        mPageCoords.first, mPageCoords.second, HeightfieldBuffer::PAGEACCESSMODE_READONLY);
    if (!inPage)
        return;

    // important to make sure all quad textures are unlocked!
    inPage->unlockAll();

    Ogre::Rect pageRect = inPage->getAbsoluteRect();

    GPU2DOperationQuadBrushPtr brushQuadPtr;

    while (!mBrushInstances.empty()) // experimental
    {
        Ogre::TexturePtr previousTexturePtr;

        if (!mCurrentGPU2DOperationPtr)
        {
            brushQuadPtr = GPU2DOperationQuadBrushPtr(
                new GPU2DOperationQuadBrush(mBrushMaterialName, mSupportedBrushInstanceCount));
            //			brushQuadPtr->setRenderRect(pageRect);
            brushQuadPtr->setHeightRange(inPage->getHeightfieldBufferSet()->getHeightRangeMin(),
                                         inPage->getHeightfieldBufferSet()->getHeightRangeMax());

            mCurrentGPU2DOperationPtr = getEngineCore()->getGPU2DOperationManager()->getGPU2DOperation(
                inPage->getAbsoluteRect(),
                // Ogre::PF_FLOAT32_RGBA);
                inPage->getHeightTexture()->getFormat());

            mCurrentGPU2DOperationPtr->addDirtyRegion(pageRect);

            if (mOnePass)
            {
                previousTexturePtr = inPage->getHeightTexture();
            }
            else
            {
                Utils::getTextureFromInternalFile(
                    _T("black"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, _T("black.png"));
                previousTexturePtr = Ogre::TextureManager::getSingleton().getByName(_T("black"));
            }
        }
        else
        {
            if (!mPreviousGPU2DOperationPtr)
            {
                mPreviousGPU2DOperationPtr = getEngineCore()->getGPU2DOperationManager()->getGPU2DOperation(
                    inPage->getAbsoluteRect(), inPage->getHeightTexture()->getFormat());

                mPreviousGPU2DOperationPtr->addDirtyRegion(pageRect);
            }

            GPU2DOperationPtr swapTemp = mPreviousGPU2DOperationPtr;
            mPreviousGPU2DOperationPtr = mCurrentGPU2DOperationPtr;
            mCurrentGPU2DOperationPtr = swapTemp;

            previousTexturePtr = mPreviousGPU2DOperationPtr->getRenderTargetTexture();
        }

        brushQuadPtr->setMaterialTexture(_T("samplerC"), previousTexturePtr);

        int quadBrushInstance = 0;
        Ogre::Rect currentDirtyRect(0, 0, 0, 0);
        while (quadBrushInstance < brushQuadPtr->getSupportedInstanceCount() && !mBrushInstances.empty())
        {
            BrushInstance& brushInstance = mBrushInstances.front();

            Ogre::Vector3 position = brushInstance.position;

            brushQuadPtr->setBrushInstance(quadBrushInstance, position, brushInstance.direction,
                                           brushInstance.strength);
            brushQuadPtr->setInnerRadius(brushInstance.innerRadius);
            brushQuadPtr->setOuterRadius(brushInstance.outerRadius);
            brushQuadPtr->setBrushShapePower(brushInstance.rampPower);

            Ogre::Rect newUpdateRect((long)(position.x - brushInstance.outerRadius - 2),
                                     (long)(position.z - brushInstance.outerRadius - 2),
                                     (long)(position.x + brushInstance.outerRadius + 2),
                                     (long)(position.z + brushInstance.outerRadius + 2));

            currentDirtyRect =
                Utils::intersectTRect(pageRect, Utils::unionTRect(currentDirtyRect, newUpdateRect));

            mBrushInstances.pop_front();
            ++quadBrushInstance;
        }

        mUpdatedRect = Utils::unionTRect(mUpdatedRect, currentDirtyRect);

        if (quadBrushInstance > 0)
        {
            mQuadListener->onSetupGPU2DOperationQuadBrush(brushQuadPtr);

            brushQuadPtr->setRenderRect(currentDirtyRect);
            brushQuadPtr->setMaterialShaderConstantUVRect(_T("coordTransformC"), pageRect);
            mCurrentGPU2DOperationPtr->addQuad(brushQuadPtr);

            GPU2DOperationQuadCustomPtr quadCopyPtrs[4];
            for (int i = 0; i < 4; ++i)
            {
                Ogre::Rect rect;

                switch (i)
                {
                case 0:
                    rect = Ogre::Rect(pageRect.left, pageRect.top, currentDirtyRect.left, pageRect.bottom);
                    break; // NW, W, SW
                case 1:
                    rect = Ogre::Rect(currentDirtyRect.left, pageRect.top, currentDirtyRect.right,
                                      currentDirtyRect.top);
                    break; // N
                case 2:
                    rect =
                        Ogre::Rect(currentDirtyRect.right, pageRect.top, pageRect.right, pageRect.bottom);
                    break; // NW, W, SW
                case 3:
                    rect = Ogre::Rect(currentDirtyRect.left, currentDirtyRect.bottom,
                                      currentDirtyRect.right, pageRect.bottom);
                    break; // S
                }

                Ogre::Rect dirtyRegion = mCurrentGPU2DOperationPtr->getDirtyRegion();
                if (mPreviousGPU2DOperationPtr)
                    dirtyRegion =
                        Utils::unionTRect(dirtyRegion, mPreviousGPU2DOperationPtr->getDirtyRegion());

                rect = Utils::intersectTRect(rect, dirtyRegion);

                if (rect.right > rect.left && rect.bottom > rect.top)
                {
                    quadCopyPtrs[i] = GPU2DOperationQuadCustomPtr(new GPU2DOperationQuadCustom());
                    quadCopyPtrs[i]->setMaterialName(_T("GPU2DOperationCopy"));
                    quadCopyPtrs[i]->setRenderRect(rect);
                    quadCopyPtrs[i]->setMaterialShaderConstantUVRect(_T("coordTransformC"), pageRect);
                    quadCopyPtrs[i]->setMaterialTexture(_T("samplerC"), previousTexturePtr);
                    mCurrentGPU2DOperationPtr->addQuad(quadCopyPtrs[i]);
                }
            }

            mCurrentGPU2DOperationPtr->render(false);
            mCurrentGPU2DOperationPtr->clearQuads();
            mCurrentGPU2DOperationPtr->clearDirtyRegion();
            mCurrentGPU2DOperationPtr->addDirtyRegion(currentDirtyRect);
            hasRendered = true;
        }
    }

    if (!mOnePass)
    {
        if (mBrushInstances.empty() && !mMaskDone)
        {
            mMaskDone = true;

            if (!mPreviousGPU2DOperationPtr)
            {
                mPreviousGPU2DOperationPtr = getEngineCore()->getGPU2DOperationManager()->getGPU2DOperation(
                    inPage->getAbsoluteRect(), inPage->getHeightTexture()->getFormat());
            }

            GPU2DOperationPtr swapTemp = mPreviousGPU2DOperationPtr;
            mPreviousGPU2DOperationPtr = mCurrentGPU2DOperationPtr;
            mCurrentGPU2DOperationPtr = swapTemp;
            //
            Ogre::Rect centerRect =
                Ogre::Rect(pageRect.left + mBorderWidth, pageRect.top + mBorderHeight,
                           pageRect.right - mBorderWidth, pageRect.bottom - mBorderHeight);
            centerRect = Utils::intersectTRect(centerRect, mUpdatedRect);
            std::cout << centerRect.left << " " << centerRect.top << " " << centerRect.right << " "
                      << centerRect.bottom << "\n"
                      << std::flush;
            //

            GPU2DOperationQuadCustomPtr quadCenterPtr =
                GPU2DOperationQuadCustomPtr(new GPU2DOperationQuadCustom());
            inPage->unlockAll();

            quadCenterPtr->setRenderRect(centerRect);
            quadCenterPtr->setMaterialName(mBlendMaterialName);
            quadCenterPtr->setMaterialTexture(_T("samplerC"), inPage->getHeightTexture());
            quadCenterPtr->setMaterialTexture(_T("samplerMask"),
                                              mPreviousGPU2DOperationPtr->getRenderTargetTexture());

            quadCenterPtr->setMaterialShaderConstantUVRect(_T("coordTransformC"), pageRect);

            mQuadListener->onSetupGPU2DOperationQuadCustom(quadCenterPtr);
            mCurrentGPU2DOperationPtr->addQuad(quadCenterPtr);

            const size_t neighborCount = Utils::MooreNeighborhood::DIRECTION_ENUM_LENGTH;
            GPU2DOperationQuadCustomPtr quadBorderPtrs[neighborCount];

            for (size_t i = 0; i < neighborCount; ++i)
            {
                Utils::MooreNeighborhood::EDirection direction = (Utils::MooreNeighborhood::EDirection)i;
                int x = Utils::MooreNeighborhood::directionToX(direction);
                int y = Utils::MooreNeighborhood::directionToY(direction);
                assert(x != 0 || y != 0);

                Ogre::Rect renderRect = pageRect;

                if (x < 0)
                {
                    renderRect.right = renderRect.left + mBorderWidth;
                }
                else if (x > 0)
                {
                    renderRect.left = renderRect.right - mBorderWidth;
                }
                else
                {
                    renderRect.left += mBorderWidth;
                    renderRect.right -= mBorderWidth;
                }

                if (y < 0)
                {
                    renderRect.bottom = renderRect.top + mBorderHeight;
                }
                else if (y > 0)
                {
                    renderRect.top = renderRect.bottom - mBorderHeight;
                }
                else
                {
                    renderRect.top += mBorderHeight;
                    renderRect.bottom -= mBorderHeight;
                }

                renderRect = Utils::intersectTRect(renderRect, mUpdatedRect);

                if (renderRect.right > renderRect.left && renderRect.bottom > renderRect.top)
                {
                    quadBorderPtrs[i] = GPU2DOperationQuadCustomPtr(new GPU2DOperationQuadCustom());
                    quadBorderPtrs[i]->setRenderRect(renderRect);

                    string materialName = mBlendMaterialName + _T("2");

                    setOperationQuadNeighbor(quadBorderPtrs[i], _T("coordTransformC"), _T("samplerC"), 0,
                                             0);
                    setOperationQuadNeighbor(quadBorderPtrs[i], _T("coordTransformC2"), _T("samplerC2"), x,
                                             y);

                    if (x != 0 && y != 0)
                    {
                        materialName = mBlendMaterialName + _T("4");
                        setOperationQuadNeighbor(quadBorderPtrs[i], _T("coordTransformC3"), _T("samplerC3"),
                                                 x, 0);
                        setOperationQuadNeighbor(quadBorderPtrs[i], _T("coordTransformC4"), _T("samplerC4"),
                                                 0, y);
                    }

                    quadBorderPtrs[i]->setMaterialName(materialName);

                    quadBorderPtrs[i]->setMaterialTexture(
                        _T("samplerMask"), mPreviousGPU2DOperationPtr->getRenderTargetTexture());

                    mQuadListener->onSetupGPU2DOperationQuadCustom(quadBorderPtrs[i]);

                    mCurrentGPU2DOperationPtr->addQuad(quadBorderPtrs[i]);
                }
            }

            GPU2DOperationQuadCustomPtr quadCopyPtrs[4];
            for (int i = 0; i < 4; ++i)
            {
                Ogre::Rect rect;

                switch (i)
                {
                case 0:
                    rect = Ogre::Rect(pageRect.left, pageRect.top, mUpdatedRect.left, pageRect.bottom);
                    break; // NW, W, SW
                case 1:
                    rect =
                        Ogre::Rect(mUpdatedRect.left, pageRect.top, mUpdatedRect.right, mUpdatedRect.top);
                    break; // N
                case 2:
                    rect = Ogre::Rect(mUpdatedRect.right, pageRect.top, pageRect.right, pageRect.bottom);
                    break; // NW, W, SW
                case 3:
                    rect = Ogre::Rect(mUpdatedRect.left, mUpdatedRect.bottom, mUpdatedRect.right,
                                      pageRect.bottom);
                    break; // S
                }

                if (rect.right > rect.left && rect.bottom > rect.top)
                {
                    quadCopyPtrs[i] = GPU2DOperationQuadCustomPtr(new GPU2DOperationQuadCustom());
                    quadCopyPtrs[i]->setMaterialName(_T("GPU2DOperationCopy"));
                    quadCopyPtrs[i]->setRenderRect(rect);
                    quadCopyPtrs[i]->setMaterialShaderConstantUVRect(_T("coordTransformC"), pageRect);
                    quadCopyPtrs[i]->setMaterialTexture(_T("samplerC"), inPage->getHeightTexture());
                    mCurrentGPU2DOperationPtr->addQuad(quadCopyPtrs[i]);
                }
            }

            mCurrentGPU2DOperationPtr->render(false);
            mCurrentGPU2DOperationPtr->clearQuads();
            hasRendered = true;
        }
    }

    if (!hasRendered)
    {
        if (mBrushInstances.empty())
        {
            if (mCurrentGPU2DOperationPtr)
            {
                HeightfieldBufferPage* outPage = mOperation->getTempHeightfieldBuffer()->getPage(
                    mPageCoords.first, mPageCoords.second, HeightfieldBuffer::PAGEACCESSMODE_WRITE_DISCARD);

                // this can take a lot of time as any queued renderings are executed immediately while
                // waiting on the result (blocking call)
                Ogre::PixelBox box = outPage->getRawHeightData();
                box.format = outPage->getHeightTexture()->getFormat();

                mCurrentGPU2DOperationPtr->blitToPixelBox(box);

                /*
                {
                        GPU2DOperation* render = new GPU2DOperation();
                        render->bindSingleRenderTarget((Ogre::RenderTexture*)getEngineCore()->getDebugRenderWindow());

                        GPU2DOperationQuadCustomPtr quad = GPU2DOperationQuadCustomPtr(new
                GPU2DOperationQuadCustom());
                        quad->setRenderRect(Ogre::Rect(0,0,512,512));
                        quad->setMaterialName(_T("GPU2DOperationUnpack"));
                        outPage->unlockAll();
                        quad->setMaterialTexture(_T("samplerC"), outPage->getHeightTexture());//
                mCurrentGPU2DOperationPtr->getRenderTargetTexture());
                        quad->setMaterialShaderConstantUVRect(_T("coordTransformC"),
                Ogre::Rect(0,0,512,512));

                        render->addQuad(quad);
                        render->render(false);

                        delete render;
                }
                */

                outPage->increaseVersion();
            }

            mCurrentGPU2DOperationPtr.reset();
            mPreviousGPU2DOperationPtr.reset();

            mCurrentState = STATE_COMPLETED;
        }
    }
}

// ----------------------------------------------------------------------------
void HeightfieldOperationTaskPageGPUBrush::setOperationQuadNeighbor(GPU2DOperationQuadCustomPtr& quadPtr,
                                                                    const string& constantCoordName,
                                                                    const string& textureAliasName,
                                                                    int deltaX, int deltaY)
{
    HeightfieldBufferPage* centerPage = mOperation->getInOutHeightfieldBuffer()->getPage(
        mPageCoords.first, mPageCoords.second, HeightfieldBuffer::PAGEACCESSMODE_READONLY);

    bool flipX = false;
    bool flipY = false;

    Ogre::Rect centerRect = centerPage->getAbsoluteRect();
    Ogre::Rect neighborRect =
        Utils::translatedRect(centerRect, deltaX * centerRect.width(), deltaY * centerRect.height());

    HeightfieldBufferPage* neighborPage = mOperation->getInOutHeightfieldBuffer()->getPage(
        mPageCoords.first + deltaX, mPageCoords.second + deltaY,
        HeightfieldBuffer::PAGEACCESSMODE_READONLY);

    if (neighborPage)
    {
        Ogre::Rect neighborRectTest = neighborPage->getAbsoluteRect();
        assert(neighborRectTest.left == neighborRect.left);
        assert(neighborRectTest.right == neighborRect.right);
        assert(neighborRectTest.top == neighborRect.top);
        assert(neighborRectTest.bottom == neighborRect.bottom);
    }
    else
    {
        neighborPage = centerPage;

        flipX = (deltaX != 0);
        flipY = (deltaY != 0);
    }

    neighborPage->unlockAll();

    quadPtr->setMaterialShaderConstantUVRect(constantCoordName, neighborRect, flipX, flipY);
    quadPtr->setMaterialTexture(textureAliasName, neighborPage->getHeightTexture());
}

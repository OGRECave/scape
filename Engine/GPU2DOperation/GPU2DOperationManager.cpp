/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
#include "GPU2DOperation.h"
#include "GPU2DOperationQuad.h"
#include "GPU2DOperationManager.h"

using namespace ScapeEngine;

#define SECONDS_KEEP_UNUSED_GPU2DOPERATIONRENDERS 3.0f

// ----------------------------------------------------------------------------
GPU2DOperationManager::GPU2DOperationManager()
    : mTextureCounter(0), mTotalGPU2DOperationCount(0), mActiveGPU2DOperationCount(0)
{
    wantsPostFrameTick();
}

// ----------------------------------------------------------------------------
GPU2DOperationManager::~GPU2DOperationManager()
{
    GPU2DOperationMap::iterator mapIt, mapItEnd = mGPU2DOperationMap.end();
    for (mapIt = mGPU2DOperationMap.begin(); mapIt != mapItEnd; ++mapIt)
    {
        GPU2DOperationInstanceList& instanceList = mapIt->second;

        GPU2DOperationInstanceList::iterator listIt, listItEnd = instanceList.end();
        for (listIt = instanceList.begin(); listIt != listItEnd;)
        {
            GPU2DOperationPtr render = listIt->ptr;
            string textureName = render->getRenderTargetTexture()->getName();
            Ogre::LogManager::getSingleton().logMessage(_T("Destroying GPU2DOperation rtt '") + textureName + _T("'"));
            Ogre::TextureManager::getSingleton().remove(textureName);
            listIt = instanceList.erase(listIt);
            --mTotalGPU2DOperationCount;
        }
    }
}

// ----------------------------------------------------------------------------
GPU2DOperationPtr GPU2DOperationManager::getGPU2DOperation(const Ogre::Rect& renderRect, Ogre::PixelFormat pixelFormat)
{
    GPU2DOperationGroup renderGroup = { renderRect.width(), renderRect.height(), pixelFormat };

    GPU2DOperationInstanceList& instanceList = mGPU2DOperationMap[renderGroup];

    GPU2DOperationPtr gpu2DRenderPtr;

    GPU2DOperationInstanceList::iterator listIt, listItEnd = instanceList.end();
    for (listIt = instanceList.begin(); listIt != listItEnd; ++listIt)
    {
        printf("%s: %d\n", listIt->ptr->getRenderTargetTexture()->getName().c_str(), listIt->ptr.useCount());
        if (listIt->ptr.useCount() == 1) // GPU2DOperation object not referenced outside manager
        {
            gpu2DRenderPtr = listIt->ptr;
            listIt->timestamp = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
            break;
        }
    }

    if (gpu2DRenderPtr.isNull())
    {
        string textureName = _T("GPU2DOperation") + Ogre::StringConverter().toString(renderGroup.width) + _T("x")
            + Ogre::StringConverter().toString(renderGroup.height) + _T(":")
            + Ogre::StringConverter().toString(renderGroup.pixelFormat) + _T("_")
            + Ogre::StringConverter().toString(mTextureCounter++);

        Ogre::LogManager::getSingleton().logMessage(_T("Creating GPU2DOperation rtt '") + textureName + _T("'"));

        Ogre::TexturePtr renderTexture = Ogre::TextureManager::getSingleton().createManual(textureName,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, renderGroup.width,
            renderGroup.height, 1, renderGroup.pixelFormat, Ogre::TU_RENDERTARGET);

        renderTexture->getBuffer()->getRenderTarget()->setAutoUpdated(false);

        Ogre::HardwarePixelBufferSharedPtr renderBuffer = renderTexture->getBuffer();

        gpu2DRenderPtr.bind(new GPU2DOperation());
        gpu2DRenderPtr->bindMultiRenderTarget(renderTexture);

        //
        /*
        Ogre::TexturePtr renderTexture2 = Ogre::TextureManager::getSingleton().createManual(
                textureName,
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                Ogre::TEX_TYPE_2D,
                renderGroup.width,
                renderGroup.height,
                1,
                renderGroup.pixelFormat,
                Ogre::TU_RENDERTARGET);

        renderTexture->getBuffer()->getRenderTarget()->setAutoUpdated(false);
        gpu2DRenderPtr->bindRenderTarget(renderTexture2);
        */
        //

        GPU2DOperationInstance instance;
        instance.ptr = gpu2DRenderPtr;
        instance.timestamp = Ogre::Root::getSingleton().getTimer()->getMilliseconds();

        instanceList.push_back(instance);

        ++mTotalGPU2DOperationCount;
        Ogre::LogManager::getSingleton().logMessage("Total number of GPU2DOperation rtt textures: "
            + Ogre::StringConverter::toString(mTotalGPU2DOperationCount) + _T(" (active est: ")
            + Ogre::StringConverter::toString(mActiveGPU2DOperationCount) + _T(")"));
    }

    gpu2DRenderPtr->setCameraPosition(renderRect.left, renderRect.top);
    gpu2DRenderPtr->clearQuads();

    ++mActiveGPU2DOperationCount;

    return gpu2DRenderPtr;
}

// ----------------------------------------------------------------------------
size_t GPU2DOperationManager::getActiveGPU2DOperationCount()
{
    mActiveGPU2DOperationCount = 0;

    GPU2DOperationMap::iterator mapIt, mapItEnd = mGPU2DOperationMap.end();
    for (mapIt = mGPU2DOperationMap.begin(); mapIt != mapItEnd; ++mapIt)
    {
        GPU2DOperationInstanceList& instanceList = mapIt->second;

        GPU2DOperationInstanceList::iterator listIt, listItEnd = instanceList.end();
        for (listIt = instanceList.begin(); listIt != listItEnd; ++listIt)
        {
            if (listIt->ptr.useCount() != 1)
            {
                ++mActiveGPU2DOperationCount;
            }
        }
    }

    return mActiveGPU2DOperationCount;
}

// ----------------------------------------------------------------------------
void GPU2DOperationManager::onPostFrameTick()
{
    unsigned long currentMillis = Ogre::Root::getSingleton().getTimer()->getMilliseconds();

    mActiveGPU2DOperationCount = 0;

    GPU2DOperationMap::iterator mapIt, mapItEnd = mGPU2DOperationMap.end();
    for (mapIt = mGPU2DOperationMap.begin(); mapIt != mapItEnd; ++mapIt)
    {
        GPU2DOperationInstanceList& instanceList = mapIt->second;

        GPU2DOperationInstanceList::iterator listIt, listItEnd = instanceList.end();
        for (listIt = instanceList.begin(); listIt != listItEnd;)
        {
            if (listIt->ptr.useCount() == 1)
            {
                if (listIt->timestamp + (int)(1000 * SECONDS_KEEP_UNUSED_GPU2DOPERATIONRENDERS) < currentMillis)
                {
                    string textureName = listIt->ptr->getRenderTargetTexture()->getName();
                    Ogre::LogManager::getSingleton().logMessage(_T("Destroying GPU2DOperation rtt '") + textureName
                        + _T("' (untouched: ") + Ogre::StringConverter::toString(currentMillis - listIt->timestamp)
                        + _T(" ms )"));
                    Ogre::TextureManager::getSingleton().remove(textureName);
                    listIt = instanceList.erase(listIt);
                    --mTotalGPU2DOperationCount;
                    Ogre::LogManager::getSingleton().logMessage("Total number of GPU2DOperation rtt textures: "
                        + Ogre::StringConverter::toString(mTotalGPU2DOperationCount) + _T(" (active est: ")
                        + Ogre::StringConverter::toString(mActiveGPU2DOperationCount) + _T(")"));
                }
                else
                {
                    ++listIt;
                }
            }
            else
            {
                ++mActiveGPU2DOperationCount;
                ++listIt;
            }
        }
    }
}

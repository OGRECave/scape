/**
 * ScapeEngine::GPU2DOperationManager class
 *
 * Manages the pool of GPU2DOperation objects
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __GPU2DOPERATIONMANAGER_H__
#define __GPU2DOPERATIONMANAGER_H__

#include "EngineCore/Tickable.h"
#include "GPU2DOperation.h"

namespace ScapeEngine
{

class GPU2DOperationManager : public Tickable
{
public:
    GPU2DOperationManager();
    ~GPU2DOperationManager();

    GPU2DOperationPtr getGPU2DOperation(const Ogre::Rect& renderRect, Ogre::PixelFormat pixelFormat);

    size_t getActiveGPU2DOperationCount();

    void onPostFrameTick();

protected:
    struct GPU2DOperationGroup
    {
        long width;
        long height;
        Ogre::PixelFormat pixelFormat;

        inline bool operator==(const GPU2DOperationGroup& rhs) const
        {
            return width == rhs.width && height == rhs.height && pixelFormat == rhs.pixelFormat;
        }

        inline bool operator<(const GPU2DOperationGroup& rhs) const
        {
            return (width < rhs.width) ||
                   (width == rhs.width &&
                    ((height < rhs.height) || (height == rhs.height && pixelFormat < rhs.pixelFormat)));
        }
    };

    struct GPU2DOperationInstance
    {
        unsigned long timestamp;
        GPU2DOperationPtr ptr;
    };

    typedef std::list<GPU2DOperationInstance> GPU2DOperationInstanceList;
    typedef std::map<GPU2DOperationGroup, GPU2DOperationInstanceList> GPU2DOperationMap;
    GPU2DOperationMap mGPU2DOperationMap;

    int mTextureCounter;
    int mTotalGPU2DOperationCount;
    int mActiveGPU2DOperationCount; // this is an optimistic approximation, corrected each frame
};
}

#endif // __GPU2DOPERATIONMANAGER_H__

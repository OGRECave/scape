/**
 * ScapeEngine::GPU2DOperationRenderableQuadManager class
 *
 * Manages (allocates and deallocated) GPU2DOperationRenderableQuads
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __GPU2DOPERATIONRENDERABLEQUADMANAGER_H__
#define __GPU2DOPERATIONRENDERABLEQUADMANAGER_H__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{

class GPU2DOperationRenderableQuadManager
{
public:
    GPU2DOperationRenderableQuadManager();
    ~GPU2DOperationRenderableQuadManager();

    GPU2DOperationRenderableQuad* getNewQuad();

    void returnQuad(GPU2DOperationRenderableQuad* quad);

protected:
    typedef std::list<GPU2DOperationRenderableQuad*> FreeQuads;
    FreeQuads mFreeQuads;
};
}

#endif // __GPU2DOPERATIONRENDERABLEQUADMANAGER_H__

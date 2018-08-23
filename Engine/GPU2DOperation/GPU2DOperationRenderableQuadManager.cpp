/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
#include "GPU2DOperationRenderableQuad.h"
#include "GPU2DOperationRenderableQuadManager.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
GPU2DOperationRenderableQuadManager::GPU2DOperationRenderableQuadManager() {}

// ----------------------------------------------------------------------------
GPU2DOperationRenderableQuadManager::~GPU2DOperationRenderableQuadManager()
{
    FreeQuads::iterator it, itEnd = mFreeQuads.end();
    for (it = mFreeQuads.begin(); it != itEnd; ++it)
    {
        delete *it;
    }
}

// ----------------------------------------------------------------------------
GPU2DOperationRenderableQuad* GPU2DOperationRenderableQuadManager::getNewQuad()
{
    if (mFreeQuads.empty())
    {
        return new GPU2DOperationRenderableQuad();
    }
    else
    {
        GPU2DOperationRenderableQuad* quad = *mFreeQuads.rbegin();
        mFreeQuads.pop_back();
        return quad;
    }
}

// ----------------------------------------------------------------------------
void GPU2DOperationRenderableQuadManager::returnQuad(GPU2DOperationRenderableQuad* quad)
{
    assert(quad);
    quad->reset();
    mFreeQuads.push_back(quad);
}

/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "GPU2DOperationQuad.h"
#include "GPU2DOperationRenderableQuad.h"
#include "GPU2DOperationRenderableQuadManager.h"
#include "OgreHardwareBufferManager.h"
#include "OgreSimpleRenderable.h"
#include "PCH/stdafx.h"
#include "Utils/ShaderCustomAutoConstants.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
GPU2DOperationQuad::GPU2DOperationQuad()
{
    mGPU2DOperationableQuad = getEngineCore()->getGPU2DOperationRenderableQuadManager()->getNewQuad();
}

// ----------------------------------------------------------------------------
GPU2DOperationQuad::~GPU2DOperationQuad()
{
    getEngineCore()->getGPU2DOperationRenderableQuadManager()->returnQuad(mGPU2DOperationableQuad);
    mGPU2DOperationableQuad = NULL;
}

// ----------------------------------------------------------------------------
Ogre::MovableObject* GPU2DOperationQuad::getMovableObject() { return mGPU2DOperationableQuad; }

// ----------------------------------------------------------------------------
Ogre::Renderable* GPU2DOperationQuad::getRenderable() { return mGPU2DOperationableQuad; }

// ----------------------------------------------------------------------------
void GPU2DOperationQuad::setRenderRect(const Ogre::Rect& rect)
{
    mGPU2DOperationableQuad->setRenderRect(rect);
}

// ----------------------------------------------------------------------------
const Ogre::Rect& GPU2DOperationQuad::getRenderRect() { return mGPU2DOperationableQuad->getRenderRect(); }

// ----------------------------------------------------------------------------
void GPU2DOperationQuad::prepareForRender(Ogre::ushort zOrder)
{
    mGPU2DOperationableQuad->prepareForRender(zOrder);
}
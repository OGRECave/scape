/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "GPU2DOperationQuadCustom.h"
#include "GPU2DOperationRenderableQuad.h"

using namespace ScapeEngine;


// ----------------------------------------------------------------------------
void GPU2DOperationQuadCustom::setMaterialName(const string& materialName)
{
	mGPU2DOperationableQuad->setMaterialName(materialName);
}


// ----------------------------------------------------------------------------
void GPU2DOperationQuadCustom::setMaterialShaderConstant(const string& constantName, const Ogre::Vector4& data)
{
	mGPU2DOperationableQuad->setMaterialShaderConstant(constantName, data);
}


// ----------------------------------------------------------------------------
void GPU2DOperationQuadCustom::setMaterialShaderConstant(const string& constantName, const Ogre::Matrix4& data)
{
	mGPU2DOperationableQuad->setMaterialShaderConstant(constantName, data);
}


// ----------------------------------------------------------------------------
void GPU2DOperationQuadCustom::setMaterialShaderConstant(const string& constantName, const Ogre::Real* data, int dataCount)
{
	mGPU2DOperationableQuad->setMaterialShaderConstant(constantName, data, dataCount);
}


// ----------------------------------------------------------------------------
void GPU2DOperationQuadCustom::setMaterialShaderConstantUVRect(const string& constantName, const Ogre::Rect& rect, bool flipX, bool flipY)
{
	mGPU2DOperationableQuad->setMaterialShaderConstantUVRect(constantName, rect, flipX, flipY);
}


// ----------------------------------------------------------------------------
void GPU2DOperationQuadCustom::setMaterialTexture(const string& aliasName, const Ogre::TexturePtr& texturePtr)
{
	mGPU2DOperationableQuad->setMaterialTexture(aliasName, texturePtr);
}

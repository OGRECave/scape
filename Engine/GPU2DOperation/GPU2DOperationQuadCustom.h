/**
 * ScapeEngine::GPU2DOperationQuad class
 *
 * 2D rectangle that can be added to a GPU2DOperation with a custom material and custom shader settings
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __GPU2DOPERATIONQUADCUSTOM_H__
#define __GPU2DOPERATIONQUADCUSTOM_H__

#include "GPU2DOperationQuad.h"

namespace ScapeEngine
{
class GPU2DOperationQuadCustom : public GPU2DOperationQuad
{
public:
    virtual void setMaterialName(const string& materialName);
    virtual void setMaterialTexture(const string& aliasName, const Ogre::TexturePtr& texturePtr);
    virtual void setMaterialShaderConstant(const string& constantName, const Ogre::Vector4& data);
    virtual void setMaterialShaderConstant(const string& constantName, const Ogre::Matrix4& data);
    virtual void setMaterialShaderConstant(const string& constantName, const Ogre::Real* data,
                                           int dataCount);
    virtual void setMaterialShaderConstantUVRect(const string& constantName, const Ogre::Rect& rect,
                                                 bool flipX = false, bool flipY = false);
};

typedef Ogre::SharedPtr<GPU2DOperationQuadCustom> GPU2DOperationQuadCustomPtr;
}

#endif // __GPU2DOPERATIONQUADCUSTOM_H__

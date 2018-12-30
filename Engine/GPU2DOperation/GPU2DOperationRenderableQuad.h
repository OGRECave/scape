/**
 * ScapeEngine::GPU2DOperationRenderableQuad class
 *
 * 2D rectangle that can be added to a GPU2DOperation
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __GPU2DOPERATIONRENDERABLEQUAD_H__
#define __GPU2DOPERATIONRENDERABLEQUAD_H__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{

class GPU2DOperationRenderableQuad : public Ogre::SimpleRenderable
{
public:
    GPU2DOperationRenderableQuad();

    ~GPU2DOperationRenderableQuad();

    virtual void reset();

    void setRenderRect(const Ogre::Rect& rect);
    const Ogre::Rect& getRenderRect();

    virtual void prepareForRender(Ogre::ushort zOrder);

    virtual void setMaterialName(const string& materialName);

    virtual void setMaterialTexture(const string& aliasName, const Ogre::TexturePtr& texturePtr);

    virtual void setMaterialShaderConstant(const string& constantName, const Ogre::Real* data,
                                           int dataCount);
    virtual void setMaterialShaderConstant(const string& constantName, const Ogre::Vector4& data);
    virtual void setMaterialShaderConstant(const string& constantName, const Ogre::Matrix4& data);
    virtual void setMaterialShaderConstantUVRect(const string& constantName, const Ogre::Rect& rect,
                                                 bool flipX = false, bool flipY = false);

protected:
    typedef std::map<string, std::pair<int, bool>>
        ShaderConstantNameToIndexMap; // pair: (constantindex,dirty)
    ShaderConstantNameToIndexMap mShaderConstantNameToIndexMap;
    Ogre::AliasTextureNamePairList mNewAliasTextureMap;
    Ogre::AliasTextureNamePairList mAliasTextureMap;

    ShaderCustomAutoConstants* mShaderCustomAutoConstants;

    bool mDirtyVertexBuffer;
    bool mDirtyMaterialName;
    string mMaterialName;
    Ogre::Rect mRenderRect;
    Ogre::ushort mZOrder;

    int getMaterialShaderConstantIndex(const string& constantName, bool makeDirty);

    void getWorldTransforms(Ogre::Matrix4* xform) const;
    const Ogre::Quaternion& getWorldOrientation(void) const;
    const Ogre::Vector3& getWorldPosition(void) const;

    void _updateCustomGpuParameter(const Ogre::GpuProgramParameters::AutoConstantEntry& constantEntry,
                                   Ogre::GpuProgramParameters* params) const;

    void _updateRenderQueue(Ogre::RenderQueue* queue);

    Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const { return 0; }

    Ogre::Real getBoundingRadius(void) const { return 0; }

    void loadMaterial(const string& materialName);
};
}

#endif // __GPU2DOPERATIONRENDERABLEQUAD_H__

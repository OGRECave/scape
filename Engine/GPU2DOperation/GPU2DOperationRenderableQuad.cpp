/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "GPU2DOperationRenderableQuad.h"
#include "OgreHardwareBufferManager.h"
#include "OgreSimpleRenderable.h"
#include "PCH/stdafx.h"
#include "Utils/ShaderCustomAutoConstants.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
GPU2DOperationRenderableQuad::GPU2DOperationRenderableQuad() : mDirtyVertexBuffer(true)
{
    mUseIdentityProjection = true;
    mUseIdentityView = false;

    mRenderOp.vertexData = new Ogre::VertexData();

    mRenderOp.indexData = 0;
    mRenderOp.vertexData->vertexCount = 4;
    mRenderOp.vertexData->vertexStart = 0;
    mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
    mRenderOp.useIndexes = false;

    Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

    decl->addElement(0, 0, Ogre::VET_FLOAT2, Ogre::VES_POSITION);

    Ogre::HardwareVertexBufferSharedPtr vbuf =
        Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(0), mRenderOp.vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    // Bind buffer
    bind->setBinding(0, vbuf);

    mBox.setInfinite();

    mShaderCustomAutoConstants = new ShaderCustomAutoConstants();

    mMaterialName = _T("GPU2DOperationQuad_") + Ogre::StringConverter::toString((long)this);
    mMaterial = Ogre::MaterialManager::getSingleton().create(
        mMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    mDirtyMaterialName = false;

    reset();
}

// ----------------------------------------------------------------------------
void GPU2DOperationRenderableQuad::reset()
{
    mShaderCustomAutoConstants->clear();
    mShaderConstantNameToIndexMap.clear();
    mNewAliasTextureMap.clear();
    mAliasTextureMap.clear();
    mRenderRect = Ogre::Rect(0, 0, 0, 0);
    mZOrder = 0;

    mMaterialName = _T("BaseWhite");
    loadMaterial(mMaterialName);
    mDirtyMaterialName = false;
}

// ----------------------------------------------------------------------------
GPU2DOperationRenderableQuad::~GPU2DOperationRenderableQuad()
{
    if (!getMaterial().isNull())
    {
        Ogre::ResourcePtr resource(mMaterial);
        mMaterial.setNull();
        Ogre::MaterialManager::getSingleton().remove(resource);
        resource.setNull();
    }
    delete mRenderOp.vertexData;
    delete mShaderCustomAutoConstants;
}

// ----------------------------------------------------------------------------
void GPU2DOperationRenderableQuad::setRenderRect(const Ogre::Rect& rect)
{
    mRenderRect = rect;
    mDirtyVertexBuffer = true;
}

// ----------------------------------------------------------------------------
const Ogre::Rect& GPU2DOperationRenderableQuad::getRenderRect() { return mRenderRect; }

// ----------------------------------------------------------------------------
void GPU2DOperationRenderableQuad::getWorldTransforms(Ogre::Matrix4* xform) const
{
    *xform = Ogre::Matrix4::IDENTITY;
}

// ----------------------------------------------------------------------------
const Ogre::Quaternion& GPU2DOperationRenderableQuad::getWorldOrientation(void) const
{
    return Ogre::Quaternion::IDENTITY;
}

// ----------------------------------------------------------------------------
const Ogre::Vector3& GPU2DOperationRenderableQuad::getWorldPosition(void) const
{
    return Ogre::Vector3::ZERO;
}

// ----------------------------------------------------------------------------
void GPU2DOperationRenderableQuad::setMaterialName(const string& materialName)
{
    if (mMaterialName.compare(materialName) != 0)
    {
        mMaterialName = materialName;
        mDirtyMaterialName = true;
    }
}

//-----------------------------------------------------------------------
void GPU2DOperationRenderableQuad::_updateCustomGpuParameter(
    const Ogre::GpuProgramParameters::AutoConstantEntry& constantEntry,
    Ogre::GpuProgramParameters* params) const
{
    if (mShaderCustomAutoConstants->updateShaderParameter(params, constantEntry))
    {
        Renderable::_updateCustomGpuParameter(constantEntry, params);
    }
}

//-----------------------------------------------------------------------
void GPU2DOperationRenderableQuad::_updateRenderQueue(Ogre::RenderQueue* queue)
{
    queue->addRenderable(this, mRenderQueueID, mZOrder);
}

// ----------------------------------------------------------------------------
int GPU2DOperationRenderableQuad::getMaterialShaderConstantIndex(const string& constantName, bool makeDirty)
{
    ShaderConstantNameToIndexMap::iterator mapIt = mShaderConstantNameToIndexMap.find(constantName);

    int constantIndex = -1;
    if (mapIt == mShaderConstantNameToIndexMap.end())
    {
        constantIndex = mShaderConstantNameToIndexMap.size();
        mShaderConstantNameToIndexMap.insert(ShaderConstantNameToIndexMap::value_type(
            constantName, std::pair<int, bool>(constantIndex, makeDirty)));
    }
    else
    {
        constantIndex = mapIt->second.first;
        if (makeDirty)
        {
            mapIt->second.second = makeDirty;
        }
    }

    return constantIndex;
}

// ----------------------------------------------------------------------------
void GPU2DOperationRenderableQuad::setMaterialShaderConstant(const string& constantName,
                                                             const Ogre::Real* data, int dataCount)
{
    int constantIndex = getMaterialShaderConstantIndex(constantName, true);

    if (constantIndex >= 0)
    {
        mShaderCustomAutoConstants->setConstant(constantIndex, data, dataCount);
    }
}

// ----------------------------------------------------------------------------
void GPU2DOperationRenderableQuad::setMaterialShaderConstant(const string& constantName,
                                                             const Ogre::Vector4& data)
{
    int constantIndex = getMaterialShaderConstantIndex(constantName, true);

    if (constantIndex >= 0)
    {
        mShaderCustomAutoConstants->setConstant(constantIndex, data);
    }
}

// ----------------------------------------------------------------------------
void GPU2DOperationRenderableQuad::setMaterialShaderConstant(const string& constantName,
                                                             const Ogre::Matrix4& data)
{
    int constantIndex = getMaterialShaderConstantIndex(constantName, true);

    if (constantIndex >= 0)
    {
        mShaderCustomAutoConstants->setConstant(constantIndex, data);
    }
}

//-----------------------------------------------------------------------
void GPU2DOperationRenderableQuad::setMaterialShaderConstantUVRect(const string& constantName,
                                                                   const Ogre::Rect& rect, bool flipX,
                                                                   bool flipY)
{
    Ogre::Vector4 uvTransform(1.0f / rect.width(), 1.0f / rect.height(),
                              (Ogre::Real)(-rect.left) / rect.width(),
                              (Ogre::Real)(-rect.top) / rect.height());

    if (flipX)
    {
        uvTransform.x = -uvTransform.x;
        uvTransform.z = 1.0f - uvTransform.z;
    }
    if (flipY)
    {
        uvTransform.y = -uvTransform.y;
        uvTransform.w = 1.0f - uvTransform.w;
    }

    setMaterialShaderConstant(constantName, uvTransform);
}

//-----------------------------------------------------------------------
void GPU2DOperationRenderableQuad::setMaterialTexture(const string& aliasName,
                                                      const Ogre::TexturePtr& texturePtr)
{
    string textureName = texturePtr->getName();

    mNewAliasTextureMap[aliasName] = textureName;
    mAliasTextureMap[aliasName] = textureName;
}

// ----------------------------------------------------------------------------
void GPU2DOperationRenderableQuad::prepareForRender(Ogre::ushort zOrder)
{
    mZOrder = zOrder;

    bool newMaterial = false;

    // Update material if necessary
    if (mDirtyMaterialName)
    {
        loadMaterial(mMaterialName);
        newMaterial = true;
        mDirtyMaterialName = false;
    }

    Ogre::Technique* technique = mMaterial->getBestTechnique();
    assert(technique);

    // Update material texture(s) if necessary
    if (newMaterial)
    {
        technique->applyTextureAliases(mAliasTextureMap, true);
    }
    else
    {
        technique->applyTextureAliases(mNewAliasTextureMap, true);
    }
    mNewAliasTextureMap.clear();

    // Check if all textures are unlocked
    for (Ogre::ushort i = 0; i < technique->getNumPasses(); ++i)
    {
        Ogre::Pass* pass = technique->getPass(i);
        Ogre::Pass::TextureUnitStateIterator tusIt = pass->getTextureUnitStateIterator();
        while (tusIt.hasMoreElements())
        {
            Ogre::TextureUnitState* unitState = tusIt.getNext();
            const string& textureName = unitState->getTextureName();
            if (textureName.length())
            {
                Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(textureName);
                assert(!texture->getBuffer()->isLocked());
            }
        }
    }

    // Update material shader constant(s) if necessary by binding the constant names to custom indices
    ShaderConstantNameToIndexMap::iterator nameIndexPairIt,
        nameIndexPairItEnd = mShaderConstantNameToIndexMap.end();
    for (nameIndexPairIt = mShaderConstantNameToIndexMap.begin(); nameIndexPairIt != nameIndexPairItEnd;
         ++nameIndexPairIt)
    {
        if (nameIndexPairIt->second.second || newMaterial)
        {
            Utils::bindShaderCustomAutoConstant(technique, nameIndexPairIt->second.first,
                                                nameIndexPairIt->first);
            nameIndexPairIt->second.second = false;
        }
    }

    // Update vertex buffer if necessary
    if (mDirtyVertexBuffer)
    {
        Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
        float* pPos = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

        *pPos++ = (Ogre::Real)mRenderRect.left;
        *pPos++ = (Ogre::Real)mRenderRect.top;
        *pPos++ = (Ogre::Real)mRenderRect.left;
        *pPos++ = (Ogre::Real)mRenderRect.bottom;
        *pPos++ = (Ogre::Real)mRenderRect.right;
        *pPos++ = (Ogre::Real)mRenderRect.top;
        *pPos++ = (Ogre::Real)mRenderRect.right;
        *pPos++ = (Ogre::Real)mRenderRect.bottom;

        vbuf->unlock();

        mDirtyVertexBuffer = false;
    }
}

// ----------------------------------------------------------------------------
void GPU2DOperationRenderableQuad::loadMaterial(const string& materialName)
{
    Ogre::MaterialPtr originalMaterial = Ogre::MaterialManager::getSingleton().getByName(materialName);
    if (originalMaterial.isNull())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find material " + mMaterialName,
                    "GPU2DOperationRenderableQuad::setMaterial");
    }

    originalMaterial->load();

    // originalMaterial->getBestTechnique()->getPass(0)->getFragmentProgram()->reload();
    originalMaterial->copyDetailsTo(mMaterial);
}
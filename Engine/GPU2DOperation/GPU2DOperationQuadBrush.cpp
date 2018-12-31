/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "GPU2DOperationQuadBrush.h"
#include "GPU2DOperationRenderableQuad.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
GPU2DOperationQuadBrush::GPU2DOperationQuadBrush(const string& materialName, int supportedInstanceCount)
    : mInnerRadius(0.0f), mOuterRadius(1.0f), mShapePower(1.0f), mDirtyBrushWeights(true),
      mActiveBrushInstances(0), mMinHeight(0.0f), mMaxHeight(1.0f),
      mSupportedInstanceCount(supportedInstanceCount)
{
    mGPU2DOperationableQuad->setMaterialName(materialName); //
    mBrushTransforms.reserve(getSupportedInstanceCount());
    mBrushDirections.reserve(getSupportedInstanceCount());
}

// ----------------------------------------------------------------------------
void GPU2DOperationQuadBrush::setInnerRadius(Ogre::Real innerRadius) { mInnerRadius = innerRadius; }

// ----------------------------------------------------------------------------
void GPU2DOperationQuadBrush::setOuterRadius(Ogre::Real outerRadius) { mOuterRadius = outerRadius; }

// ----------------------------------------------------------------------------
void GPU2DOperationQuadBrush::setBrushShapePower(Ogre::Real shapePower) { mShapePower = shapePower; }

// ----------------------------------------------------------------------------
void GPU2DOperationQuadBrush::setHeightRange(Ogre::Real minHeight, Ogre::Real maxHeight)
{
    mMinHeight = minHeight;
    mMaxHeight = maxHeight;
}

// ----------------------------------------------------------------------------
void GPU2DOperationQuadBrush::prepareForRender(Ogre::ushort zOrder)
{
    //	mInnerRadius = 60;
    //	mOuterRadius = 61;
    //	mShapePower = 1.0f;

    Ogre::Real innerPow = pow(mInnerRadius, 1.0f / mShapePower);
    Ogre::Real outerPow = pow(mOuterRadius, 1.0f / mShapePower);

    Ogre::Real brushMul = 1.0f / (innerPow - outerPow);
    Ogre::Real brushAdd = -outerPow * brushMul;
    Ogre::Real brushPow = 0.5f * 1.0f / mShapePower;

    if (mActiveBrushInstances > 0)
    {
        mGPU2DOperationableQuad->setMaterialShaderConstant(
            _T("brushShape"), Ogre::Vector4(brushAdd, brushMul, brushPow, mInnerRadius));
        mGPU2DOperationableQuad->setMaterialShaderConstant(
            _T("heightScale"), Ogre::Vector4(mMinHeight, mMaxHeight, mMaxHeight - mMinHeight, 0.0f));

        mBrushTransforms.resize(getSupportedInstanceCount());
        mBrushDirections.resize(getSupportedInstanceCount());
        for (int transformIndex = mActiveBrushInstances; transformIndex < getSupportedInstanceCount();
             ++transformIndex)
        {
            mBrushTransforms[transformIndex] = Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            mBrushDirections[transformIndex] = Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        }
        mGPU2DOperationableQuad->setMaterialShaderConstant(
            _T("brushTransform"), (Ogre::Real*)&mBrushTransforms[0], 4 * getSupportedInstanceCount());
        mGPU2DOperationableQuad->setMaterialShaderConstant(
            _T("brushDirection"), (Ogre::Real*)&mBrushDirections[0], 4 * getSupportedInstanceCount());
        for (int transformIndex = 0; transformIndex < mActiveBrushInstances; ++transformIndex)
        {
            mBrushTransforms[transformIndex] = Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            mBrushDirections[transformIndex] = Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    // std::cout << _T("ACTIVE INSTANCES: ") << mActiveBrushInstances << _T("\n");

    mActiveBrushInstances = 0;

    mGPU2DOperationableQuad->prepareForRender(zOrder);
}

// ----------------------------------------------------------------------------
int GPU2DOperationQuadBrush::getSupportedInstanceCount() { return mSupportedInstanceCount; }

// ----------------------------------------------------------------------------
void GPU2DOperationQuadBrush::setBrushInstance(int instanceIndex, const Ogre::Vector3& position,
                                               const Ogre::Vector3& direction, Ogre::Real strength)
{
    assert(instanceIndex < getSupportedInstanceCount());

    if (instanceIndex >= (int)mBrushTransforms.size())
    {
        mBrushTransforms.resize(instanceIndex + 1, Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f));
        mBrushDirections.resize(instanceIndex + 1, Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f));
    }

    mActiveBrushInstances = std::max(mActiveBrushInstances, instanceIndex + 1);

    //	std::cout << "strength: " << strength << "\n";

    //	strength = 0.01f;

    // string transformName = _T("brushTransform") + Ogre::StringConverter::toString(instanceIndex);
    // printf("%p%s: %f %f %f:   %f\n", this, transformName.c_str(), position.x, position.y, position.z,
    // strength);
    Ogre::Vector4 brushTransform = Ogre::Vector4(position.x, position.y, position.z, strength);
    Ogre::Vector4 brushDirection = Ogre::Vector4(direction.x, direction.z, direction.z, -direction.x);
    // mGPU2DOperationableQuad->setMaterialShaderConstant(transformName, Ogre::Vector4(position.x,
    // position.y, position.z, strength));

    mBrushTransforms[instanceIndex] = brushTransform;
    mBrushDirections[instanceIndex] = brushDirection;

    mDirtyBrushWeights = true;
}

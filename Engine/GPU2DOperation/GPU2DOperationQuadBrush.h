/**
 * ScapeEngine::GPU2DOperationQuad class
 *
 * 2D rectangle that can be added to a GPU2DOperation with a custom material and custom shader settings
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __GPU2DOPERATIONQUADBRUSH_H__
#define __GPU2DOPERATIONQUADBRUSH_H__

#include "GPU2DOperationQuadCustom.h"

namespace ScapeEngine {
    class GPU2DOperationQuadBrush : public GPU2DOperationQuadCustom
    {
    public:
        GPU2DOperationQuadBrush(const string& materialName, int supportedInstanceCount);

        virtual void prepareForRender(Ogre::ushort zOrder);

        virtual void setHeightRange(Ogre::Real minHeight, Ogre::Real maxHeight);

        virtual void setInnerRadius(Ogre::Real innerRadius);
        virtual void setOuterRadius(Ogre::Real outerRadius);
        virtual void setBrushShapePower(Ogre::Real shapePower);

        virtual int getSupportedInstanceCount();
        virtual void setBrushInstance(
            int instanceIndex, const Ogre::Vector3& position, const Ogre::Vector3& direction, Ogre::Real strength);

    protected:
        Ogre::Real mInnerRadius;
        Ogre::Real mOuterRadius;
        Ogre::Real mShapePower;
        Ogre::Real mMinHeight;
        Ogre::Real mMaxHeight;

        std::vector<Ogre::Vector4> mBrushTransforms;
        std::vector<Ogre::Vector4> mBrushDirections;
        int mActiveBrushInstances;
        int mSupportedInstanceCount;
        bool mDirtyBrushWeights;
    };

    typedef Ogre::SharedPtr<GPU2DOperationQuadBrush> GPU2DOperationQuadBrushPtr;
}

#endif // __GPU2DOPERATIONQUADBRUSH_H__

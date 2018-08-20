/**
 * ScapeEngine::HeightfieldOperationGPUNoiseDrag class
 *
 * A base class for all GPU-based operations operated like a noise tool
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONGPUNOISEDRAG_H__
#define __HEIGHTFIELDOPERATIONGPUNOISEDRAG_H__

#include "HeightfieldOperationBrushBase.h"
#include "HeightfieldOperationTaskPageGPU.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine {
    class HeightfieldGeom;
    class HeightfieldBufferPage;
    class HeightfieldOperationTaskPage;

    class HeightfieldOperationGPUNoiseDrag
        : public HeightfieldOperationTaskPageGPUListener,
          public HeightfieldOperationPolicyBrushInstancerSpacer<HeightfieldOperationGPUNoiseDrag,
              HeightfieldOperationPolicySchedulerTaskPageBatch<HeightfieldOperationGPUNoiseDrag,
                                                                    HeightfieldOperationBrushBase> >
    {
        DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationGPUNoiseDrag)
    public:
        DEFINE_FACTORYCLASS(HeightfieldOperationGPUNoiseDrag, HeightfieldOperation)

        HeightfieldOperationGPUNoiseDrag();

        HeightfieldOperationTaskPage* createTaskPage(
            const BrushPageCoords& pageCoords, const BrushInstances& brushInstances);

    protected:
        Ogre::Real mNoiseLargestFeat;
        Ogre::Real mNoiseCoarseRough;
        Ogre::Real mNoiseFineRough;
        Ogre::Real mNoisePeakify;
        Ogre::Real mNoiseSeed;
        Ogre::Real mNoiseBlend;
        Ogre::Real mNoiseAllowUp;
        Ogre::Real mNoiseAllowDown;

        Ogre::Real mDistortLargestFeat;
        Ogre::Real mDistortCoarseRough;
        Ogre::Real mDistortFineRough;
        Ogre::Real mDirectionStretchAngle;
        Ogre::Real mDirectionStretchRatio;
        Ogre::Real mDistortScaleCoarse;
        Ogre::Real mDistortScaleFine;
        int mDistortType;

        virtual bool initPersistentElementValueMap(StringStringMap& map);
        virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
        virtual string setUIElementPropertyValue(const string& elementName, const string& value);

        virtual void onSetupGPU2DOperationQuadBrush(GPU2DOperationQuadBrushPtr quadPtr);
        //		virtual void onSetupGPU2DOperationQuadCustom(GPU2DOperationQuadCustomPtr quadPtr);
    };
}

#endif // __HEIGHTFIELDOPERATIONGPUNOISEDRAG_H__

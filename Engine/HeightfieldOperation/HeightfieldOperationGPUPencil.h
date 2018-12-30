/**
 * ScapeEngine::HeightfieldOperationGPUPencil class
 *
 * A base class for all GPU-based operations operated like a brush tool
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONGPUPENCIL_H__
#define __HEIGHTFIELDOPERATIONGPUPENCIL_H__

#include "ScapeEnginePrerequisites.h"

#include "HeightfieldOperationBrushBase.h"
#include "HeightfieldOperationTaskPageGPU.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine
{

class HeightfieldOperationGPUPencil
    : public HeightfieldOperationTaskPageGPUListener,
      public HeightfieldOperationPolicyBrushInstancerSpacer<
          HeightfieldOperationGPUPencil, HeightfieldOperationPolicySchedulerTaskPageBatch<
                                             HeightfieldOperationGPUPencil, HeightfieldOperationBrushBase>>
{
    DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationGPUPencil)
public:
    DEFINE_FACTORYCLASS(HeightfieldOperationGPUPencil, HeightfieldOperation)

    HeightfieldOperationGPUPencil();

    HeightfieldOperationTaskPage* createTaskPage(const BrushPageCoords& pageCoords,
                                                 const BrushInstances& brushInstances);

    virtual void onSetupGPU2DOperationQuadBrush(GPU2DOperationQuadBrushPtr quadPtr);

protected:
    virtual bool initPersistentElementValueMap(StringStringMap& map);
    virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
    virtual string setUIElementPropertyValue(const string& elementName, const string& value);

    Ogre::Real mLevelEffect;
};
}

#endif // __HEIGHTFIELDOPERATIONGPUPENCIL_H__

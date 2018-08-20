/**
 * ScapeEngine::HeightfieldOperationGPUBrush class
 *
 * A base class for all GPU-based operations operated like a brush tool
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONGPUBRUSH_H__
#define __HEIGHTFIELDOPERATIONGPUBRUSH_H__

#include "HeightfieldOperationBrushBase.h"
#include "HeightfieldOperationTaskPageGPU.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine {
    class HeightfieldGeom;
    class HeightfieldBufferPage;
    class HeightfieldOperationTaskPage;

    class HeightfieldOperationGPUBrush
        : public HeightfieldOperationTaskPageGPUListener,
          public HeightfieldOperationPolicyBrushInstancerSpacer<HeightfieldOperationGPUBrush,
              HeightfieldOperationPolicySchedulerTaskPageBatch<HeightfieldOperationGPUBrush,
                                                                    HeightfieldOperationBrushBase> >
    {
        DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationGPUBrush)
    public:
        DEFINE_FACTORYCLASS(HeightfieldOperationGPUBrush, HeightfieldOperation)

        HeightfieldOperationGPUBrush();

        HeightfieldOperationTaskPage* createTaskPage(
            const BrushPageCoords& pageCoords, const BrushInstances& brushInstances);

    protected:
        virtual bool initPersistentElementValueMap(StringStringMap& map);
        virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
        virtual string setUIElementPropertyValue(const string& elementName, const string& value);

        bool mShowDebug;
    };
}

#endif // __HEIGHTFIELDOPERATIONGPUBRUSH_H__

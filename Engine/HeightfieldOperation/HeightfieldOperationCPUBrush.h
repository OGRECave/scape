/**
 * ScapeEngine::HeightfieldOperationCPUBrush class
 *
 * A base class for all CPU-based operations operated like a brush tool
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONCPUBRUSH_H__
#define __HEIGHTFIELDOPERATIONCPUBRUSH_H__

#include "HeightfieldOperationBrushBase.h"
//#include "HeightfieldOperationPolicySchedulerTaskDirect.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine {
    class HeightfieldGeom;

    class HeightfieldOperationCPUBrush
        : public HeightfieldOperationPolicyBrushInstancerSpacer<HeightfieldOperationCPUBrush,
              HeightfieldOperationPolicySchedulerTaskDirect<HeightfieldOperationCPUBrush,
                                                                    HeightfieldOperationBrushBase> >
    {
        DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationCPUBrush)
    public:
        DEFINE_FACTORYCLASS(HeightfieldOperationCPUBrush, HeightfieldOperation)

        HeightfieldOperationCPUBrush();

        virtual HeightfieldOperationTaskDirect* createTaskDirect(const BrushInstance& brushInstance);

        bool isLedgeMode() { return mLedgeMode; }
    protected:
        bool mLedgeMode;

        virtual bool initPersistentElementValueMap(StringStringMap& map);
        virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
        virtual string setUIElementPropertyValue(const string& elementName, const string& value);
    };
}

#endif // __HEIGHTFIELDOPERATIONCPUBRUSH_H__

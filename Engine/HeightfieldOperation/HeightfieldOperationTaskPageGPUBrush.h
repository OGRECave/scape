/**
 * ScapeEngine::HeightfieldOperationTaskPageGPUBrush class
 *
 * The HeightfieldBufferPage GPU processor class, controlled by HeightfieldOperationBrushBase
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONTASKPAGEGPUBRUSH_H__
#define __HEIGHTFIELDOPERATIONTASKPAGEGPUBRUSH_H__

#include "ScapeEnginePrerequisites.h"

#include "HeightfieldOperationTaskPageGPU.h"
//#include "HeightfieldOperationGPU.h"
#include "GPU2DOperation/GPU2DOperation.h"
#include "GPU2DOperation/GPU2DOperationQuadBrush.h"

namespace ScapeEngine
{
class HeightfieldBuffer;

class HeightfieldOperationTaskPageGPUBrush : public HeightfieldOperationTaskPageGPU
{
public:
    HeightfieldOperationTaskPageGPUBrush(const string& brushMaterialName, const string& blendMaterialName,
                                         int supportedBrushInstanceCount, bool onePass,
                                         HeightfieldOperationTaskPageGPUListener* quadListener,
                                         HeightfieldOperationBrush* operation,
                                         const BrushPageCoords pageCoords,
                                         const BrushInstances& brushInstances);

    virtual void setBorderSize(int width, int height);

protected:
    GPU2DOperationPtr mCurrentGPU2DOperationPtr;
    GPU2DOperationPtr mPreviousGPU2DOperationPtr;
    string mBrushMaterialName;
    string mBlendMaterialName;
    int mBorderWidth;
    int mBorderHeight;
    int mSupportedBrushInstanceCount;
    bool mMaskDone;
    bool mOnePass;
    virtual void tickPending();
    virtual void tickActive();

    void setOperationQuadNeighbor(GPU2DOperationQuadCustomPtr& quadPtr, const string& constantCoordName,
                                  const string& textureAliasName, int deltaX, int deltaY);
};
}

#endif // __HEIGHTFIELDOPERATIONTASKPAGEGPUBRUSH_H__

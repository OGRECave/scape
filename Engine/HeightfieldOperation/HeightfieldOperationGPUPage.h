/**
 * ScapeEngine::HeightfieldOperationGPUPage class
 *
 * The HeightfieldBufferPage GPU processor class, controlled by HeightfieldOperationBrushBase
 *
 * G.J.P. de Carpentier, 2007 
 */


#ifndef __HEIGHTFIELDOPERATIONGPUPAGE_H__
#define __HEIGHTFIELDOPERATIONGPUPAGE_H__

#include "HeightfieldOperationTaskPage.h"
//#include "HeightfieldOperationGPU.h"
#include "GPU2DOperation/GPU2DOperation.h"
#include "GPU2DOperation/GPU2DOperationQuadBrush.h"

namespace ScapeEngine
{
	class HeightfieldBuffer;

	class HeightfieldOperationGPUPage : public HeightfieldOperationTaskPage
	{
	public:

		HeightfieldOperationGPUPage(HeightfieldOperationBrush* operation, const BrushPageCoords pageCoords, const BrushInstances& brushInstances);

	protected:

		GPU2DOperationPtr mCurrentGPU2DOperationPtr;
		GPU2DOperationPtr mPreviousGPU2DOperationPtr;
		GPU2DOperationQuadBrushPtr mQuadPtr;

		virtual void tickPending();
		virtual void tickActive();

	};
}

#endif // __HEIGHTFIELDOPERATIONGPUPAGE_H__

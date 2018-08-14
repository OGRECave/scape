/**
 * ScapeEngine::HeightfieldOperationTaskPageGPU class
 *
 * The HeightfieldBufferPage GPU processor class, controlled by HeightfieldOperationBrushBase
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDOPERATIONTASKPAGEGPU_H__
#define __HEIGHTFIELDOPERATIONTASKPAGEGPU_H__

#include "HeightfieldOperationTaskPage.h"
//#include "HeightfieldOperationGPU.h"
#include "GPU2DOperation/GPU2DOperation.h"
#include "GPU2DOperation/GPU2DOperationQuadBrush.h"
#include "GPU2DOperation/GPU2DOperationQuadCustom.h"

namespace ScapeEngine
{
	class HeightfieldBuffer;

	class HeightfieldOperationTaskPageGPUListener
	{
	public:
		virtual void onSetupGPU2DOperationQuadBrush(GPU2DOperationQuadBrushPtr quadPtr) {}
		virtual void onSetupGPU2DOperationQuadCustom(GPU2DOperationQuadCustomPtr quadPtr) {}
	};

	class HeightfieldOperationTaskPageGPU : public HeightfieldOperationTaskPage
	{
	public:

		HeightfieldOperationTaskPageGPU(HeightfieldOperationTaskPageGPUListener* quadListener, HeightfieldOperationBrush* operation, const BrushPageCoords pageCoords, const BrushInstances& brushInstances)
		:	HeightfieldOperationTaskPage(operation, pageCoords, brushInstances),
			mQuadListener(quadListener) 
		{}

	protected:
		HeightfieldOperationTaskPageGPUListener* mQuadListener;
	};
}

#endif // __HEIGHTFIELDOPERATIONTASKPAGEGPU_H__

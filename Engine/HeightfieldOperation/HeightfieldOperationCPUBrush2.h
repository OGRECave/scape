/**
 * ScapeEngine::HeightfieldOperationCPUBrush2 class
 *
 * Page-based cpu pencil brush 
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDOPERATIONCPUBRUSH2_H__
#define __HEIGHTFIELDOPERATIONCPUBRUSH2_H__

#include "HeightfieldOperationBrushBase.h"
//#include "HeightfieldOperationPolicySchedulerTaskDirect.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine
{
	class HeightfieldGeom;

	class HeightfieldOperationCPUBrush2
	: 	public HeightfieldOperationPolicyBrushInstancerSpacer<HeightfieldOperationCPUBrush2, 
			   HeightfieldOperationPolicySchedulerTaskPageBatch<HeightfieldOperationCPUBrush2, HeightfieldOperationBrushBase> >
	{
		DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationCPUBrush2)
	public:
		DEFINE_FACTORYCLASS(HeightfieldOperationCPUBrush2, HeightfieldOperation)

		HeightfieldOperationCPUBrush2();

		//virtual HeightfieldOperationTaskDirect* createTaskDirect(const BrushInstance& brushInstance);

		virtual HeightfieldOperationTaskPage* createTaskPage(const BrushPageCoords& pageCoords, const BrushInstances& brushInstances);


		bool isLedgeMode() {return mLedgeMode;}
	protected:

		bool mLedgeMode;

		virtual bool initPersistentElementValueMap(StringStringMap& map);
		virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
		virtual string setUIElementPropertyValue(const string& elementName, const string& value);
	};
}

#endif // __HEIGHTFIELDOPERATIONCPUBRUSH2_H__

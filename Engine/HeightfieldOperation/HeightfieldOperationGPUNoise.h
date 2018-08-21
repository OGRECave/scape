/**
 * ScapeEngine::HeightfieldOperationGPUNoise class
 *
 * A base class for all GPU-based operations operated like a noise tool
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDOPERATIONGPUNOISE_H__
#define __HEIGHTFIELDOPERATIONGPUNOISE_H__

#include "HeightfieldOperationBrushBase.h"
#include "HeightfieldOperationTaskPageGPU.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine
{
	class HeightfieldGeom;
	class HeightfieldBufferPage;
	class HeightfieldOperationTaskPage;

	class HeightfieldOperationGPUNoise
	: 	public HeightfieldOperationTaskPageGPUListener,
	 	public HeightfieldOperationPolicyBrushInstancerSpacer<HeightfieldOperationGPUNoise, 
			   HeightfieldOperationPolicySchedulerTaskPageBatch<HeightfieldOperationGPUNoise, HeightfieldOperationBrushBase> >
	{
		DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationGPUNoise)
	public:
		DEFINE_FACTORYCLASS(HeightfieldOperationGPUNoise, HeightfieldOperation)

		HeightfieldOperationGPUNoise();

		HeightfieldOperationTaskPage* createTaskPage(const BrushPageCoords& pageCoords, const BrushInstances& brushInstances);

	protected:

		Ogre::Real mNoiseLargestFeat;
		Ogre::Real mNoiseCoarseRough;
		Ogre::Real mNoiseFineRough;
		Ogre::Real mNoiseStretchAngle;
		Ogre::Real mNoiseStretchRatio;
		Ogre::Real mNoisePeakify;
		Ogre::Real mNoiseSeed;
		int mNoiseType;
		Ogre::Real mNoiseBlend;

		Ogre::Real mDistortLargestFeat;
		Ogre::Real mDistortCoarseRough;
		Ogre::Real mDistortFineRough;
		Ogre::Real mDistortStretchAngle;
		Ogre::Real mDistortStretchRatio;
		Ogre::Real mDistortScaleCoarse;
		Ogre::Real mDistortScaleFine;
		int mDistortType;

		virtual bool initPersistentElementValueMap(StringStringMap& map);
		virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
		virtual string setUIElementPropertyValue(const string& elementName, const string& value);

		virtual void onSetupGPU2DOperationQuadCustom(GPU2DOperationQuadCustomPtr quadPtr);
	};
}

#endif // __HEIGHTFIELDOPERATIONGPUNOISE_H__

/**
 * ScapeEngine::HeightfieldOperationTaskDirect class
 *
 * The HeightfieldBufferDirect processor class, controlled by HeightfieldOperation
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDOPERATIONDIRECT_H__
#define __HEIGHTFIELDOPERATIONDIRECT_H__

#include "HeightfieldOperationBrush.h"

namespace ScapeEngine
{
	class HeightfieldBuffer;

	class HeightfieldOperationTaskDirect
	{
	public:

		HeightfieldOperationTaskDirect(HeightfieldOperationBrush* operation, const BrushInstance& brushInstance);
		virtual ~HeightfieldOperationTaskDirect();

		//bool addBrushInstances(const BrushInstances& brushInstances);

		virtual void tick();

		bool isActive() {return mCurrentState == STATE_ACTIVE;}
		bool isPending() {return mCurrentState == STATE_PENDING;}
		bool isCompleted() {return mCurrentState == STATE_COMPLETED;}

		const Ogre::Rect& getUpdatedRect() {return mUpdatedRect;}

	protected:

		enum State
		{
			STATE_PENDING,
			STATE_ACTIVE,
			STATE_COMPLETED,
		} mCurrentState;

		Ogre::Rect mUpdatedRect;

		BrushInstance mBrushInstance;
		HeightfieldOperationBrush* mOperation;
		Ogre::Real mStrength;

		virtual void tickPending() = 0;
		virtual void tickActive() = 0;

	};
}

#endif // __HEIGHTFIELDOPERATIONPAGE_H__

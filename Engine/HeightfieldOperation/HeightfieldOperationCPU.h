/**
 * ScapeEngine::HeightfieldOperationCPU class
 *
 * A base class for all CPU-based operations operated like a brush tool
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDOPERATIONCPU_H__
#define __HEIGHTFIELDOPERATIONCPU_H__

#include "HeightfieldOperationBrush.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine
{
	class HeightfieldOperationCPU : public HeightfieldOperationBrush
	{
	public:

		virtual ~HeightfieldOperationCPU();

		virtual bool canInitiate(EState state);
		virtual bool isActive();
		virtual bool isActive(EState state);
		virtual bool isDone(EState state);
		virtual void initiate(EState state, HeightfieldBuffer* inOutHeightfieldBuffer);
		virtual void tick();

		virtual HeightfieldBuffer* getInOutHeightfieldBuffer() {return NULL;}
		virtual HeightfieldBuffer* getTempHeightfieldBuffer() {return NULL;}

	protected:

		EState mState;
		bool mIsStateActive;
		bool mHasBeenStateActive;

		class HeightfieldBuffer* mOriginalHeightfieldBuffer;
		HeightfieldBuffer* mInOutHeightfieldBuffer;
		Utils::SerialMemoryBufferPtr mDeltaBufferPtr;

		string mBrushName;
		class HeightfieldGeom* mHeightfieldGeom;

		HeightfieldOperationCPU();

		struct BrushPathNode
		{
			Ogre::Vector3 position;
			Ogre::Real pressure;
			unsigned long time;
			bool primary;
		};

		typedef std::list<BrushPathNode> BrushPathNodes;
		BrushPathNodes mBrushPathNodes;
		Ogre::Real mPathNextParam;

		Ogre::Real mPathSpacing;
		bool mPencilMode;

		virtual void initiateDo();
		virtual void initiateUndo();
		virtual void initiateRedo();
		virtual void tickDo();
		virtual void tickUndo();
		virtual void tickRedo();
		virtual void tickInactive();
		virtual void finishDo();

		virtual void applyPrimary(const Ogre::Vector3& position, Ogre::Real strength) = 0;
		virtual void applySecondary(const Ogre::Vector3& position, Ogre::Real strength) = 0;

	};
}

#endif // __HEIGHTFIELDOPERATIONCPU_H__

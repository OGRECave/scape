/**
 * ScapeEngine::HeightfieldOperationBrushInstancr class
 *
 * An abstract super class for brush instances policies for HeightfieldOperations
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDOPERATIONPOLICYBRUSHINSTANCER_H__
#define __HEIGHTFIELDOPERATIONPOLICYBRUSHINSTANCER_H__

#include "HeightfieldOperationBrush.h"

namespace ScapeEngine
{
	class HeightfieldBrush;

	typedef struct	
	{
		Ogre::Vector3 position;
		Ogre::Real pressure;
		unsigned long time;
		bool primary;
	} BrushPathNode;
	typedef std::list<BrushPathNode> BrushPathNodes;

	class HeightfieldOperationPolicyBrushInstancer
	{
	public:
		virtual ~HeightfieldOperationPolicyBrushInstancer() {}

	protected:
		HeightfieldOperationPolicyBrushInstancer() {}

		virtual void brushInstancerTick() = 0;

		virtual size_t getBrushInstanceCount() {return mBrushInstances.size();}
		virtual BrushInstance popBrushInstance() {assert(!mBrushInstances.empty()); BrushInstance instance = *mBrushInstances.begin(); mBrushInstances.pop_front(); return instance;}

	protected:
		BrushPathNodes mBrushPathNodes;
		BrushInstances mBrushInstances;
	};
}

#endif // __HEIGHTFIELDOPERATIONPOLICYBRUSHINSTANCER_H__

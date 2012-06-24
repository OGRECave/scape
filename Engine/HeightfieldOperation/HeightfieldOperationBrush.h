/**
 * ScapeEngine::HeightfieldOperation class
 *
 * An abstract super class for an HeightfieldPage operation
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDOPERATIONBRUSH_H__
#define __HEIGHTFIELDOPERATIONBRUSH_H__

#include "HeightfieldOperation.h"

namespace ScapeEngine
{
	class HeightfieldBrush;
	class HeightfieldOperationTaskPage;
	class HeightfieldOperationTaskDirect;

	struct BrushInstance
	{
		Ogre::Vector3 position;
		Ogre::Vector3 direction;
		Ogre::Real strength;
		Ogre::Real outerRadius;
		Ogre::Real innerRadius;
		Ogre::Real rampPower;
	};
	typedef std::list<BrushInstance> BrushInstances;
	typedef std::pair<int, int> BrushPageCoords; 
	typedef std::map<BrushPageCoords, BrushInstances> BrushInstancesGrid; 
	typedef std::list<HeightfieldOperationTaskPage*> HeightfieldOperationTaskPageList;
	typedef std::list<HeightfieldOperationTaskDirect*> HeightfieldOperationTaskDirectList;


	class HeightfieldOperationBrush : public HeightfieldOperation
	{
	public:
		virtual HeightfieldBrush* getBrush() {return mBrush;}
		virtual Ogre::Real getStrength() {return mStrength;}

	protected:
		class HeightfieldBrush* mBrush;
		Ogre::Real mStrength;

		HeightfieldOperationBrush() : mBrush(NULL), mStrength(1.0f) {}
	};
}

#endif // __HEIGHTFIELDOPERATIONBRUSH_H__

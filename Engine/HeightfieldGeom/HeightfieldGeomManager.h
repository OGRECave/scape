/**
 * ScapeEngine::HeightfieldGeomManager class
 *
 * Manager of HeightfieldGeom objects
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDGEOMMANAGER_H__
#define __HEIGHTFIELDGEOMMANAGER_H__

#include "EngineCore/Tickable.h"

namespace ScapeEngine
{
	class HeightfieldGeom;

	class HeightfieldGeomManager : public Tickable
	{
	public:

		HeightfieldGeomManager();

		HeightfieldGeom* create(class HeightfieldBuffer* heightfieldBuffer, Ogre::SceneNode* rootNode);

		bool getClosestHeightfieldGeomRayIntersection(const Ogre::Ray &ray, const Ogre::SceneManager& sceneManager, class HeightfieldGeom*& outHeightfieldGeom, Ogre::Vector3& outPoint);

		HeightfieldGeom* getCurrentHeightfieldGeom() const {return mCurrentHeightfieldGeom; }

		void destroy(class HeightfieldGeom* heightfieldGeom);

		void onPostFrameTick();

	private:

		typedef std::list<HeightfieldGeom*> HeightfieldGeoms;
		HeightfieldGeom* mCurrentHeightfieldGeom;
		HeightfieldGeoms mHeightfieldGeoms;
	};

}

#endif // __HEIGHTFIELDGEOMMANAGER_H__
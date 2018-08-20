/**
 * ScapeEngine::HeightfieldBufferSetManager class
 *
 * Manages HeightfieldBufferSet objects
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#ifndef __HEIGHTFIELDBUFFERSETMANAGER_H__
#define __HEIGHTFIELDBUFFERSETMANAGER_H__

#include "HeightfieldBufferSet.h"

namespace ScapeEngine
{
	class HeightfieldBufferSetManager
	{
	public:
		HeightfieldBufferSetManager();

		~HeightfieldBufferSetManager();

		// Add a new HeightfieldBuffer object to the set and return its handle
		HeightfieldBufferSetHandle create(const string& name, int elementColumnCount, int elementRowCount, Ogre::Real heightRangeMin, Ogre::Real heightRangeMax, int elementColumnCountPerPage, int elementRowCountPerPage);

		HeightfieldBufferSetHandle findHandle(const string& name);

		bool destroy(HeightfieldBufferSetHandle handle);

		HeightfieldBufferSet* get(HeightfieldBufferSetHandle handle);

		void unlockAll();
		
	private:

		typedef std::map<HeightfieldBufferSetHandle, HeightfieldBufferSet*> HeightfieldBufferSetMap;

		HeightfieldBufferSetMap mHeightfieldBufferSetMap;
	};

}

#endif // __HEIGHTFIELDBUFFERSETMANAGER_H__

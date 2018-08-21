/**
 * ScapeEngine::HeightfieldGeomTileIndexBufferManager class
 *
 * Manages vertex buffer streams representing the UV coordinate vertex elements
 * of a regular grid of requested sizes and types
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDGEOMTILEINDEXBUFFERMANAGER_H__
#define __HEIGHTFIELDGEOMTILEINDEXBUFFERMANAGER_H__

#include "EngineCore/Tickable.h"

namespace ScapeEngine
{
	namespace HeightfieldGeomTileIndexBufferManagerNS
	{
		struct CachedBufferDescription;
		struct CachedBufferReference;
	}


	class HeightfieldGeomTileIndexBufferManager : public Tickable
	{
	public:
		HeightfieldGeomTileIndexBufferManager();

		~HeightfieldGeomTileIndexBufferManager();

		Ogre::HardwareIndexBufferSharedPtr getBuffer(
			int quadColumnCount, 
			int quadRowCount, 
			int spacingNorth, 
			int spacingEast,
			int spacingSouth,
			int spacingWest
		);

		float getBufferTimeToLive() const {return mBufferTimeToLive;}

		void setBufferTimeToLive(float timeToLive) {mBufferTimeToLive = timeToLive;}

		void onPostFrameTick();

	private:

		typedef std::map <
			HeightfieldGeomTileIndexBufferManagerNS::CachedBufferDescription, 
			HeightfieldGeomTileIndexBufferManagerNS::CachedBufferReference
		> CachedBuffers;

		CachedBuffers *mCachedBuffers;

		float mBufferTimeToLive;

		template<class IndexType>
		Ogre::HardwareIndexBufferSharedPtr createBuffer(
			Ogre::HardwareIndexBuffer::IndexType type, 
			int quadColumnCount, 
			int quadRowCount, 
			int spacingNorth, 
			int spacingEast,
			int spacingSouth,
			int spacingWest
		);
	};
}

#endif // __HEIGHTFIELDGEOMTILEINDEXBUFFERMANAGER_H__
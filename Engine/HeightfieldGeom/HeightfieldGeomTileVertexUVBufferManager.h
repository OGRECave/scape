/**
 * ScapeEngine::HeightfieldGeomTileVertexUVBufferManager class
 *
 * Manages vertex buffer streams representing the UV coordinate vertex elements
 * of a regular grid of requested sizes and types
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDGEOMTILEVERTEXUVBUFFERMANAGER_H__
#define __HEIGHTFIELDGEOMTILEVERTEXUVBUFFERMANAGER_H__

#include "EngineCore/Tickable.h"

namespace ScapeEngine
{
namespace HeightfieldGeomTileVertexUVBufferManagerNS
{
struct CachedBufferDescription;
struct CachedBufferReference;
}

class HeightfieldGeomTileVertexUVBufferManager : public Tickable
{
public:
    HeightfieldGeomTileVertexUVBufferManager();

    ~HeightfieldGeomTileVertexUVBufferManager();

    Ogre::HardwareVertexBufferSharedPtr getBuffer(Ogre::VertexElementType type,
                                                  const Ogre::Rect& visibleQuadRect,
                                                  int templateQuadColumnCount, int templateQuadRowCount,
                                                  int quadColumnSpacing, int quadRowSpacing);

    Ogre::Real getBufferTimeToLive() const { return mBufferTimeToLive; }

    void setBufferTimeToLive(Ogre::Real timeToLive) { mBufferTimeToLive = timeToLive; }

    void onPostFrameTick();

private:
    typedef std::map<HeightfieldGeomTileVertexUVBufferManagerNS::CachedBufferDescription,
                     HeightfieldGeomTileVertexUVBufferManagerNS::CachedBufferReference>
        CachedBuffers;

    CachedBuffers* mCachedBuffers;

    Ogre::Real mBufferTimeToLive;

    Ogre::HardwareVertexBufferSharedPtr createBuffer(Ogre::VertexElementType type,
                                                     const Ogre::Rect& visibleQuadRect,
                                                     int templateQuadColumnCount, int templateQuadRowCount,
                                                     int quadColumnSpacing, int quadRowSpacing);
};
}

#endif // __HEIGHTFIELDGEOMTILEVERTEXUVBUFFERMANAGER_H__
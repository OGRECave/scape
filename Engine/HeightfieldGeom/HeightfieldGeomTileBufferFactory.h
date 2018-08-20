/**
 * Index and vertex buffer construction factory
 *
 * Creates hardware buffers filled with indices, UVs and heightvalues to be used by HeightfieldGeomTiles
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDGEOMTILEBUFFERFACTORY_H__
#define __HEIGHTFIELDGEOMTILEBUFFERFACTORY_H__

#include "HeightfieldGeomTile.h"

namespace ScapeEngine {
    // ----------------------------------------------------------------------------
    class HeightfieldGeomTileBufferFactory
    {
    public:
        static Ogre::HardwareVertexBufferSharedPtr createVertexHeightBuffer(HeightfieldGeomTile* heightfieldGeomTile);

        static Ogre::HardwareVertexBufferSharedPtr createVertexUVBuffer(HeightfieldGeomTile* heightfieldGeomTile);

        static Ogre::HardwareIndexBufferSharedPtr createIndexBuffer(HeightfieldGeomTile* heightfieldGeomTile);

    private:
        template <class PageDataType, class VertexDataType>
        static Ogre::HardwareVertexBufferSharedPtr _createHeightBuffer(HeightfieldGeomTile* heightfieldGeomTile);
    };
}

#endif // __HEIGHTFIELDGEOMTILEBUFFERFACTORY_H__

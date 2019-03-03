#ifndef __HEIGHTFIELDMANAGER_H__
#define __HEIGHTFIELDMANAGER_H__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{

class HeightfieldManager
{
public:
    HeightfieldManager();
    ~HeightfieldManager();

    HeightfieldGeomTileVertexUVBufferManager* getHeightfieldGeomTileVertexUVBufferManager() const;

    HeightfieldGeomTileIndexBufferManager* getHeightfieldGeomTileIndexBufferManager() const;

    HeightfieldBufferSetManager* getHeightfieldBufferSetManager() const;

    HeightfieldGeomManager* getHeightfieldGeomManager() const;

    void initialize();
    void resetHeightfield();

private:
    HeightfieldGeomTileVertexUVBufferManager* mHeightfieldGeomTileVertexUVBufferManager;
    HeightfieldGeomTileIndexBufferManager* mHeightfieldGeomTileIndexBufferManager;
    HeightfieldBufferSetManager* mHeightfieldBufferSetManager;
    HeightfieldGeomManager* mHeightfieldGeomManager;
};
}

#endif // __HEIGHTFIELDMANAGER_H__

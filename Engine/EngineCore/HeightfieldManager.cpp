#include "HeightfieldManager.h"

#include "ScapeEngineStableHeaders.h"

#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldBuffer/HeightfieldBufferSetManager.h"

#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldGeom/HeightfieldGeomManager.h"
#include "HeightfieldGeom/HeightfieldGeomTileIndexBufferManager.h"
#include "HeightfieldGeom/HeightfieldGeomTileVertexUVBufferManager.h"

namespace ScapeEngine
{

HeightfieldManager::HeightfieldManager()
    : mHeightfieldGeomTileVertexUVBufferManager(NULL),
      mHeightfieldGeomTileIndexBufferManager(NULL), mHeightfieldBufferSetManager(NULL),
      mHeightfieldGeomManager(NULL)
{
    mHeightfieldGeomTileVertexUVBufferManager = new HeightfieldGeomTileVertexUVBufferManager();
    mHeightfieldGeomTileIndexBufferManager = new HeightfieldGeomTileIndexBufferManager();
    mHeightfieldBufferSetManager = new HeightfieldBufferSetManager();
    mHeightfieldGeomManager = new HeightfieldGeomManager();
}

HeightfieldManager::~HeightfieldManager()
{
    SAFE_DELETE(mHeightfieldBufferSetManager);

    mHeightfieldGeomManager = NULL;                   // tickable
    mHeightfieldGeomTileIndexBufferManager = NULL;    // tickable
    mHeightfieldGeomTileVertexUVBufferManager = NULL; // tickable
}

HeightfieldGeomTileVertexUVBufferManager*
HeightfieldManager::getHeightfieldGeomTileVertexUVBufferManager() const
{
    return mHeightfieldGeomTileVertexUVBufferManager;
}

HeightfieldGeomTileIndexBufferManager* HeightfieldManager::getHeightfieldGeomTileIndexBufferManager() const
{
    return mHeightfieldGeomTileIndexBufferManager;
}

HeightfieldBufferSetManager* HeightfieldManager::getHeightfieldBufferSetManager() const
{
    return mHeightfieldBufferSetManager;
}

HeightfieldGeomManager* HeightfieldManager::getHeightfieldGeomManager() const
{
    return mHeightfieldGeomManager;
}
}

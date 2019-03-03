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

    HeightfieldBrushManager* getHeightfieldBrushManager() const;

    HeightfieldOperationFactory* getHeightfieldOperationFactory() const;

    HeightfieldOperationStack* getHeightfieldOperationStack() const;

    GPU2DOperationManager* getGPU2DOperationManager() const;

    GPU2DOperationRenderableQuadManager* getGPU2DOperationRenderableQuadManager() const;

    HeightfieldFileEncoderManager* getHeightfieldFileEncoderManager() const;

    HeightfieldFileDecoderManager* getHeightfieldFileDecoderManager() const;

    void initialize();
    void resetHeightfield();
    void createGeometry();

private:
    bool mCreatedGeometry;

    HeightfieldGeomTileVertexUVBufferManager* mHeightfieldGeomTileVertexUVBufferManager;
    HeightfieldGeomTileIndexBufferManager* mHeightfieldGeomTileIndexBufferManager;
    HeightfieldBufferSetManager* mHeightfieldBufferSetManager;
    HeightfieldGeomManager* mHeightfieldGeomManager;

    HeightfieldOperationFactory* mHeightfieldOperationFactory;

    HeightfieldOperationStack* mHeightfieldOperationStack;

    HeightfieldBrushManager* mHeightfieldBrushManager;

    GPU2DOperationManager* mGPU2DOperationManager;

    GPU2DOperationRenderableQuadManager* mGPU2DOperationRenderableQuadManager;

    HeightfieldFileEncoderManager* mHeightfieldFileEncoderManager;
    HeightfieldFileDecoderManager* mHeightfieldFileDecoderManager;
};
}

#endif // __HEIGHTFIELDMANAGER_H__

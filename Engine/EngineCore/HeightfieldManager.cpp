#include "HeightfieldManager.h"

#include "ScapeEngineStableHeaders.h"

#include "SettingsDatasetManager.h"
#include "SettingsDataset.h"

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

void HeightfieldManager::initialize()
{
    // create the heightfield buffer with the dimensions specified in Startup.conf
    int columns = 0;
    int rows = 0;
    Ogre::Real height = 0;
    SettingsDataset* startupDataset = getEngineCore()->getSettingsDatasetManager()->getDataset("Startup");
    if (startupDataset)
    {
        columns = Ogre::StringConverter::parseInt(startupDataset->getSetting("Heightfield", "", "columns"));
        rows = Ogre::StringConverter::parseInt(startupDataset->getSetting("Heightfield", "", "rows"));
        height = Ogre::StringConverter::parseReal(startupDataset->getSetting("Heightfield", "", "height"));
    }

    if (columns <= 0)
        columns = 4096;
    if (rows <= 0)
        rows = 4096;
    if (height <= 0.0f)
        height = 500.0f;

    mHeightfieldBufferSetManager->create("Project", columns, rows, 0, height, 512, 512);

    HeightfieldBufferSetHandle handle = mHeightfieldBufferSetManager->findHandle("Project");
    HeightfieldBufferSet* heightfieldBufferSet = mHeightfieldBufferSetManager->get(handle);
    heightfieldBufferSet->create("Render", Ogre::PF_SHORT_L);
}

void HeightfieldManager::resetHeightfield()
{
    HeightfieldBufferSetHandle handle = mHeightfieldBufferSetManager->findHandle("Project");
    HeightfieldBufferSet* heightfieldBufferSet = mHeightfieldBufferSetManager->get(handle);
    HeightfieldBufferHandle heightfieldBufferHandle = heightfieldBufferSet->findHandle("Render");
    HeightfieldBuffer* heightfieldBuffer = heightfieldBufferSet->get(heightfieldBufferHandle);

    for (int pageRowIndex = 0;
         pageRowIndex < heightfieldBuffer->getHeightfieldBufferSet()->getPageRowCount(); ++pageRowIndex)
    {
        for (int pageColumnIndex = 0;
             pageColumnIndex < heightfieldBuffer->getHeightfieldBufferSet()->getPageColumnCount();
             ++pageColumnIndex)
        {
            HeightfieldBufferPage* page = heightfieldBuffer->getPage(
                pageColumnIndex, pageRowIndex, HeightfieldBuffer::PAGEACCESSMODE_WRITE_DISCARD);
            Ogre::Real* data =
                new Ogre::Real[heightfieldBuffer->getHeightfieldBufferSet()
                                   ->getElementColumnCountPerPage() *
                               heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCountPerPage()];
            Ogre::PixelBox pixelBox(
                heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCountPerPage(),
                heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCountPerPage(), 1,
                Ogre::PF_FLOAT32_R, data);

            // int relX = pageColumnIndex * heightfieldBuffer->getElementColumnCountPerPage();
            // int relY = pageRowIndex * heightfieldBuffer->getElementRowCountPerPage();

            for (int y = 0; y < heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCountPerPage();
                 ++y)
            {
                for (int x = 0;
                     x < heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCountPerPage(); ++x)
                {
                    data[heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCountPerPage() * y +
                         x] = 0.0f;
                    /*
                                                                                                    float
                       power = 0.05f;
                                                                                                    for (int
                       level = 11; level >= 0; level--)
                                                                                                    {
                                                                                                                    int spacing = (255 << level);
                                                                                                                    int invmask = spacing - 255;
                                                                                                                    int mask = ~invmask;
                                                                                                                    int px = (x + relX) & mask;
                                                                                                                    int py = ((y + relY) & mask) * 13;
                                                                                                                    int rpx = Utils::randHash(px);
                                                                                                                    int rpy = Utils::randHash(py);
                                                                                                                    int rpx2 = Utils::randHash(px +
                       spacing);
                                                                                                                    int rpy2 = Utils::randHash(py + spacing
                       * 13);
                                                                                                                    int h1 = Utils::randHash(rpx + rpy);
                                                                                                                    int h2 = Utils::randHash(rpx2 + rpy);
                                                                                                                    int h3 = Utils::randHash(rpx + rpy2);
                                                                                                                    int h4 = Utils::randHash(rpx2 + rpy2);
                                                                                                                    int wx = ((x + relX) & invmask) * 4096 /
                       spacing;
                                                                                                                    int wy = ((y + relY) & invmask) * 4096 /
                       spacing;
                                                                                                                    int hy1 = ((h1 << 12) + (h2 - h1) * wx)
                       >> 12;
                                                                                                                    int hy2 = ((h3 << 12) + (h4 - h3) * wx)
                       >> 12;
                                                                                                                    int h = ((hy1 << 12) + (hy2 - hy1) * wy)
                       >> 12;

                                                                                                                    float height = power * (h / 4096.0f -
                       0.5f);
                                                                                                                    power = power * 0.45f + 0.0000025f;
                                                                                                                    data[heightfieldBuffer->getElementRowCountPerPage()*y+x]
                       += height;
                                                                                                    }
                    */
                    //						data[heightfieldBuffer->getElementRowCountPerPage()*y+x] =
                    //255.0f
                    //*
                    //(Ogre::Real)((x + y) & 255);
                    float height = Utils::clamp(
                        data[heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCountPerPage() * y +
                             x],
                        0.0f, 255.0f);
                    height = 2 * height * height;
                    // height = height * 2.0f - 255.0f;
                    // height = height * height;
                    // height = 0.5f + 0.5f * height;
                    data[heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCountPerPage() * y +
                         x] = 0.5f; // height;
                }
            }
            page->updateFrom(pixelBox);
            delete data;
        }
    }

    heightfieldBuffer->revalidate();
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

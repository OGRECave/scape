/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"

#include "Input/InputManager.h"
#include "Input/InputPointer.h"
#include "SettingsDatasetManager.h"
#include "SettingsDataset.h"
#include "RenderViewManager.h"
#include "RenderView.h"
#include "TickableManager.h"
#include "Tickable.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldBuffer/HeightfieldBufferSetManager.h"
#include "SkySettings.h"
#include "Utils/Utils.h"
#include "Utils/ProceduralLookupTextures.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldGeom/HeightfieldGeomManager.h"
#include "HeightfieldGeom/HeightfieldGeomTileIndexBufferManager.h"
#include "HeightfieldGeom/HeightfieldGeomTileVertexUVBufferManager.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"
#include "HeightfieldOperation/HeightfieldOperationFactory.h"
#include "HeightfieldOperation/HeightfieldOperationStack.h"
#include "GPU2DOperation/GPU2DOperationManager.h"
#include "GPU2DOperation/GPU2DOperationRenderableQuadManager.h"
#include "HeightfieldFileCodecs/HeightfieldFileEncoderManager.h"
#include "HeightfieldFileCodecs/HeightfieldFileDecoderManager.h"

#include "Input/QtJSONButtonDefinitionDataAccesObject.h"

#define EXTERNAL_TEXTURE_BASENAME _T("file:")

using namespace ScapeEngine;

template <> EngineCore* Ogre::Singleton<EngineCore>::msSingleton = 0;

EngineCore::EngineCore(EngineInterface* engineInterface)
    : mEngineInterface(engineInterface), mRoot(NULL), mDebugRenderWindow(NULL),
      mApplicationSettingsConfigFile(NULL), mSceneManager(NULL), mSettingsDatasetManager(NULL),
      mTickableManager(NULL), mRenderViewManager(NULL), mHeightfieldGeomTileVertexUVBufferManager(NULL),
      mHeightfieldGeomTileIndexBufferManager(NULL), mHeightfieldBufferSetManager(NULL), mInputManager(NULL),
      mSceneLoaded(false), mSceneManagerLoaded(false), mFrameTimerMilliseconds(0),
      mLastFrameTimerMilliseconds(0), mTimeSinceLastFrame(0.0f), mHeightfieldGeomManager(NULL),
      mHeightfieldOperationFactory(NULL), mHeightfieldOperationStack(NULL), mHeightfieldBrushManager(NULL),
      mHeightfieldFileEncoderManager(NULL), mHeightfieldFileDecoderManager(NULL), mSkySettings(NULL),
      mGPU2DOperationManager(NULL), mGPU2DOperationRenderableQuadManager(NULL), mFrameCount(0)
{
}

EngineCore::~EngineCore() {}

void EngineCore::initialize()
{
    // Create a new Ogre ROOT
    mRoot = new Ogre::Root("plugins.cfg", Utils::emptyString, "Scape.log");

    mSettingsDatasetManager = new SettingsDatasetManager();

    loadApplicationSettings();

    loadResourceLocations();

    setupRenderSystem();

    mRoot->initialise(false);

    mTickableManager = new TickableManager();

    mRenderViewManager = new RenderViewManager();

    mInputManager = new InputManager();

    mSkySettings = new SkySettings();
    mSkySettings->addListener(this);

    mHeightfieldGeomTileVertexUVBufferManager = new HeightfieldGeomTileVertexUVBufferManager();

    mHeightfieldGeomTileIndexBufferManager = new HeightfieldGeomTileIndexBufferManager();

    mHeightfieldBufferSetManager = new HeightfieldBufferSetManager();

    mHeightfieldGeomManager = new HeightfieldGeomManager();

    mHeightfieldBrushManager = new HeightfieldBrushManager();

    mGPU2DOperationManager = new GPU2DOperationManager();

    mGPU2DOperationRenderableQuadManager = new GPU2DOperationRenderableQuadManager();

    mHeightfieldOperationFactory = new HeightfieldOperationFactory();
    mHeightfieldOperationFactory->registerClasses();

    mHeightfieldOperationStack = new HeightfieldOperationStack();
    // mHeightfieldOperationStack->setNewOperationClassName(_T("HeightfieldOperationCPUBrush"));

    mHeightfieldFileEncoderManager = new HeightfieldFileEncoderManager();
    mHeightfieldFileDecoderManager = new HeightfieldFileDecoderManager();

    std::shared_ptr<ButtonDefinitionDataAccessObject> dao =
        std::shared_ptr<ButtonDefinitionDataAccessObject>(new QtJSONButtonDefinitionDataAccesObject(
            "config.json", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
    getInputManager()->setButtonDefinitionDataAccessObject(dao);
    getInputManager()->loadButtonDefinitions();

    mFrameTimerMilliseconds = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
    mLastFrameTimerMilliseconds = mFrameTimerMilliseconds - 1;
}

void EngineCore::deinitialize()
{
    SAFE_DELETE(mHeightfieldFileEncoderManager);

    SAFE_DELETE(mHeightfieldFileDecoderManager);

    SAFE_DELETE(mHeightfieldOperationFactory);

    SAFE_DELETE(mGPU2DOperationRenderableQuadManager);

    SAFE_DELETE(mHeightfieldBrushManager);

    SAFE_DELETE(mHeightfieldBufferSetManager);

    SAFE_DELETE(mInputManager);

    SAFE_DELETE(mRenderViewManager);

    getTickableManager()->disposeAllActiveTickables(true);

    mHeightfieldOperationStack = NULL;                // tickable
    mHeightfieldGeomManager = NULL;                   // tickable
    mHeightfieldGeomTileIndexBufferManager = NULL;    // tickable
    mHeightfieldGeomTileVertexUVBufferManager = NULL; // tickable
    mGPU2DOperationManager = NULL;

    SAFE_DELETE(mTickableManager);

    SAFE_DELETE(mSettingsDatasetManager);

    SAFE_DELETE(mApplicationSettingsConfigFile);

    SAFE_DELETE(mRoot);
}

void EngineCore::loadApplicationSettings()
{
    const Ogre::String GLOBALSETTINGS_FILENAME = "settings.cfg";
    mApplicationSettingsConfigFile = new Ogre::ConfigFile();

    mApplicationSettingsConfigFile->load(GLOBALSETTINGS_FILENAME);
    Ogre::LogManager::getSingleton().logMessage(_T("Loaded application settings from '") +
                                                string(GLOBALSETTINGS_FILENAME) + _T("'"));

    Ogre::String settingsPath = getApplicationSetting(_T("Paths"), _T("DatasetSettings"));
    getSettingsDatasetManager()->setDatasetResourcePath(settingsPath);
}

string EngineCore::getApplicationSetting(const string& section, const string& key)
{
    if (mApplicationSettingsConfigFile)
    {
        return mApplicationSettingsConfigFile->getSetting(key, section);
    }
    return Utils::emptyString;
}

void EngineCore::loadResourceLocations()
{
    const Ogre::String RESOURCES_FILENAME = "resources.cfg";
    Ogre::ConfigFile cf;
    cf.load(RESOURCES_FILENAME);

    Ogre::LogManager::getSingleton().logMessage("Loaded resource paths from '" +
                                                string(RESOURCES_FILENAME) + "'");

    const Ogre::ConfigFile::SettingsBySection_& settings = cf.getSettingsBySection();
    for (Ogre::ConfigFile::SettingsBySection_::const_iterator it = settings.begin(); it != settings.end(); it++)
    {
        const Ogre::String& secName = it->first;
        const Ogre::ConfigFile::SettingsMultiMap& settings = it->second;

        for (Ogre::ConfigFile::SettingsMultiMap::const_iterator sit = settings.begin(); sit != settings.end(); sit++)
        {
            const Ogre::String& typeName = sit->first;
            const Ogre::String& archName = sit->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
}

void EngineCore::setupRenderSystem()
{
    const Ogre::RenderSystemList& renderSystemList = mRoot->getAvailableRenderers();
    Ogre::RenderSystemList::const_iterator renderSystemIt = renderSystemList.begin();
    if (renderSystemIt != renderSystemList.end())
    {
        mRoot->setRenderSystem(*renderSystemIt);
    }
}

void EngineCore::attachInputToWindow(string inputWindow)
{
    // getInputManager()->attachToWindow(inputWindow);
}

void EngineCore::loadScene()
{
    if (!mSceneLoaded)
    {
        mSceneManager->setAmbientLight(Ogre::ColourValue(0.0, 0.0, 0.0));

        mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);

        Ogre::ColourValue fadeColour(0.63, 0.71, 0.67);

        loadSkyBox();

        { // create the heightfield buffer with the dimensions specified in Startup.conf
            int columns = 0;
            int rows = 0;
            Ogre::Real height = 0;
            SettingsDataset* startupDataset =
                getEngineCore()->getSettingsDatasetManager()->getDataset("Startup");
            if (startupDataset)
            {
                columns = Ogre::StringConverter::parseInt(
                    startupDataset->getSetting("Heightfield", "", "columns"));
                rows =
                    Ogre::StringConverter::parseInt(startupDataset->getSetting("Heightfield", "", "rows"));
                height = Ogre::StringConverter::parseReal(
                    startupDataset->getSetting("Heightfield", "", "height"));
            }

            if (columns <= 0)
                columns = 4096;
            if (rows <= 0)
                rows = 4096;
            if (height <= 0.0f)
                height = 500.0f;

            getHeightfieldBufferSetManager()->create(_T("Project"), columns, rows, 0, height, 512, 512);
        }

        HeightfieldBufferSetHandle handle = getHeightfieldBufferSetManager()->findHandle(_T("Project"));
        HeightfieldBufferSet* heightfieldBufferSet = getHeightfieldBufferSetManager()->get(handle);
        heightfieldBufferSet->create(_T("Render"), Ogre::PF_SHORT_L);

        resetHeightfield();

        Utils::createPerlinPerm2DTexture(_T("PerlinPerm2D"));
        Utils::createPerlinGrad2DTexture(_T("PerlinGrad2D"));

        mSceneLoaded = true;
    }
}

void EngineCore::resetHeightfield()
{
    HeightfieldBufferSetHandle handle = getHeightfieldBufferSetManager()->findHandle(_T("Project"));
    HeightfieldBufferSet* heightfieldBufferSet = getHeightfieldBufferSetManager()->get(handle);
    HeightfieldBufferHandle heightfieldBufferHandle = heightfieldBufferSet->findHandle(_T("Render"));
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
                                                            float power = 0.05f;
                                                            for (int level = 11; level >= 0; level--)
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
                    //						data[heightfieldBuffer->getElementRowCountPerPage()*y+x] = 255.0f *
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

Ogre::SceneManager* EngineCore::getSceneManager() { return mSceneManager; }

void EngineCore::loadSceneManager()
{
    if (!mSceneManagerLoaded)
    {

        mRoot->getRenderSystem()->setWBufferEnabled(true);

        // Create the SceneManager, in this case a generic one
        mSceneManager = mRoot->createSceneManager(_T("OctreeSceneManager"));

        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
        // MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
        // MaterialManager::getSingleton().setDefaultAnisotropy(8);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        mSceneManagerLoaded = true;

        //	new TestTickable(0, false);
        // new TestTickable(255, true);
        // new TestTickable(2, false);
    }
}

float EngineCore::getTimeSinceLastFrame() const { return mTimeSinceLastFrame; }

static Ogre::SceneNode* node = 0;
void EngineCore::update()
{
    if (getSceneManager())
    {
        long newFrameTickMilliseconds = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
        mTimeSinceLastFrame =
            Utils::max(0.0005f, 0.001f * (newFrameTickMilliseconds - mFrameTimerMilliseconds));
        mLastFrameTimerMilliseconds = mFrameTimerMilliseconds;
        mFrameTimerMilliseconds = newFrameTickMilliseconds;
        ++mFrameCount;

        getInputManager()->captureInput();

        getTickableManager()->onPreSubTick();
        getTickableManager()->onSubTick();
        getTickableManager()->onPostSubTick();
        getTickableManager()->onPreFrameTick();

        /*
        {
                static bool a = false;
                if
        (getEngineCore()->getInputManager()->getButton(ButtonId::MATERIAL_RESET)->isJustPressed())
                {
                        if (a)
                        {
                                getHeightfieldOperationStack()->setNewOperationClassName(_T("HeightfieldOperationCPUBrush"));
                        }
                        else
                        {
                                getHeightfieldOperationStack()->setNewOperationClassName(_T("HeightfieldOperationCPUErosion"));
                        }
                        a = !a;
                }
        }
        */

        /*
        // reset the heightfield when F5 is pressed
        if (getEngineCore()->getInputManager()->getButton(ButtonId::MATERIAL_RESET)->isJustPressed())
        {
                resetHeightfield();
        }
        */

        {
            static float time = 0;
            time += getTimeSinceLastFrame();

            {
                static bool started = false;
                // if (!started) {mActiveHeightfieldOperation->tick(); started = true;}
                // mActiveHeightfieldOperation->tick();
            }
        }

        {
            static bool createdGeometry = false;
            if (!createdGeometry)
            {
                createdGeometry = true;

                // getHeightfieldGeomTileVertexUVBufferManager()->getBuffer(Ogre::VET_FLOAT2, 1025, 1025);
                // getHeightfieldGeomTileVertexUVBufferManager()->getBuffer(Ogre::VET_SHORT2, 1025, 1025);

                Ogre::SceneNode* node = getEngineCore()->getSceneManager()->getRootSceneNode();
                HeightfieldBufferSet* heigtfieldBufferSet = getHeightfieldBufferSetManager()->get(
                    getHeightfieldBufferSetManager()->findHandle(_T("Project")));
                getHeightfieldGeomManager()->create(
                    heigtfieldBufferSet->get(heigtfieldBufferSet->findHandle(_T("Render"))), node);

                /*
                Ogre::SceneNode* node2 =
                getEngineCore()->getSceneManager()->getRootSceneNode()->createChildSceneNode(_T("spherenode"));
                Ogre::Entity* entity2 = getSceneManager()->createEntity(_T("sphere"), _T("sphere.mesh"));
                node2->attachObject(entity2);

                node2->setScale(0.01, 0.01, 0.01);
                node2->setPosition(0, 20, 20);

                Ogre::SceneNode* node3 =
                getEngineCore()->getSceneManager()->getRootSceneNode()->createChildSceneNode(_T("spherenode2"));
                Ogre::Entity* entity3 = getSceneManager()->createEntity(_T("sphere2"), _T("sphere.mesh"));
                node3->attachObject(entity3);

                node3->setScale(0.01, 0.01, 0.01);
                node3->setPosition(20, 0, 0);
                */
            }
        }

        // TODO GdCarpentier 8/27/2007 : remove
        {
            static float theta = 255.4;
            static float phi = 0;
            static float t = 0;
            // phi += 0.002f;
            t += getTimeSinceLastFrame();
            theta = 255.0;
            phi = 255 * t; // sin(2 * t) * 2;

            const float cosTheta = cos(theta);
            const float sinTheta = sin(theta);

            const float y = cosTheta;
            const float x = sinTheta * cos(phi);
            const float z = sinTheta * sin(phi);

            Ogre::Vector3 dir(-x, -y, -z);
            dir.normalise();

            if (!mSceneManager->hasLight(_T("MainLight")))
            {
                mSceneManager->createLight(_T("MainLight"));
            }
            Ogre::Light* l = mSceneManager->getLight(_T("MainLight"));
            l->setType(Ogre::Light::LT_DIRECTIONAL);
            l->setDirection(dir);

            /* this one when using normal map as the up direction is in z coordinate.
            const Real z = cosTheta;
            const Real x = sinTheta * cos(phi);
            const Real y = sinTheta * sin(phi); */

            // Interchange y & z because normal map uses z as up while in our world (for sun too), y
            // direction is up.
            const Ogre::Vector3 SunDir(x, z, y);
            const Ogre::Real SunAngle = z;

            mSceneManager->setOption(_T("Sun"), &SunDir);
            mSceneManager->setOption(_T("SunAngle"), &SunAngle);
        }

        /*
        {
                if (getInputManager()->getInputPointer()->getCurrentState().getRenderView2DPoint().first)
                {
                        Ogre::Viewport* viewport =
        getInputManager()->getInputPointer()->getCurrentState().getRenderView2DPoint().first->getViewport();
                        //static bool boundCompositor = false;
                        if (viewport)
                        {
                                Ogre::CompositorInstance* compInst =
        Ogre::CompositorManager::getSingleton().addCompositor(viewport, _T("B&W"));
                                Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport,
        _T("B&W"), true);
                                //boundCompositor = true;
                        }
                }
        }
        */

        getTickableManager()->onFrameTick();

        getHeightfieldBufferSetManager()->unlockAll();

        getRenderViewManager()->updateAll();

        // printf("%f\n", getTimeSinceLastFrame());//somehow smoothes fps

        getTickableManager()->onPostFrameTick();

        getHeightfieldBufferSetManager()->unlockAll();

        getTickableManager()->postFrameCleanup();

        getSettingsDatasetManager()->saveAllDirty();
    }
}

EngineCore& EngineCore::getSingleton() { return *msSingleton; }

Ogre::RenderWindow* EngineCore::getDebugRenderWindow()
{
    if (mDebugRenderWindow == NULL)
    {
        mDebugRenderWindow =
            Ogre::Root::getSingleton().createRenderWindow("GPU2DOperationTarget", 512, 512, false);
    }
    return mDebugRenderWindow;
}

void EngineCore::onSkySettingsUpdate() { loadSkyBox(); }

void EngineCore::loadSkyBox()
{
    const SkySettings::SkyBoxTextureNames& textureNames = getSkySettings()->getSkyBoxTextureNames();

    string newTextureNames[6];
    newTextureNames[0] = textureNames.front;
    newTextureNames[1] = textureNames.back;
    newTextureNames[2] = textureNames.left;
    newTextureNames[3] = textureNames.right;
    newTextureNames[4] = textureNames.top;
    newTextureNames[5] = textureNames.bottom;

    bool foundDifference = false;

    Ogre::MaterialPtr matPtr = Ogre::MaterialManager::getSingleton().getByName(_T("SkyBox"));
    matPtr->load();
    const Ogre::Material::Techniques& techniques = matPtr->getSupportedTechniques();
    for (Ogre::Material::Techniques::const_iterator it = techniques.begin(); it != techniques.end(); it++)
    {
        Ogre::Technique* tech = (*it);

        for (int side = 0; side < 6; ++side)
        {
            string oldTextureName = tech->getPass(0)->getTextureUnitState(0)->getFrameTextureName(side);
            string newTextureName = EXTERNAL_TEXTURE_BASENAME + newTextureNames[side];

            if ((EXTERNAL_TEXTURE_BASENAME + oldTextureName).compare(newTextureName) != 0)
            {
                Ogre::TextureManager::getSingleton().remove(oldTextureName);
                foundDifference = true;
            }

            Utils::getTextureFromExternalFile(newTextureName,
                                              Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                              newTextureNames[side]);

            tech->getPass(0)->getTextureUnitState(0)->setFrameTextureName(newTextureName, side);
        }
    }

    if (foundDifference)
    {
        mSceneManager->setSkyBox(true, _T("SkyBox"), 5000, false);
    }
}

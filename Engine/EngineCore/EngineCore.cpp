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
#include "HeightfieldManager.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldBuffer/HeightfieldBufferSetManager.h"
#include "SkySettings.h"
#include "Utils/Utils.h"
#include "Utils/ProceduralLookupTextures.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"
#include "Input/QtJSONButtonDefinitionDataAccesObject.h"
#include "QtJSONStartupSettingsDataAccessObject.h"

#define EXTERNAL_TEXTURE_BASENAME _T("file:")

using namespace ScapeEngine;

template <> EngineCore* Ogre::Singleton<EngineCore>::msSingleton = 0;

EngineCore::EngineCore(EngineInterface* engineInterface)
    : mEngineInterface(engineInterface), mRoot(NULL), mOverlaySystem(NULL), mDebugRenderWindow(NULL),
      mApplicationSettingsConfigFile(NULL), mStartupSettingsDataAccessObject(), mSceneManager(NULL),
      mSettingsDatasetManager(NULL), mSettingsPath(), mTickableManager(NULL), mRenderViewManager(NULL),
      mHeightfieldManager(NULL), mInputManager(NULL), mSceneLoaded(false), mSceneManagerLoaded(false),
      mFrameTimerMilliseconds(0), mLastFrameTimerMilliseconds(0), mTimeSinceLastFrame(0.0f),
      mSkySettings(NULL), mFrameCount(0)
{
}

EngineCore::~EngineCore() {}

void EngineCore::initialize()
{
    // Create a new Ogre ROOT
    mRoot = new Ogre::Root("plugins.cfg", Utils::emptyString, "Scape.log");

    mOverlaySystem = new Ogre::OverlaySystem();

    mSettingsDatasetManager = new SettingsDatasetManager();

    loadApplicationSettings();

    loadResourceLocations();

    setupRenderSystem();

    mRoot->initialise(false);

    mTickableManager = new TickableManager();

    mRenderViewManager = new RenderViewManager();

    mInputManager = new InputManager();

    mStartupSettingsDataAccessObject = std::unique_ptr<StartupSettingsDataAccessObject>(
        new QtJSONStartupSettingsDataAccessObject(mSettingsPath + "config.json"));

    mSkySettings = new SkySettings();
    mSkySettings->addListener(this);

    mHeightfieldManager = new HeightfieldManager(*mStartupSettingsDataAccessObject);

    std::shared_ptr<ButtonDefinitionDataAccessObject> dao =
        std::shared_ptr<ButtonDefinitionDataAccessObject>(
            new QtJSONButtonDefinitionDataAccesObject(mSettingsPath + "config.json"));
    getInputManager()->setButtonDefinitionDataAccessObject(dao);
    getInputManager()->loadButtonDefinitions();

    mFrameTimerMilliseconds = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
    mLastFrameTimerMilliseconds = mFrameTimerMilliseconds - 1;
}

void EngineCore::deinitialize()
{
    SAFE_DELETE(mInputManager);

    SAFE_DELETE(mRenderViewManager);

    SAFE_DELETE(mHeightfieldManager);

    getTickableManager()->disposeAllActiveTickables(true);

    SAFE_DELETE(mTickableManager);

    SAFE_DELETE(mSettingsDatasetManager);

    SAFE_DELETE(mApplicationSettingsConfigFile);

    if (mSceneManagerLoaded)
    {
        mSceneManager->removeRenderQueueListener(mOverlaySystem);
    }

    SAFE_DELETE(mOverlaySystem);

    SAFE_DELETE(mRoot);
}

void EngineCore::loadApplicationSettings()
{
    const Ogre::String GLOBALSETTINGS_FILENAME = "settings.cfg";
    mApplicationSettingsConfigFile = new Ogre::ConfigFile();

    mApplicationSettingsConfigFile->load(GLOBALSETTINGS_FILENAME);
    Ogre::LogManager::getSingleton().logMessage(_T("Loaded application settings from '") +
                                                string(GLOBALSETTINGS_FILENAME) + _T("'"));

    mSettingsPath = getApplicationSetting(_T("Paths"), _T("DatasetSettings"));
    if (mSettingsPath.length() > 0)
    {
        mSettingsPath = Ogre::StringUtil::standardisePath(mSettingsPath);
    }
    getSettingsDatasetManager()->setDatasetResourcePath(mSettingsPath);
}

string EngineCore::getApplicationSetting(const string& section, const string& key)
{
    if (mApplicationSettingsConfigFile)
    {
        return mApplicationSettingsConfigFile->getSetting(key, section);
    }
    return Utils::emptyString;
}

SettingsDatasetManager* EngineCore::getSettingsDatasetManager() { return mSettingsDatasetManager; }

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

        mHeightfieldManager->initialize();

        mHeightfieldManager->resetHeightfield();

        Utils::createPerlinPerm2DTexture(_T("PerlinPerm2D"));
        Utils::createPerlinGrad2DTexture(_T("PerlinGrad2D"));

        mSceneLoaded = true;
    }
}

InputManager* EngineCore::getInputManager() const { return mInputManager; }

RenderViewManager* EngineCore::getRenderViewManager() const { return mRenderViewManager; }

TickableManager* EngineCore::getTickableManager() const { return mTickableManager; }

StartupSettingsDataAccessObject& EngineCore::getStartupSettingsDataAccessObject() const
{
    return *mStartupSettingsDataAccessObject;
}

SkySettings* EngineCore::getSkySettings() const { return mSkySettings; }

HeightfieldManager* EngineCore::getHeightfieldManager() const { return mHeightfieldManager; }

HeightfieldGeomManager* EngineCore::getHeightfieldGeomManager() const
{
    return mHeightfieldManager->getHeightfieldGeomManager();
}

HeightfieldBrushManager* EngineCore::getHeightfieldBrushManager() const
{
    return mHeightfieldManager->getHeightfieldBrushManager();
}

HeightfieldOperationFactory* EngineCore::getHeightfieldOperationFactory() const
{
    return mHeightfieldManager->getHeightfieldOperationFactory();
}

HeightfieldOperationStack* EngineCore::getHeightfieldOperationStack() const
{
    return mHeightfieldManager->getHeightfieldOperationStack();
}

GPU2DOperationManager* EngineCore::getGPU2DOperationManager() const
{
    return mHeightfieldManager->getGPU2DOperationManager();
}

GPU2DOperationRenderableQuadManager* EngineCore::getGPU2DOperationRenderableQuadManager() const
{
    return mHeightfieldManager->getGPU2DOperationRenderableQuadManager();
}

HeightfieldFileEncoderManager* EngineCore::getHeightfieldFileEncoderManager() const
{
    return mHeightfieldManager->getHeightfieldFileEncoderManager();
}

HeightfieldFileDecoderManager* EngineCore::getHeightfieldFileDecoderManager() const
{
    return mHeightfieldManager->getHeightfieldFileDecoderManager();
}

Ogre::SceneManager* EngineCore::getSceneManager() { return mSceneManager; }

void EngineCore::loadSceneManager()
{
    if (!mSceneManagerLoaded)
    {
        // Create the SceneManager, in this case a generic one
        mSceneManager = mRoot->createSceneManager(_T("OctreeSceneManager"));

        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
        // MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
        // MaterialManager::getSingleton().setDefaultAnisotropy(8);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        mSceneManager->addRenderQueueListener(mOverlaySystem);

        mSceneManagerLoaded = true;

        //	new TestTickable(0, false);
        // new TestTickable(255, true);
        // new TestTickable(2, false);
    }
}

float EngineCore::getTimeSinceLastFrame() const { return mTimeSinceLastFrame; }

unsigned long EngineCore::getFrameMilliseconds() { return mFrameTimerMilliseconds; }
unsigned long EngineCore::getLastFrameMilliseconds() { return mLastFrameTimerMilliseconds; }
unsigned long EngineCore::getFrameCount() { return mFrameCount; }

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

        tick();

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

        mHeightfieldManager->createGeometry();

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

        mHeightfieldManager->getHeightfieldBufferSetManager()->unlockAll();

        getRenderViewManager()->updateAll();

        // printf("%f\n", getTimeSinceLastFrame());//somehow smoothes fps

        getTickableManager()->onPostFrameTick();

        mHeightfieldManager->getHeightfieldBufferSetManager()->unlockAll();

        getTickableManager()->postFrameCleanup();

        getSettingsDatasetManager()->saveAllDirty();
    }
}

void EngineCore::tick() { mHeightfieldManager->getHeightfieldBrushManager()->tick(); }

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
    bool foundDifference = false;

    string newTextureName = getSkySettings()->getSkyBoxTextureName();

    Ogre::MaterialPtr matPtr = Ogre::MaterialManager::getSingleton().getByName("SkyBox");
    matPtr->load();

    const Ogre::Material::Techniques& techniques = matPtr->getSupportedTechniques();
    for (Ogre::Material::Techniques::const_iterator it = techniques.begin(); it != techniques.end(); it++)
    {
        Ogre::TextureUnitState* tus = (*it)->getPass(0)->getTextureUnitState(0);

        string oldTextureName = tus->getTextureName();
        Ogre::TextureType oldTextureType = tus->getTextureType();

        if (oldTextureName.compare(newTextureName) != 0 || oldTextureType != Ogre::TEX_TYPE_CUBE_MAP)
        {
            foundDifference = true;
            tus->setTextureName(newTextureName, Ogre::TEX_TYPE_CUBE_MAP);
        }
    }

    if (foundDifference || !mSceneLoaded)
    {
        mSceneManager->setSkyBox(true, "SkyBox", 5000, false);
    }
}

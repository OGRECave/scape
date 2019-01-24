/**
 * ScapeEngine::EngineCore class
 *
 * Engine core, managing all other modules.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __ENGINECORE_H__
#define __ENGINECORE_H__

#include "ScapeEnginePrerequisites.h"

#include "SkySettings.h"

namespace ScapeEngine
{

class EngineCore : public Ogre::Singleton<EngineCore>, public SkySettingsListener
{

public:
    // Create EngineCore object. Don't call this directly but
    // use getSingleton() instead.
    EngineCore(class EngineInterface* engineInterface);

    // Deconstruct EngineCore. Be sure to call deinitialize() before an
    // initialized engine is deconstructed
    ~EngineCore();

    // Get reference to EngineCore singleton instance
    static EngineCore& getSingleton();

    // Get pointer to the InputManager instance
    class InputManager* getInputManager() const { return mInputManager; }

    // Get pointer to the HeightfieldGeomTileVertexUVBufferManager instance
    class HeightfieldGeomTileVertexUVBufferManager* getHeightfieldGeomTileVertexUVBufferManager() const
    {
        return mHeightfieldGeomTileVertexUVBufferManager;
    }

    // Get pointer to the HeightfieldGeomTileVertexIndexManager instance
    class HeightfieldGeomTileIndexBufferManager* getHeightfieldGeomTileIndexBufferManager() const
    {
        return mHeightfieldGeomTileIndexBufferManager;
    }

    // Get pointer to the RenderViewManager instance
    class RenderViewManager* getRenderViewManager() const { return mRenderViewManager; }

    class HeightfieldBrushManager* getHeightfieldBrushManager() const { return mHeightfieldBrushManager; }

    class HeightfieldOperationFactory* getHeightfieldOperationFactory() const
    {
        return mHeightfieldOperationFactory;
    }

    class HeightfieldOperationStack* getHeightfieldOperationStack() const
    {
        return mHeightfieldOperationStack;
    }

    class GPU2DOperationManager* getGPU2DOperationManager() { return mGPU2DOperationManager; }

    class GPU2DOperationRenderableQuadManager* getGPU2DOperationRenderableQuadManager()
    {
        return mGPU2DOperationRenderableQuadManager;
    }

    // Get pointer to the TickableManager instance
    class TickableManager* getTickableManager() const { return mTickableManager; }

    class SkySettings* getSkySettings() { return mSkySettings; }

    class HeightfieldGeomManager* getHeightfieldGeomManager() const { return mHeightfieldGeomManager; }

    // Read-only global settings
    string getApplicationSetting(const string& section, const string& key);

    // More structured read/write settings
    class SettingsDatasetManager* getSettingsDatasetManager() { return mSettingsDatasetManager; }

    class HeightfieldFileEncoderManager* getHeightfieldFileEncoderManager()
    {
        return mHeightfieldFileEncoderManager;
    }
    class HeightfieldFileDecoderManager* getHeightfieldFileDecoderManager()
    {
        return mHeightfieldFileDecoderManager;
    }

    Ogre::RenderWindow* getDebugRenderWindow();

    // Give the engine a change to update itself and render a frame
    // if it wants to. Call this as very often.
    void update();

    void tick();

    // Initialize all engine modules except a render view
    void initialize();

    // Destroy the engine and its managed modules
    void deinitialize();

    // Attach mouse and keyboard input to root window
    void attachInputToWindow(string inputWindow);

    // Load scene when ready for it
    void loadScene();

    // Load scene manager when ready for it
    void loadSceneManager();

    // Get current scene manager
    Ogre::SceneManager* getSceneManager();

    // Get time (in seconds) since last frame
    float getTimeSinceLastFrame() const;

    unsigned long getFrameMilliseconds() { return mFrameTimerMilliseconds; }
    unsigned long getLastFrameMilliseconds() { return mLastFrameTimerMilliseconds; }
    unsigned long getFrameCount() { return mFrameCount; }

    virtual void onSkySettingsUpdate();

private:
    // Ogre root pointer
    Ogre::Root* mRoot;

    // Ogre render system pointer
    Ogre::SceneManager* mSceneManager;

    class EngineInterface* mEngineInterface;

    class TickableManager* mTickableManager;

    class RenderViewManager* mRenderViewManager;

    class HeightfieldGeomTileVertexUVBufferManager* mHeightfieldGeomTileVertexUVBufferManager;

    class HeightfieldGeomTileIndexBufferManager* mHeightfieldGeomTileIndexBufferManager;

    class HeightfieldBufferSetManager* mHeightfieldBufferSetManager;

    class HeightfieldGeomManager* mHeightfieldGeomManager;

    class HeightfieldOperationFactory* mHeightfieldOperationFactory;

    class HeightfieldOperationStack* mHeightfieldOperationStack;

    class HeightfieldBrushManager* mHeightfieldBrushManager;

    Ogre::RenderWindow* mDebugRenderWindow;

    class InputManager* mInputManager;

    class SettingsDatasetManager* mSettingsDatasetManager;

    class Ogre::ConfigFile* mApplicationSettingsConfigFile;

    class HeightfieldFileEncoderManager* mHeightfieldFileEncoderManager;
    class HeightfieldFileDecoderManager* mHeightfieldFileDecoderManager;

    class SkySettings* mSkySettings;

    class GPU2DOperationManager* mGPU2DOperationManager;

    class GPU2DOperationRenderableQuadManager* mGPU2DOperationRenderableQuadManager;

    // Is scene loaded already?
    bool mSceneLoaded;

    // Is scenemanager loaded already?
    bool mSceneManagerLoaded;

    // Milliseconds since start of application
    unsigned long mFrameTimerMilliseconds;
    unsigned long mLastFrameTimerMilliseconds;
    unsigned long mFrameCount;

    // Time since last frame in seconds
    float mTimeSinceLastFrame;

    // Get pointer to the HeightfieldBufferSet instance
    class HeightfieldBufferSetManager* getHeightfieldBufferSetManager() const
    {
        return mHeightfieldBufferSetManager;
    }

    // Load ogre plug-ins, among other things
    void loadApplicationSettings();

    // Set Ogre resource locations
    void loadResourceLocations();

    // Set Ogre render system
    void setupRenderSystem();

    void loadSkyBox();

    void resetHeightfield();
};

inline EngineCore* getEngineCore() { return &EngineCore::getSingleton(); }
}

#endif // __ENGINECORE_H__

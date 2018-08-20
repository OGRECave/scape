/**
 * ScapeEngine::SkySettings class
 *
 * Manages all sky settings
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __SKYSETTINGS_H__
#define __SKYSETTINGS_H__

#include "EngineCore/UIElementClasses.h"
#include "EngineCore/Tickable.h"

namespace ScapeEngine {
    class SkySettingsListener
    {
    public:
        virtual void onSkySettingsUpdate() = 0;
    };

    class SkySettings : public UIElementPresetContainerSimple, public Tickable
    {
        DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(SkySettings)
    public:
        struct SkyBoxTextureNames
        {
            string left, right, top, bottom, front, back;
        };

        const Ogre::ColourValue getSkyColor() { return mSkyColor; }
        Ogre::Real getSkyOverbright() { return mSkyOverbright; }
        const Ogre::ColourValue getSunColor() { return mSunColor; }
        Ogre::Real getSunOverbright() { return mSunOverbright; }
        Ogre::Real getSunDirection() { return mSunDirection; }
        Ogre::Real getSunHeight() { return mSunHeight; }
        const Ogre::ColourValue getFogColor() { return mFogColor; }
        Ogre::Real getFogDistance() { return mFogDistance; }
        const SkyBoxTextureNames& getSkyBoxTextureNames() { return mSkyBoxTextureNames; }
        SkySettings();

        void addListener(SkySettingsListener* listener);
        void removeListener(SkySettingsListener* listener);

        virtual void onPreFrameTick();

    protected:
        Ogre::ColourValue mSkyColor;
        Ogre::Real mSkyOverbright;

        Ogre::ColourValue mSunColor;
        Ogre::Real mSunOverbright;
        Ogre::Real mSunDirection;
        Ogre::Real mSunHeight;

        Ogre::ColourValue mFogColor;
        Ogre::Real mFogDistance;

        SkyBoxTextureNames mSkyBoxTextureNames;

        typedef std::list<SkySettingsListener*> SkySettingsListeners;
        SkySettingsListeners mListeners;

        virtual bool initPersistentElementValueMap(StringStringMap& map);
        virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
        virtual string setUIElementPropertyValue(const string& elementName, const string& value);

        void notifyListeners();
    };
}

#endif // __SKYSETTINGS_H__

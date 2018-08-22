/**
 * ScapeEngine::HeightfieldGeomMaterialSplat class
 *
 * Subclass of HeightfieldGeomMaterial, capable of rendering HeightfieldGeomTiles with splatted textures
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDGEOMMATERIALSPLAT_H__
#define __HEIGHTFIELDGEOMMATERIALSPLAT_H__

#include "EngineCore/SkySettings.h"
#include "HeightfieldGeomMaterial.h"

namespace ScapeEngine
{
namespace _HeightfieldGeomMaterialSplatNS
{
enum EShaderConstant
{
    GRADIENTSCALEBIAS = _HeightfieldGeomMaterialNS::SHADERCONSTANT_COUNT,
    LIGHTDIRECTION,
    SUNCOLOR,
    SKYCOLOR,
    ISOSCALES,
    ISODOTSCALES,
    UVSCALEGLOBAL,
    UVSCALELOCAL,
    UVSCALELOCALDISTORT,
    LAYERHEIGHTSCALE,
    LAYERHEIGHTBIAS,
    LAYERHEIGHTDISTORT,
    LAYERSLOPESCALE,
    LAYERSLOPEBIAS,
    INTENSITYSCALEGLOBAL,
    FOGCOLOR,
    LODERROR,
    SHADERCONSTANT_COUNT
};
}

class HeightfieldGeomMaterialSplat : public HeightfieldGeomMaterial, public SkySettingsListener
{
    DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldGeomMaterialSplat)
public:
    HeightfieldGeomMaterialSplat();

    virtual ~HeightfieldGeomMaterialSplat();

    virtual void updateShaderConstantsGeom(const class HeightfieldGeom* heightfieldGeom,
                                           class ShaderCustomAutoConstants* shaderCustomAutoConstants,
                                           bool forceAll);
    virtual void updateShaderConstantsGeomTile(const class HeightfieldGeomTile* heightfieldGeomTile,
                                               class ShaderCustomAutoConstants* shaderCustomAutoConstants,
                                               bool forceAll);

    virtual void updateTextures();

    virtual void onSkySettingsUpdate();

    virtual Ogre::Real getLODPixelError() { return mLODPixelError; }
    virtual Ogre::Real getLODDistancePower() { return mLODDistancePower; }

protected:
    string mMaterialName;

    Ogre::Real mUVScaleGlobal;
    Ogre::Real mUVScaleLocal;
    Ogre::Real mSlopeDistortLo, mSlopeDistortMid, mSlopeDistortHi;
    Ogre::Real mLightAngleScale;
    Ogre::Real mIntensityDistortLo, mIntensityDistortMid, mIntensityDistortHi;
    struct LayerProperties
    {
        Ogre::Real height, heightVar, heightUp;
        Ogre::Real slope, slopeVar, slopeUp;
        Ogre::Real distortLo, distortMid, distortHi;
        string textureFileName;
    };
    LayerProperties mLayerProperties[5];
    string mNoiseTextureFileName;
    bool mIsoXZ, mIsoY;
    Ogre::Real mIsoFade;
    bool mMultiProject;
    Ogre::Real mLODPixelError;
    Ogre::Real mLODDistancePower;

    virtual void bindShaderConstants();

    virtual Ogre::MaterialPtr createMaterial();

    virtual bool initPersistentElementValueMap(StringStringMap& map);
    virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
    virtual string setUIElementPropertyValue(const string& elementName, const string& value);

    void forceReloadTexture(const string& oldFileName, const string& newFileName);
    void updateMaterialType();
};
}

#endif // __HEIGHTFIELDGEOMMATERIALSPLAT_H__
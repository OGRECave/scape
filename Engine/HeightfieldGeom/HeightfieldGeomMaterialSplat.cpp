/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "EngineCore/SkySettings.h"
#include "HeightfieldGeomMaterialSplat.h"
#include "HeightfieldGeom.h"
#include "HeightfieldGeomTile.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "Utils/ShaderCustomAutoConstants.h"

using namespace ScapeEngine;

#define TEXTUREBASENAME _T("file:")

enum EPropertyId
{
    PROPERTYID_MULTIPROJECT,

    PROPERTYID_UVSCALEGLOBAL,
    PROPERTYID_UVSCALELOCAL,
    PROPERTYID_ISOXZ,
    PROPERTYID_ISOY,
    PROPERTYID_ISOFADE,

    PROPERTYID_NOISETEXTURE,
    PROPERTYID_SLOPEDISTORTLO,
    PROPERTYID_SLOPEDISTORTMID,
    PROPERTYID_SLOPEDISTORTHI,
    PROPERTYID_INTENSITYDISTORTLO,
    PROPERTYID_INTENSITYDISTORTMID,
    PROPERTYID_INTENSITYDISTORTHI,

    PROPERTYID_LIGHTANGLESCALE,
    PROPERTYID_SUNHEIGHT,
    PROPERTYID_SUNDIRECTION,

    PROPERTYID_LAYER1TEXTURE,

    PROPERTYID_LAYER2TEXTURE,
    PROPERTYID_LAYER2DISTORTLO,
    PROPERTYID_LAYER2DISTORTMID,
    PROPERTYID_LAYER2DISTORTHI,
    PROPERTYID_LAYER2HEIGHT,
    PROPERTYID_LAYER2HEIGHTVAR,
    PROPERTYID_LAYER2HEIGHTUP,
    PROPERTYID_LAYER2SLOPE,
    PROPERTYID_LAYER2SLOPEVAR,
    PROPERTYID_LAYER2SLOPEUP,

    PROPERTYID_LAYER3TEXTURE,
    PROPERTYID_LAYER3DISTORTLO,
    PROPERTYID_LAYER3DISTORTMID,
    PROPERTYID_LAYER3DISTORTHI,
    PROPERTYID_LAYER3HEIGHT,
    PROPERTYID_LAYER3HEIGHTVAR,
    PROPERTYID_LAYER3HEIGHTUP,
    PROPERTYID_LAYER3SLOPE,
    PROPERTYID_LAYER3SLOPEVAR,
    PROPERTYID_LAYER3SLOPEUP,

    PROPERTYID_LAYER4TEXTURE,
    PROPERTYID_LAYER4DISTORTLO,
    PROPERTYID_LAYER4DISTORTMID,
    PROPERTYID_LAYER4DISTORTHI,
    PROPERTYID_LAYER4HEIGHT,
    PROPERTYID_LAYER4HEIGHTVAR,
    PROPERTYID_LAYER4HEIGHTUP,
    PROPERTYID_LAYER4SLOPE,
    PROPERTYID_LAYER4SLOPEVAR,
    PROPERTYID_LAYER4SLOPEUP,

    PROPERTYID_LODERROR,
    PROPERTYID_LODPOWER,
};


// ----------------------------------------------------------------------------
bool HeightfieldGeomMaterialSplat::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString));
    return true;
}

// ----------------------------------------------------------------------------
bool HeightfieldGeomMaterialSplat::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    // lighting properties
    ADD_STRINGENUM(map, PROPERTYID_, LIGHTANGLESCALE);

    // iso-line properties
    ADD_STRINGENUM(map, PROPERTYID_, ISOXZ);
    ADD_STRINGENUM(map, PROPERTYID_, ISOY);
    ADD_STRINGENUM(map, PROPERTYID_, ISOFADE);
    // projection properties
    ADD_STRINGENUM(map, PROPERTYID_, MULTIPROJECT);
    ADD_STRINGENUM(map, PROPERTYID_, UVSCALEGLOBAL);
    ADD_STRINGENUM(map, PROPERTYID_, UVSCALELOCAL);

    // noise properties
    ADD_STRINGENUM(map, PROPERTYID_, NOISETEXTURE);
    ADD_STRINGENUM(map, PROPERTYID_, SLOPEDISTORTLO);
    ADD_STRINGENUM(map, PROPERTYID_, SLOPEDISTORTMID);
    ADD_STRINGENUM(map, PROPERTYID_, SLOPEDISTORTHI);
    ADD_STRINGENUM(map, PROPERTYID_, INTENSITYDISTORTLO);
    ADD_STRINGENUM(map, PROPERTYID_, INTENSITYDISTORTMID);
    ADD_STRINGENUM(map, PROPERTYID_, INTENSITYDISTORTHI);

    // properties layer 1
    ADD_STRINGENUM(map, PROPERTYID_, LAYER1TEXTURE);

    // properties layer 2
    ADD_STRINGENUM(map, PROPERTYID_, LAYER2TEXTURE);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER2HEIGHT);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER2HEIGHTVAR);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER2HEIGHTUP);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER2SLOPE);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER2SLOPEVAR);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER2SLOPEUP);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER2DISTORTLO);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER2DISTORTMID);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER2DISTORTHI);

    // properties layer 3
    ADD_STRINGENUM(map, PROPERTYID_, LAYER3TEXTURE);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER3HEIGHT);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER3HEIGHTVAR);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER3HEIGHTUP);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER3SLOPE);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER3SLOPEVAR);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER3SLOPEUP);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER3DISTORTLO);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER3DISTORTMID);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER3DISTORTHI);

    // properties layer 4
    ADD_STRINGENUM(map, PROPERTYID_, LAYER4TEXTURE);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER4HEIGHT);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER4HEIGHTVAR);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER4HEIGHTUP);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER4SLOPE);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER4SLOPEVAR);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER4SLOPEUP);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER4DISTORTLO);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER4DISTORTMID);
    ADD_STRINGENUM(map, PROPERTYID_, LAYER4DISTORTHI);

    // lighting properties
    ADD_STRINGENUM(map, PROPERTYID_, LODERROR);
    ADD_STRINGENUM(map, PROPERTYID_, LODPOWER);

    return true;
}

// ----------------------------------------------------------------------------
string HeightfieldGeomMaterialSplat::setUIElementPropertyValue(const string& elementName,
                                                               const string& value)
{
    EPropertyId propertyId =
        (EPropertyId)Utils::findEnumFromStringEnumMap(getPersistentElementStringEnumMap(), elementName);
    Ogre::Real realValue = Ogre::StringConverter::parseReal(value);
    bool boolValue = Ogre::StringConverter::parseBool(value);

    string outValue;

    switch (propertyId)
    {
    case PROPERTYID_MULTIPROJECT:
    {
        mMultiProject = boolValue;
        updateMaterialType();
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }

    case PROPERTYID_UVSCALEGLOBAL:
    {
        realValue = Utils::clamp(realValue, 0.0001f, 10000.0f);
        mUVScaleGlobal = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_UVSCALELOCAL:
    {
        realValue = Utils::clamp(realValue, 0.0001f, 10000.0f);
        mUVScaleLocal = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_ISOXZ:
    {
        mIsoXZ = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }
    case PROPERTYID_ISOY:
    {
        mIsoY = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }
    case PROPERTYID_ISOFADE:
    {
        realValue = Utils::clamp(realValue, 1.0f, 1000.0f);
        mIsoFade = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }

    case PROPERTYID_NOISETEXTURE:
    {
        outValue = Utils::getRelativePath(value, Utils::getWorkingPath());
        forceReloadTexture(mNoiseTextureFileName, outValue);
        mNoiseTextureFileName = outValue;
        break;
    }
    case PROPERTYID_SLOPEDISTORTLO:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mSlopeDistortLo = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_SLOPEDISTORTMID:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mSlopeDistortMid = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_SLOPEDISTORTHI:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mSlopeDistortHi = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_INTENSITYDISTORTLO:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mIntensityDistortLo = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_INTENSITYDISTORTMID:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mIntensityDistortMid = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_INTENSITYDISTORTHI:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mIntensityDistortHi = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }

    case PROPERTYID_LIGHTANGLESCALE:
    {
        realValue = Utils::clamp(realValue, 0.01f, 100.0f);
        mLightAngleScale = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }

    case PROPERTYID_LAYER1TEXTURE:
    {
        outValue = Utils::getRelativePath(value, Utils::getWorkingPath());
        forceReloadTexture(mLayerProperties[1].textureFileName, outValue);
        mLayerProperties[1].textureFileName = outValue;
        break;
    }

    case PROPERTYID_LAYER2TEXTURE:
    {
        outValue = Utils::getRelativePath(value, Utils::getWorkingPath());
        forceReloadTexture(mLayerProperties[2].textureFileName, outValue);
        mLayerProperties[2].textureFileName = outValue;
        break;
    }
    case PROPERTYID_LAYER2DISTORTLO:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mLayerProperties[2].distortLo = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER2DISTORTMID:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mLayerProperties[2].distortMid = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER2DISTORTHI:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mLayerProperties[2].distortHi = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER2HEIGHT:
    {
        realValue = Utils::clamp(realValue, -10000.0f, 10000.0f);
        mLayerProperties[2].height = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER2HEIGHTVAR:
    {
        realValue = Utils::clamp(realValue, 0.0f, 10000.0f);
        mLayerProperties[2].heightVar = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER2HEIGHTUP:
    {
        mLayerProperties[2].heightUp = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }
    case PROPERTYID_LAYER2SLOPE:
    {
        realValue = Utils::clamp(realValue, -30.0f, 120.0f);
        mLayerProperties[2].slope = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER2SLOPEVAR:
    {
        realValue = Utils::clamp(realValue, 0.0f, 120.0f);
        mLayerProperties[2].slopeVar = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER2SLOPEUP:
    {
        mLayerProperties[2].slopeUp = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }

    case PROPERTYID_LAYER3TEXTURE:
    {
        outValue = Utils::getRelativePath(value, Utils::getWorkingPath());
        forceReloadTexture(mLayerProperties[3].textureFileName, outValue);
        mLayerProperties[3].textureFileName = outValue;
        break;
    }
    case PROPERTYID_LAYER3DISTORTLO:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mLayerProperties[3].distortLo = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER3DISTORTMID:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mLayerProperties[3].distortMid = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER3DISTORTHI:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mLayerProperties[3].distortHi = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER3HEIGHT:
    {
        realValue = Utils::clamp(realValue, -10000.0f, 10000.0f);
        mLayerProperties[3].height = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER3HEIGHTVAR:
    {
        realValue = Utils::clamp(realValue, 0.0f, 10000.0f);
        mLayerProperties[3].heightVar = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER3HEIGHTUP:
    {
        mLayerProperties[3].heightUp = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }
    case PROPERTYID_LAYER3SLOPE:
    {
        realValue = Utils::clamp(realValue, -30.0f, 130.0f);
        mLayerProperties[3].slope = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER3SLOPEVAR:
    {
        realValue = Utils::clamp(realValue, 0.0f, 130.0f);
        mLayerProperties[3].slopeVar = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER3SLOPEUP:
    {
        mLayerProperties[3].slopeUp = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }

    case PROPERTYID_LAYER4TEXTURE:
    {
        outValue = Utils::getRelativePath(value, Utils::getWorkingPath());
        forceReloadTexture(mLayerProperties[4].textureFileName, outValue);
        mLayerProperties[4].textureFileName = outValue;
        break;
    }
    case PROPERTYID_LAYER4DISTORTLO:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mLayerProperties[4].distortLo = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER4DISTORTMID:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mLayerProperties[4].distortMid = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER4DISTORTHI:
    {
        realValue = Utils::clamp(realValue, 0.001f, 1000.0f);
        mLayerProperties[4].distortHi = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER4HEIGHT:
    {
        realValue = Utils::clamp(realValue, 0.0f, 10000.0f);
        mLayerProperties[4].height = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER4HEIGHTVAR:
    {
        realValue = Utils::clamp(realValue, -10000.0f, 10000.0f);
        mLayerProperties[4].heightVar = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER4HEIGHTUP:
    {
        mLayerProperties[4].heightUp = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }
    case PROPERTYID_LAYER4SLOPE:
    {
        realValue = Utils::clamp(realValue, -30.0f, 140.0f);
        mLayerProperties[4].slope = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER4SLOPEVAR:
    {
        realValue = Utils::clamp(realValue, 0.0f, 140.0f);
        mLayerProperties[4].slopeVar = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LAYER4SLOPEUP:
    {
        mLayerProperties[4].slopeUp = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }

    case PROPERTYID_LODERROR:
    {
        realValue = Utils::clamp(realValue, 1.0f, 1000.0f);
        mLODPixelError = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_LODPOWER:
    {
        realValue = Utils::clamp(realValue, 50.0f, 200.0f);
        mLODDistancePower = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }

    default:
    {
        Ogre::LogManager::getSingleton().logMessage(
            _T("Material's setUIElementPropertyValue(): unknown elementName '") + elementName + _T("'"));
        break;
    }
    }

    mHasDirtySettings = true;

    getPersistentElementValueMap()[elementName] = outValue;
    return outValue;
}

// ----------------------------------------------------------------------------
HeightfieldGeomMaterialSplat::HeightfieldGeomMaterialSplat() : HeightfieldGeomMaterial()
{
    LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();
    assert(getEngineCore()->getSkySettings());
    getEngineCore()->getSkySettings()->addListener(this);
    updateMaterialType();
}

// ----------------------------------------------------------------------------
HeightfieldGeomMaterialSplat::~HeightfieldGeomMaterialSplat()
{
    assert(getEngineCore()->getSkySettings());
    getEngineCore()->getSkySettings()->removeListener(this);
}

// ----------------------------------------------------------------------------
void HeightfieldGeomMaterialSplat::onSkySettingsUpdate() { mHasDirtySettings = true; }

// ----------------------------------------------------------------------------
Ogre::MaterialPtr HeightfieldGeomMaterialSplat::createMaterial()
{
    Ogre::ResourcePtr resource = Ogre::MaterialManager::getSingleton().load(
        mMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::MaterialPtr material = std::static_pointer_cast<Ogre::Material>(resource);

    return material;
}

// ----------------------------------------------------------------------------
void HeightfieldGeomMaterialSplat::updateShaderConstantsGeom(
    const HeightfieldGeom* heightfieldGeom, ShaderCustomAutoConstants* shaderCustomAutoConstants,
    bool forceAll)
{
    assert(heightfieldGeom);
    assert(shaderCustomAutoConstants);

    HeightfieldGeomMaterial::updateShaderConstantsGeom(heightfieldGeom, shaderCustomAutoConstants,
                                                       forceAll);

    Ogre::Real worldQuadSize = heightfieldGeom->getWorldQuadSize();

    if (forceAll)
    {
        SkySettings* skySettings = getEngineCore()->getSkySettings();

        {
            Ogre::Real isoScaleXZ = 1000.f;
            Ogre::Real isoScaleY = 1000.f;
            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::ISOSCALES,
                Ogre::Vector4(isoScaleXZ * worldQuadSize, // x iso ramp steepness (reciprocal to thickness)
                              isoScaleY * worldQuadSize,  // y iso ramp steepness (reciprocal to thickness)
                              isoScaleXZ * worldQuadSize, // z iso ramp steepness (reciprocal to thickness)
                              0.0f));                     // not used
        }

        {
            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::ISODOTSCALES,
                Ogre::Vector4(
                    (mIsoXZ ? mIsoFade : 0.0f), // amount of additive greyscale dep. on x iso output
                    (mIsoY ? -mIsoFade : 0.0f), // amount of additive greyscale dep. on y iso output
                    (mIsoXZ ? mIsoFade : 0.0f), // amount of additive greyscale dep. on x iso output
                    (mIsoXZ ? -mIsoFade : 0.0f) +
                        (mIsoY ? 0.5f * mIsoFade
                               : 0.0f))); // amount of additive greyscale indep. of iso output
        }

        {
            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::UVSCALEGLOBAL,
                Ogre::Vector4(1.0f / mUVScaleGlobal, // scale from quad unit U to large low-res texture U
                              1.0f / mUVScaleGlobal, // scale from quad unit V to large low-res texture V
                              0.0f,                  // not used
                              0.0f));                // not used
        }

        {
            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::UVSCALELOCAL,
                Ogre::Vector4(1.0f / mUVScaleLocal, // scale from quad unit U to smaller hi-res texture U
                              1.0f / mUVScaleLocal, // scale from quad unit V to smaller hi-res texture V
                              0.0f,   // scale of distortion in U for hi-res texture. distortion comes from
                                      // the red channel of the global texture
                              0.0f)); // scale of distortion in V for hi-res texture. distortion comes from
                                      // the green channel of the global texture
        }

        {
            Ogre::Real layer2Lo = mLayerProperties[2].height -
                                  (mLayerProperties[2].heightUp ? 0.5f : -0.5f) *
                                      std::max(0.001f, mLayerProperties[2].heightVar);
            Ogre::Real layer2Hi = mLayerProperties[2].height +
                                  (mLayerProperties[2].heightUp ? 0.5f : -0.5f) *
                                      std::max(0.001f, mLayerProperties[2].heightVar);
            Ogre::Real layer3Lo = mLayerProperties[3].height -
                                  (mLayerProperties[3].heightUp ? 0.5f : -0.5f) *
                                      std::max(0.001f, mLayerProperties[3].heightVar);
            Ogre::Real layer3Hi = mLayerProperties[3].height +
                                  (mLayerProperties[3].heightUp ? 0.5f : -0.5f) *
                                      std::max(0.001f, mLayerProperties[3].heightVar);
            Ogre::Real layer4Lo = mLayerProperties[4].height -
                                  (mLayerProperties[4].heightUp ? 0.5f : -0.5f) *
                                      std::max(0.001f, mLayerProperties[4].heightVar);
            Ogre::Real layer4Hi = mLayerProperties[4].height +
                                  (mLayerProperties[4].heightUp ? 0.5f : -0.5f) *
                                      std::max(0.001f, mLayerProperties[4].heightVar);

            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::LAYERHEIGHTSCALE,
                Ogre::Vector4(1.0f / (layer2Hi - layer2Lo), // weight of layer2 (sand)
                              1.0f / (layer3Hi - layer3Lo), // weight of layer3 (rock)
                              1.0f / (layer4Hi - layer4Lo), // weight of layer4 (snow)
                              0.0f));

            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::LAYERHEIGHTBIAS,
                Ogre::Vector4(-layer2Lo / (layer2Hi - layer2Lo), // weight of layer2 (sand)
                              -layer3Lo / (layer3Hi - layer3Lo), // weight of layer3 (rock)
                              -layer4Lo / (layer4Hi - layer4Lo), // weight of layer4 (snow)
                              0.0));
        }

        {
            Ogre::Real layer2SlopeLoTan =
                tan(0.01745f * (mLayerProperties[2].slope -
                                (mLayerProperties[2].slopeUp ? 0.5f : -0.5f) *
                                    std::max(0.001f, mLayerProperties[2].slopeVar)));
            Ogre::Real layer2SlopeHiTan =
                tan(0.01745f * (mLayerProperties[2].slope +
                                (mLayerProperties[2].slopeUp ? 0.5f : -0.5f) *
                                    std::max(0.001f, mLayerProperties[2].slopeVar)));
            Ogre::Real layer3SlopeLoTan =
                tan(0.01745f * (mLayerProperties[3].slope -
                                (mLayerProperties[3].slopeUp ? 0.5f : -0.5f) *
                                    std::max(0.001f, mLayerProperties[3].slopeVar)));
            Ogre::Real layer3SlopeHiTan =
                tan(0.01745f * (mLayerProperties[3].slope +
                                (mLayerProperties[3].slopeUp ? 0.5f : -0.5f) *
                                    std::max(0.001f, mLayerProperties[3].slopeVar)));
            Ogre::Real layer4SlopeLoTan =
                tan(0.01745f * (mLayerProperties[4].slope -
                                (mLayerProperties[4].slopeUp ? 0.5f : -0.5f) *
                                    std::max(0.001f, mLayerProperties[4].slopeVar)));
            Ogre::Real layer4SlopeHiTan =
                tan(0.01745f * (mLayerProperties[4].slope +
                                (mLayerProperties[4].slopeUp ? 0.5f : -0.5f) *
                                    std::max(0.001f, mLayerProperties[4].slopeVar)));

            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::LAYERSLOPESCALE,
                Ogre::Vector4(1.0f / (layer2SlopeHiTan - layer2SlopeLoTan), // weight of layer2 (sand)
                              1.0f / (layer3SlopeHiTan - layer3SlopeLoTan), // weight of layer3 (rock)
                              1.0f / (layer4SlopeHiTan - layer4SlopeLoTan), // weight of layer4 (snow)
                              0.0f));

            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::LAYERSLOPEBIAS,
                Ogre::Vector4(
                    -layer2SlopeLoTan / (layer2SlopeHiTan - layer2SlopeLoTan), // weight of layer2 (sand)
                    -layer3SlopeLoTan / (layer3SlopeHiTan - layer3SlopeLoTan), // weight of layer3 (rock)
                    -layer4SlopeLoTan / (layer4SlopeHiTan - layer4SlopeLoTan), // weight of layer4 (snow)
                    0.0));
        }

        {
            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::LAYERHEIGHTDISTORT,
                Ogre::Matrix4(10.0f * mLayerProperties[2].distortHi, 10.0f * mLayerProperties[2].distortMid,
                              10.0f * mLayerProperties[2].distortLo, 0.0f,
                              10.0f * mLayerProperties[3].distortHi, 10.0f * mLayerProperties[3].distortMid,
                              10.0f * mLayerProperties[3].distortLo, 0.0f,
                              10.0f * mLayerProperties[4].distortHi, 10.0f * mLayerProperties[4].distortMid,
                              10.0f * mLayerProperties[4].distortLo, 0.0f, 0.1f * mSlopeDistortHi,
                              0.1f * mSlopeDistortMid, 0.1f * mSlopeDistortLo, 0.0f));
        }

        {
            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::INTENSITYSCALEGLOBAL,
                Ogre::Vector4(0.01f * mIntensityDistortHi,  // intensity weight of red noise component
                              0.01f * mIntensityDistortMid, // intensity weight of green noise component
                              0.01f * mIntensityDistortLo,  // intensity weight of blue noise component
                              1.0f -
                                  0.005f *
                                      (mIntensityDistortHi + mIntensityDistortMid +
                                       mIntensityDistortLo))); // intensity weight of alpha noise component
        }

        {
            Ogre::ColourValue fogColor = skySettings->getFogColor();
            Ogre::Real fogDistance = skySettings->getFogDistance();
            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::FOGCOLOR,
                Ogre::Vector4(fogColor.r,                  // fog color red
                              fogColor.g,                  // fog color green
                              fogColor.b,                  // fog color blue
                              fogDistance * fogDistance)); // fog begin distance
        }

        {
            Ogre::Real sunHeight = skySettings->getSunHeight();
            Ogre::Real sunDirection = skySettings->getSunDirection();

            Ogre::Real cosHeight = cosf(0.01745f * sunHeight);
            Ogre::Vector3 sunVec(sinf(0.01745f * sunDirection) * cosHeight, sinf(0.01745f * sunHeight),
                                 cosf(0.01745f * sunDirection) * cosHeight);
            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::LIGHTDIRECTION,
                Ogre::Vector4(sunVec.x,                  // x component
                              sunVec.y,                  // y compnent
                              sunVec.z,                  // z component
                              0.1f * mLightAngleScale)); // multiplier for terrain gradient before being
                                                         // normalized and used in lighting calculations
        }

        {
            const Ogre::ColourValue& sunColor = skySettings->getSunColor();
            Ogre::Real sunOverbright = skySettings->getSunOverbright();
            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::SUNCOLOR,
                Ogre::Vector4(sunColor.r * sunOverbright, // sun color red
                              sunColor.g * sunOverbright, // sun color green
                              sunColor.b * sunOverbright, // sun color blue
                              1.0f));                     // not used
        }

        {
            const Ogre::ColourValue& skyColor = skySettings->getSkyColor();
            Ogre::Real skyOverbright = skySettings->getSkyOverbright();
            shaderCustomAutoConstants->setConstant(
                _HeightfieldGeomMaterialSplatNS::SKYCOLOR,
                Ogre::Vector4(skyColor.r * skyOverbright, // sky color red
                              skyColor.g * skyOverbright, // sky color green
                              skyColor.b * skyOverbright, // sky color blue
                              1.0f));                     // not used
        }
    }
}

// ----------------------------------------------------------------------------
void HeightfieldGeomMaterialSplat::updateShaderConstantsGeomTile(
    const HeightfieldGeomTile* heightfieldGeomTile, ShaderCustomAutoConstants* shaderCustomAutoConstants,
    bool forceAll)
{
    assert(heightfieldGeomTile);
    assert(shaderCustomAutoConstants);

    HeightfieldGeomMaterial::updateShaderConstantsGeomTile(heightfieldGeomTile, shaderCustomAutoConstants,
                                                           forceAll);
}

// ----------------------------------------------------------------------------
void HeightfieldGeomMaterialSplat::updateTextures()
{
    Ogre::Technique* technique = getMaterial()->getBestTechnique();
    assert(technique);

    Ogre::Pass* pass = technique->getPass(0);
    assert(pass);

    string group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

    Utils::getTextureFromExternalFile(TEXTUREBASENAME + mNoiseTextureFileName, group,
                                      mNoiseTextureFileName);
    Utils::getTextureFromExternalFile(TEXTUREBASENAME + mLayerProperties[1].textureFileName, group,
                                      mLayerProperties[1].textureFileName);
    Utils::getTextureFromExternalFile(TEXTUREBASENAME + mLayerProperties[2].textureFileName, group,
                                      mLayerProperties[2].textureFileName);
    Utils::getTextureFromExternalFile(TEXTUREBASENAME + mLayerProperties[3].textureFileName, group,
                                      mLayerProperties[3].textureFileName);
    Utils::getTextureFromExternalFile(TEXTUREBASENAME + mLayerProperties[4].textureFileName, group,
                                      mLayerProperties[4].textureFileName);

    // Ogre::TextureManager::getSingleton().unload(mNoiseTextureName);
    // Ogre::TextureManager::getSingleton().unload(mLayerProperties[1].textureName);
    // Ogre::TextureManager::getSingleton().unload(mLayerProperties[2].textureName);
    // Ogre::TextureManager::getSingleton().unload(mLayerProperties[3].textureName);
    // Ogre::TextureManager::getSingleton().unload(mLayerProperties[4].textureName);

    pass->getTextureUnitState(0)->setTextureName(TEXTUREBASENAME + mNoiseTextureFileName);
    pass->getTextureUnitState(1)->setTextureName(TEXTUREBASENAME + mLayerProperties[1].textureFileName);
    pass->getTextureUnitState(2)->setTextureName(TEXTUREBASENAME + mLayerProperties[2].textureFileName);
    pass->getTextureUnitState(3)->setTextureName(TEXTUREBASENAME + mLayerProperties[3].textureFileName);
    pass->getTextureUnitState(4)->setTextureName(TEXTUREBASENAME + mLayerProperties[4].textureFileName);
}

// ----------------------------------------------------------------------------
void HeightfieldGeomMaterialSplat::bindShaderConstants()
{
    Ogre::Technique* technique = getMaterial()->getBestTechnique();
    assert(technique);

    HeightfieldGeomMaterial::bindShaderConstants();

    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::ISOSCALES,
                                        _T("isoScales"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::ISODOTSCALES,
                                        _T("isoDotScales"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::UVSCALEGLOBAL,
                                        _T("uvScaleGlobal"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::UVSCALELOCAL,
                                        _T("uvScaleLocal"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::LAYERHEIGHTSCALE,
                                        _T("layerHeightScale"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::LAYERHEIGHTBIAS,
                                        _T("layerHeightBias"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::LAYERHEIGHTDISTORT,
                                        _T("layerHeightDistort"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::LAYERSLOPESCALE,
                                        _T("layerSlopeScale"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::LAYERSLOPEBIAS,
                                        _T("layerSlopeBias"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::INTENSITYSCALEGLOBAL,
                                        _T("intensityScaleGlobal"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::FOGCOLOR,
                                        _T("fogColor"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::LIGHTDIRECTION,
                                        _T("lightDirection"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::SUNCOLOR,
                                        _T("sunColor"));
    Utils::bindShaderCustomAutoConstant(technique, _HeightfieldGeomMaterialSplatNS::SKYCOLOR,
                                        _T("skyColor"));
}

// ----------------------------------------------------------------------------
void HeightfieldGeomMaterialSplat::forceReloadTexture(const string& oldFileName, const string& newFileName)
{
    if (!oldFileName.empty() && oldFileName.compare(newFileName) != 0)
    {
        Ogre::TextureManager::getSingleton().remove(TEXTUREBASENAME + oldFileName);
    }
    Utils::getTextureFromExternalFile(TEXTUREBASENAME + newFileName,
                                      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, newFileName);
}

// ----------------------------------------------------------------------------
void HeightfieldGeomMaterialSplat::updateMaterialType()
{
    mMaterialName = _T("HeightfieldMaterial_Splat4");
    if (mMultiProject)
    {
        mMaterialName += _T("_MultiProject");
    }
    resetMaterial();
    bindShaderConstants();
}

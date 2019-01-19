/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
  */

#include "ScapeEngineStableHeaders.h"
#include "EngineCore/SettingsDatasetManager.h"
#include "HeightfieldOperationGPUNoiseDrag.h"
#include "HeightfieldOperationTaskPageGPUBrush.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"

#include "HeightfieldSerializer/HeightfieldSerializerIncremental.h"

#include "GPU2DOperation/GPU2DOperationQuad.h"
#include "GPU2DOperation/GPU2DOperation.h"
#include "GPU2DOperation/GPU2DOperationManager.h"
#include "HeightfieldOperationTaskPageGPUBrush.h"

using namespace ScapeEngine;


#define DEGTORAD(a) ((3.141592f / 180.0f) * (a))

// ----------------------------------------------------------------------------
enum EPropertyId
{
    PROPERTYID_STRENGTH,
    PROPERTYID_PENCILMODE,
    PROPERTYID_PATHSPACING,

    PROPERTYID_NOISELARGESTFEAT,
    PROPERTYID_NOISECOARSEROUGH,
    PROPERTYID_NOISEFINEROUGH,
    PROPERTYID_NOISEPEAKIFY,
    PROPERTYID_NOISESEED,
    PROPERTYID_NOISEBLEND,
    PROPERTYID_NOISEALLOWUP,
    PROPERTYID_NOISEALLOWDOWN,

    PROPERTYID_DISTORTLARGESTFEAT,
    PROPERTYID_DISTORTCOARSEROUGH,
    PROPERTYID_DISTORTFINEROUGH,
    PROPERTYID_DISTORTSCALECOARSE,
    PROPERTYID_DISTORTSCALEFINE,
    PROPERTYID_DISTORTTYPE,

    PROPERTYID_DIRECTIONSTRETCHANGLE,
    PROPERTYID_DIRECTIONSTRETCHRATIO,
};

// ----------------------------------------------------------------------------
bool HeightfieldOperationGPUNoiseDrag::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString));
    return true;
}

// ----------------------------------------------------------------------------
bool HeightfieldOperationGPUNoiseDrag::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    ADD_STRINGENUM(map, PROPERTYID_, STRENGTH);
    ADD_STRINGENUM(map, PROPERTYID_, PENCILMODE);
    ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);

    ADD_STRINGENUM(map, PROPERTYID_, NOISELARGESTFEAT);
    ADD_STRINGENUM(map, PROPERTYID_, NOISECOARSEROUGH);
    ADD_STRINGENUM(map, PROPERTYID_, NOISEFINEROUGH);
    // ADD_STRINGENUM(map, PROPERTYID_, NOISEPEAKIFY);
    ADD_STRINGENUM(map, PROPERTYID_, NOISESEED);
    // ADD_STRINGENUM(map, PROPERTYID_, NOISEBLEND);
    ADD_STRINGENUM(map, PROPERTYID_, NOISEALLOWUP);
    ADD_STRINGENUM(map, PROPERTYID_, NOISEALLOWDOWN);

    ADD_STRINGENUM(map, PROPERTYID_, DISTORTLARGESTFEAT);
    ADD_STRINGENUM(map, PROPERTYID_, DISTORTCOARSEROUGH);
    ADD_STRINGENUM(map, PROPERTYID_, DISTORTFINEROUGH);
    ADD_STRINGENUM(map, PROPERTYID_, DISTORTSCALECOARSE);
    ADD_STRINGENUM(map, PROPERTYID_, DISTORTSCALEFINE);
    ADD_STRINGENUM(map, PROPERTYID_, DISTORTTYPE);

    ADD_STRINGENUM(map, PROPERTYID_, DIRECTIONSTRETCHANGLE);
    ADD_STRINGENUM(map, PROPERTYID_, DIRECTIONSTRETCHRATIO);
    return true;
}

// ----------------------------------------------------------------------------
string HeightfieldOperationGPUNoiseDrag::setUIElementPropertyValue(const string& elementName,
                                                                   const string& value)
{
    EPropertyId propertyId =
        (EPropertyId)Utils::findEnumFromStringEnumMap(getPersistentElementStringEnumMap(), elementName);
    int intValue = Ogre::StringConverter::parseInt(value);
    bool boolValue = Ogre::StringConverter::parseBool(value);
    string outValue;

    switch (propertyId)
    {
    case PROPERTYID_STRENGTH:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mStrength = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_PENCILMODE:
    {
        mPencilMode = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }
    case PROPERTYID_PATHSPACING:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mPathSpacing = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }

    case PROPERTYID_NOISELARGESTFEAT:
    {
        intValue = Utils::clamp(intValue, 0, 4000);
        mNoiseLargestFeat = intValue * 1.0f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_NOISECOARSEROUGH:
    {
        intValue = Utils::clamp(intValue, 0, 200);
        mNoiseCoarseRough = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_NOISEFINEROUGH:
    {
        intValue = Utils::clamp(intValue, 0, 200);
        mNoiseFineRough = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_NOISEPEAKIFY:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mNoisePeakify = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_NOISESEED:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mNoiseSeed = intValue * 1.0f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_NOISEBLEND:
    {
        intValue = Utils::clamp(intValue, 0, 100);
        mNoiseBlend = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_NOISEALLOWUP:
    {
        intValue = Utils::clamp(intValue, 0, 100);
        mNoiseAllowUp = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_NOISEALLOWDOWN:
    {
        intValue = Utils::clamp(intValue, 0, 100);
        mNoiseAllowDown = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }

    case PROPERTYID_DISTORTLARGESTFEAT:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mDistortLargestFeat = intValue * 1.0f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_DISTORTCOARSEROUGH:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mDistortCoarseRough = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_DISTORTFINEROUGH:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mDistortFineRough = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_DISTORTSCALECOARSE:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mDistortScaleCoarse = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_DISTORTSCALEFINE:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mDistortScaleFine = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_DISTORTTYPE:
    {
        intValue = Utils::clamp(intValue, 0, 2);
        mDistortType = intValue;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }

    case PROPERTYID_DIRECTIONSTRETCHANGLE:
    {
        intValue = Utils::clamp(intValue, 0, 90);
        mDirectionStretchAngle = intValue * 1.0f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_DIRECTIONSTRETCHRATIO:
    {
        intValue = Utils::clamp(intValue, 1, 100);
        mDirectionStretchRatio = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }

    default:
    {
        Ogre::LogManager::getSingleton().logMessage(
            string(getClassName()) + _T("::setUIElementPropertyValue(): unknown elementName '") +
            elementName + _T("'"));
        break;
    }
    }

    getPersistentElementValueMap()[elementName] = outValue;
    return outValue;
}

// ----------------------------------------------------------------------------
HeightfieldOperationGPUNoiseDrag::HeightfieldOperationGPUNoiseDrag()
    : HeightfieldOperationPolicyBrushInstancerSpacer<
          HeightfieldOperationGPUNoiseDrag,
          HeightfieldOperationPolicySchedulerTaskPageBatch<HeightfieldOperationGPUNoiseDrag,
                                                           HeightfieldOperationBrushBase>>()
{
    allowMultipleBrushInstances(true);

    //	mBrushName = _T("HeightfieldOperationGPUNoise");
    mBrushName = _T("HeightfieldBrush");

    loadPersistentProperties();
}

// ----------------------------------------------------------------------------
HeightfieldOperationTaskPage*
HeightfieldOperationGPUNoiseDrag::createTaskPage(const BrushPageCoords& pageCoords,
                                                 const BrushInstances& brushInstances)
{
    string matName = _T("GPU2DOperationNoiseDrag");

    //	return new HeightfieldOperationTaskPageGPUBrush(_T("GPU2DOperationPencil"), _T(""), true, this,
    //this, pageCoords, brushInstances);
    return new HeightfieldOperationTaskPageGPUBrush(_T("GPU2DOperationNoiseDrag"), _T(""), 8, true, this,
                                                    this, pageCoords, brushInstances);
    //	return new HeightfieldOperationTaskPageGPUBrush(_T("GPU2DOperationBrush"), matName, false, this,
    //this, pageCoords, brushInstances);
}

// ----------------------------------------------------------------------------
// void HeightfieldOperationGPUNoiseDrag::onSetupGPU2DOperationQuadCustom(GPU2DOperationQuadCustomPtr
// quadPtr)
void HeightfieldOperationGPUNoiseDrag::onSetupGPU2DOperationQuadBrush(GPU2DOperationQuadBrushPtr quadPtr)
{

    Ogre::Real lacunarity = 1.92f;
    Ogre::Real noiseHighestOctave = log(mNoiseLargestFeat) / log(2.0f);
    Ogre::Real distortHighestOctave = log(mDistortLargestFeat) / log(2.0f);

    Ogre::Real scale = 1.0f;
    Ogre::Matrix4 noiseOctaveAmps;
    for (int index = 0; index < 16; ++index)
    {
        noiseOctaveAmps[index / 4][index % 4] = scale;
        scale *= 0.5f * Utils::lerp(mNoiseCoarseRough, mNoiseFineRough,
                                    Utils::min(1.0f, index / noiseHighestOctave));
    }

    scale = 1.0f;
    Ogre::Matrix4 distortOctaveAmps;
    for (int index = 0; index < 16; ++index)
    {
        distortOctaveAmps[index / 4][index % 4] = scale;
        scale *= 0.5f * Utils::lerp(mDistortCoarseRough, mDistortFineRough,
                                    Utils::min(1.0f, index / distortHighestOctave));
    }

    quadPtr->setMaterialShaderConstant(_T("noiseOctaveAmps"), noiseOctaveAmps);
    quadPtr->setMaterialShaderConstant(_T("distortOctaveAmps"), distortOctaveAmps);

    quadPtr->setMaterialShaderConstant(
        _T("noiseTrans"), Ogre::Vector4(1.0f / mNoiseLargestFeat, 1.0f / mDistortLargestFeat, 0.0f, 0.0f));

    quadPtr->setMaterialShaderConstant(
        _T("directionTrans"),
        Ogre::Vector4(cos(DEGTORAD(mDirectionStretchAngle)) * sqrtf(mDirectionStretchRatio),
                      sin(DEGTORAD(mDirectionStretchAngle)) * sqrtf(mDirectionStretchRatio),
                      -sin(DEGTORAD(mDirectionStretchAngle)) * sqrtf(mDirectionStretchRatio),
                      cos(DEGTORAD(mDirectionStretchAngle)) * sqrtf(mDirectionStretchRatio)));

    quadPtr->setMaterialShaderConstant(_T("scales"),
                                       Ogre::Vector4(mDistortScaleFine, mDistortScaleCoarse,
                                                     0.5f * mNoiseAllowUp, 0.5f * mNoiseAllowDown));
    quadPtr->setMaterialShaderConstant(_T("lacunarity"), Ogre::Vector4(lacunarity, 0.0f, 0.0f, 0.0f));
    quadPtr->setMaterialShaderConstant(_T("seeds"), Ogre::Vector4(mNoiseSeed + 10, mNoiseSeed, 0.0f, 0.0f));

    Ogre::Vector4 distortTransform;
    switch (mDistortType)
    {
    case 0:
        distortTransform = Ogre::Vector4(-1.0f, 1.0f, 1.0f, 1.0f);
        break;
    case 1:
        distortTransform = Ogre::Vector4(1.0f, -1.0f, 0.0f, 1.0f);
        break;
    case 2:
        distortTransform = Ogre::Vector4(0.0f, 1.0f, 0.0f, 1.0f);
        break;
    }
    quadPtr->setMaterialShaderConstant(_T("distortFunc"), distortTransform);
}

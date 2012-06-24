/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
  */

#include "PCH/stdafx.h"
#include "EngineCore/SettingsDatasetManager.h"
#include "HeightfieldOperationGPUNoise.h"
#include "HeightfieldOperationTaskPageGPUBrush.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
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

IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationGPUNoise);

#define DEGTORAD(a) ((3.141592f / 180.0f)*(a))

// ----------------------------------------------------------------------------
class HeightfieldOperationGPUNoiseTask
{
protected:

};

// ----------------------------------------------------------------------------
enum EPropertyId
{
	PROPERTYID_STRENGTH,
	PROPERTYID_PENCILMODE,
	PROPERTYID_PATHSPACING,

	PROPERTYID_NOISELARGESTFEAT,
	PROPERTYID_NOISECOARSEROUGH,
	PROPERTYID_NOISEFINEROUGH,
	PROPERTYID_NOISESTRETCHANGLE,
	PROPERTYID_NOISESTRETCHRATIO,
	PROPERTYID_NOISEPEAKIFY,
	PROPERTYID_NOISESEED,
	PROPERTYID_NOISETYPE,
	PROPERTYID_NOISEBLEND,

	PROPERTYID_DISTORTLARGESTFEAT,
	PROPERTYID_DISTORTCOARSEROUGH,
	PROPERTYID_DISTORTFINEROUGH,
	PROPERTYID_DISTORTSTRETCHANGLE,
	PROPERTYID_DISTORTSTRETCHRATIO,
	PROPERTYID_DISTORTSCALECOARSE,
	PROPERTYID_DISTORTSCALEFINE,
	PROPERTYID_DISTORTTYPE,
};


// ----------------------------------------------------------------------------
bool HeightfieldOperationGPUNoise::initPersistentElementValueMap(StringStringMap& map) 
{
	setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString)); 
	return true;
}


// ----------------------------------------------------------------------------
bool HeightfieldOperationGPUNoise::initPersistentElementStringEnumMap(StringEnumMap& map) 
{
	ADD_STRINGENUM(map, PROPERTYID_, STRENGTH);
	ADD_STRINGENUM(map, PROPERTYID_, PENCILMODE);
	ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);

	ADD_STRINGENUM(map, PROPERTYID_, NOISELARGESTFEAT);
	ADD_STRINGENUM(map, PROPERTYID_, NOISECOARSEROUGH);
	ADD_STRINGENUM(map, PROPERTYID_, NOISEFINEROUGH);
	ADD_STRINGENUM(map, PROPERTYID_, NOISESTRETCHANGLE);
	ADD_STRINGENUM(map, PROPERTYID_, NOISESTRETCHRATIO);
	ADD_STRINGENUM(map, PROPERTYID_, NOISEPEAKIFY);
	ADD_STRINGENUM(map, PROPERTYID_, NOISESEED);
	ADD_STRINGENUM(map, PROPERTYID_, NOISETYPE);
	ADD_STRINGENUM(map, PROPERTYID_, NOISEBLEND);

	ADD_STRINGENUM(map, PROPERTYID_, DISTORTLARGESTFEAT);
	ADD_STRINGENUM(map, PROPERTYID_, DISTORTCOARSEROUGH);
	ADD_STRINGENUM(map, PROPERTYID_, DISTORTFINEROUGH);
	ADD_STRINGENUM(map, PROPERTYID_, DISTORTSTRETCHANGLE);
	ADD_STRINGENUM(map, PROPERTYID_, DISTORTSTRETCHRATIO);
	ADD_STRINGENUM(map, PROPERTYID_, DISTORTSCALECOARSE);
	ADD_STRINGENUM(map, PROPERTYID_, DISTORTSCALEFINE);
	ADD_STRINGENUM(map, PROPERTYID_, DISTORTTYPE);

	return true;
}


// ----------------------------------------------------------------------------
string HeightfieldOperationGPUNoise::setUIElementPropertyValue(const string& elementName, const string& value) 
{
	EPropertyId propertyId = (EPropertyId)Utils::findEnumFromStringEnumMap(getPersistentElementStringEnumMap(), elementName);
	int intValue = Ogre::StringConverter::parseInt(value);
	bool boolValue = Ogre::StringConverter::parseBool(value);
	string outValue;

	switch (propertyId)
	{
	case PROPERTYID_STRENGTH: {intValue = Utils::clamp(intValue, 0, 1000); mStrength = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_PENCILMODE: {mPencilMode = boolValue; outValue = Ogre::StringConverter::toString(boolValue); break;}
	case PROPERTYID_PATHSPACING: {intValue = Utils::clamp(intValue, 0, 1000); mPathSpacing = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}

	case PROPERTYID_NOISELARGESTFEAT: {intValue = Utils::clamp(intValue, 0, 4000); mNoiseLargestFeat = intValue * 1.0f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_NOISECOARSEROUGH: {intValue = Utils::clamp(intValue, 0, 200); mNoiseCoarseRough = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_NOISEFINEROUGH: {intValue = Utils::clamp(intValue, 0, 200); mNoiseFineRough = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_NOISESTRETCHANGLE: {intValue = Utils::clamp(intValue, 0, 10000); mNoiseStretchAngle = intValue * 1.0f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_NOISESTRETCHRATIO: {intValue = Utils::clamp(intValue, 1, 10000); mNoiseStretchRatio = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_NOISEPEAKIFY: {intValue = Utils::clamp(intValue, 0, 1000); mNoisePeakify = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_NOISESEED: {intValue = Utils::clamp(intValue, 0, 1000); mNoiseSeed = intValue * 1.0f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_NOISETYPE: {intValue = Utils::clamp(intValue, 0, 5); mNoiseType = intValue; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_NOISEBLEND: {intValue = Utils::clamp(intValue, 0, 100); mNoiseBlend = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}

	case PROPERTYID_DISTORTLARGESTFEAT: {intValue = Utils::clamp(intValue, 0, 1000); mDistortLargestFeat = intValue * 1.0f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_DISTORTCOARSEROUGH: {intValue = Utils::clamp(intValue, 0, 1000); mDistortCoarseRough = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_DISTORTFINEROUGH: {intValue = Utils::clamp(intValue, 0, 1000); mDistortFineRough = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_DISTORTSTRETCHANGLE: {intValue = Utils::clamp(intValue, 0, 10000); mDistortStretchAngle = intValue * 1.0f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_DISTORTSTRETCHRATIO: {intValue = Utils::clamp(intValue, 1, 10000); mDistortStretchRatio = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_DISTORTSCALECOARSE: {intValue = Utils::clamp(intValue, 0, 1000); mDistortScaleCoarse = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_DISTORTSCALEFINE: {intValue = Utils::clamp(intValue, 0, 1000); mDistortScaleFine = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_DISTORTTYPE: {intValue = Utils::clamp(intValue, 0, 2); mDistortType = intValue; outValue = Ogre::StringConverter::toString(intValue); break;}

	default: {Ogre::LogManager::getSingleton().logMessage(string(getClassName()) + _T("::setUIElementPropertyValue(): unknown elementName '") + elementName + _T("'")); break;}
	}

	getPersistentElementValueMap()[elementName] = outValue;
	return outValue;
}


// ----------------------------------------------------------------------------
HeightfieldOperationGPUNoise::HeightfieldOperationGPUNoise()
:	HeightfieldOperationPolicyBrushInstancerSpacer<HeightfieldOperationGPUNoise, HeightfieldOperationPolicySchedulerTaskPageBatch<HeightfieldOperationGPUNoise, HeightfieldOperationBrushBase> >()
{
	allowMultipleBrushInstances(true);
//	mBrushName = _T("HeightfieldOperationGPUNoise");
	mBrushName = _T("HeightfieldBrush");

	LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();
}

#include "Input/InputManager.h"
// ----------------------------------------------------------------------------
HeightfieldOperationTaskPage* HeightfieldOperationGPUNoise::createTaskPage(const BrushPageCoords& pageCoords, const BrushInstances& brushInstances)
{
	string matName = _T("GPU2DOperationNoiseMask");
	switch (mNoiseType)
	{
	case 0: matName += _T("_PERLIN"); break;
	case 1: matName += _T("_RIDGED"); break;
	case 2: matName += _T("_BILLOWED"); break;
	case 3: matName += _T("_IQ"); break;
	case 4: matName += _T("_EROSION1"); break;
	case 5: matName += _T("_EROSION2"); break;
	default: assert(false && _T("unknown noise type"));
	}

	if (getEngineCore()->getInputManager()->getButton(ButtonId::MATERIAL_RESET)->isJustPressed())
	{
		Utils::reloadMaterial(matName);
	}


//	return new HeightfieldOperationTaskPageGPUBrush(_T("GPU2DOperationPencil"), _T(""), true, this, this, pageCoords, brushInstances);
	return new HeightfieldOperationTaskPageGPUBrush(_T("GPU2DOperationBrush"), matName, 16, false, this, this, pageCoords, brushInstances);
//	return new HeightfieldOperationTaskPageGPUBrush(matName, _T(""), true, this, this, pageCoords, brushInstances);
}

// ----------------------------------------------------------------------------
void HeightfieldOperationGPUNoise::onSetupGPU2DOperationQuadCustom(GPU2DOperationQuadCustomPtr quadPtr)
{

	Ogre::Real lacunarity = 1.92f;
	Ogre::Real noiseHighestOctave = log(mNoiseLargestFeat) / log(2.0f);
	Ogre::Real distortHighestOctave = log(mDistortLargestFeat) / log(2.0f);

	Ogre::Real scale = 1.0f;
	Ogre::Matrix4 noiseOctaveAmps;
	for (int index = 0; index < 16; ++index) 
	{
		noiseOctaveAmps[index / 4][index % 4] = scale;
		scale *= 0.5f * Utils::lerp(mNoiseCoarseRough, mNoiseFineRough, Utils::min(1.0f, index / noiseHighestOctave));
	}

	scale = 1.0f;
	Ogre::Matrix4 distortOctaveAmps;
	for (int index = 0; index < 16; ++index) 
	{
		distortOctaveAmps[index / 4][index % 4] = scale;
		scale *= 0.5f * Utils::lerp(mDistortCoarseRough, mDistortFineRough, Utils::min(1.0f, index / distortHighestOctave));
	}

	quadPtr->setMaterialShaderConstant(_T("noiseOctaveAmps"), noiseOctaveAmps);
	quadPtr->setMaterialShaderConstant(_T("distortOctaveAmps"), distortOctaveAmps);

	quadPtr->setMaterialShaderConstant(_T("noiseTrans"), Ogre::Vector4(
		cos(DEGTORAD(mNoiseStretchAngle))/mNoiseLargestFeat*sqrtf(mNoiseStretchRatio), 
		sin(DEGTORAD(mNoiseStretchAngle))/mNoiseLargestFeat/sqrtf(mNoiseStretchRatio), 
	   -sin(DEGTORAD(mNoiseStretchAngle))/mNoiseLargestFeat*sqrtf(mNoiseStretchRatio), 
		cos(DEGTORAD(mNoiseStretchAngle))/mNoiseLargestFeat/sqrtf(mNoiseStretchRatio)));

	quadPtr->setMaterialShaderConstant(_T("distortTrans"), Ogre::Vector4(
		cos(DEGTORAD(mDistortStretchAngle))/mDistortLargestFeat*sqrtf(mDistortStretchRatio), 
		sin(DEGTORAD(mDistortStretchAngle))/mDistortLargestFeat/sqrtf(mDistortStretchRatio), 
	   -sin(DEGTORAD(mDistortStretchAngle))/mDistortLargestFeat*sqrtf(mDistortStretchRatio), 
		cos(DEGTORAD(mDistortStretchAngle))/mDistortLargestFeat/sqrtf(mDistortStretchRatio)));

	quadPtr->setMaterialShaderConstant(_T("scales"), Ogre::Vector4(mDistortScaleFine, mDistortScaleCoarse, mNoiseBlend, mNoisePeakify));
	quadPtr->setMaterialShaderConstant(_T("lacunarity"), Ogre::Vector4(lacunarity, 0.0f, 0.0f, 0.0f));
	quadPtr->setMaterialShaderConstant(_T("seeds"), Ogre::Vector4(mNoiseSeed + 10, mNoiseSeed, 0.0f, 0.0f));

	Ogre::Vector4 distortTransform;
	switch (mDistortType)
	{
		case 0: distortTransform = Ogre::Vector4(-1.0f,  1.0f,  1.0f,  1.0f); break;
		case 1: distortTransform = Ogre::Vector4( 1.0f, -1.0f,  0.0f,  1.0f); break;
		case 2: distortTransform = Ogre::Vector4( 0.0f,  1.0f,  0.0f,  1.0f); break;
	}
	quadPtr->setMaterialShaderConstant(_T("distortFunc"), distortTransform);

}

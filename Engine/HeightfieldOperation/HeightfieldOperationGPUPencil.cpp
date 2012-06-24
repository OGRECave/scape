/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
  */

#include "PCH/stdafx.h"
#include "EngineCore/SettingsDatasetManager.h"
#include "HeightfieldOperationGPUPencil.h"
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

IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationGPUPencil);

// ----------------------------------------------------------------------------
class HeightfieldOperationGPUPencilTask
{
protected:

};

// ----------------------------------------------------------------------------
enum EPropertyId
{
	PROPERTYID_STRENGTH,
	PROPERTYID_PATHSPACING,
	PROPERTYID_PENCILMODE,
	PROPERTYID_LEVELEFFECT,
};


// ----------------------------------------------------------------------------
bool HeightfieldOperationGPUPencil::initPersistentElementValueMap(StringStringMap& map) 
{
	setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString)); 
	return true;
}


// ----------------------------------------------------------------------------
bool HeightfieldOperationGPUPencil::initPersistentElementStringEnumMap(StringEnumMap& map) 
{
	ADD_STRINGENUM(map, PROPERTYID_, STRENGTH);
	ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);
	ADD_STRINGENUM(map, PROPERTYID_, PENCILMODE);
	ADD_STRINGENUM(map, PROPERTYID_, LEVELEFFECT);

	return true;
}


// ----------------------------------------------------------------------------
string HeightfieldOperationGPUPencil::setUIElementPropertyValue(const string& elementName, const string& value) 
{
	EPropertyId propertyId = (EPropertyId)Utils::findEnumFromStringEnumMap(getPersistentElementStringEnumMap(), elementName);
	int intValue = Ogre::StringConverter::parseInt(value);
	bool boolValue = Ogre::StringConverter::parseBool(value);
	string outValue;

	switch (propertyId)
	{
	case PROPERTYID_STRENGTH: {intValue = Utils::clamp(intValue, 0, 1000); mStrength = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_PATHSPACING: {intValue = Utils::clamp(intValue, 0, 1000); mPathSpacing = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	case PROPERTYID_PENCILMODE: {mPencilMode = boolValue; outValue = Ogre::StringConverter::toString(boolValue); break;}
	case PROPERTYID_LEVELEFFECT: {intValue = Utils::clamp(intValue, 0, 100); mLevelEffect = intValue * 0.01f; outValue = Ogre::StringConverter::toString(intValue); break;}
	default: {Ogre::LogManager::getSingleton().logMessage(string(getClassName()) + _T("::setUIElementPropertyValue(): unknown elementName '") + elementName + _T("'")); break;}
	}

	getPersistentElementValueMap()[elementName] = outValue;
	return outValue;
}


// ----------------------------------------------------------------------------
HeightfieldOperationGPUPencil::HeightfieldOperationGPUPencil()
:	HeightfieldOperationPolicyBrushInstancerSpacer<HeightfieldOperationGPUPencil, HeightfieldOperationPolicySchedulerTaskPageBatch<HeightfieldOperationGPUPencil, HeightfieldOperationBrushBase> >()
{
	allowMultipleBrushInstances(true);
//	mBrushName = _T("HeightfieldOperationGPUBrush");
	mBrushName = _T("HeightfieldBrush");

	LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();
}

// ----------------------------------------------------------------------------
HeightfieldOperationTaskPage* HeightfieldOperationGPUPencil::createTaskPage(const BrushPageCoords& pageCoords, const BrushInstances& brushInstances)
{
	return new HeightfieldOperationTaskPageGPUBrush(_T("GPU2DOperationPencil"), _T(""), 16, true, this, this, pageCoords, brushInstances);
}

// ----------------------------------------------------------------------------
void HeightfieldOperationGPUPencil::onSetupGPU2DOperationQuadBrush(GPU2DOperationQuadBrushPtr quadPtr)
{
	quadPtr->setMaterialShaderConstant(_T("brushParams"), Ogre::Vector4(mLevelEffect, 0.0f, 0.0f, 0.0f));
}

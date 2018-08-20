/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
  */

#include "PCH/stdafx.h"
#include "EngineCore/SettingsDatasetManager.h"
#include "HeightfieldOperationGPUBrush.h"
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

IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationGPUBrush);

// ----------------------------------------------------------------------------
class HeightfieldOperationGPUBrushTask
{
protected:
};

// ----------------------------------------------------------------------------
enum EPropertyId
{
    PROPERTYID_STRENGTH,
    PROPERTYID_PATHSPACING,
    PROPERTYID_PENCILMODE,
    PROPERTYID_SHOWDEBUG
};

// ----------------------------------------------------------------------------
bool HeightfieldOperationGPUBrush::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString));
    return true;
}

// ----------------------------------------------------------------------------
bool HeightfieldOperationGPUBrush::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    ADD_STRINGENUM(map, PROPERTYID_, STRENGTH);
    ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);
    ADD_STRINGENUM(map, PROPERTYID_, PENCILMODE);
    ADD_STRINGENUM(map, PROPERTYID_, SHOWDEBUG);

    return true;
}

// ----------------------------------------------------------------------------
string HeightfieldOperationGPUBrush::setUIElementPropertyValue(const string& elementName, const string& value)
{
    EPropertyId propertyId
        = (EPropertyId)Utils::findEnumFromStringEnumMap(getPersistentElementStringEnumMap(), elementName);
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
    case PROPERTYID_PATHSPACING:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mPathSpacing = intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_PENCILMODE:
    {
        mPencilMode = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }
    case PROPERTYID_SHOWDEBUG:
    {
        mShowDebug = boolValue;
        outValue = Ogre::StringConverter::toString(boolValue);
        break;
    }
    default:
    {
        Ogre::LogManager::getSingleton().logMessage(string(getClassName())
            + _T("::setUIElementPropertyValue(): unknown elementName '") + elementName + _T("'"));
        break;
    }
    }

    getPersistentElementValueMap()[elementName] = outValue;
    return outValue;
}

// ----------------------------------------------------------------------------
HeightfieldOperationGPUBrush::HeightfieldOperationGPUBrush()
    : HeightfieldOperationPolicyBrushInstancerSpacer<HeightfieldOperationGPUBrush,
          HeightfieldOperationPolicySchedulerTaskPageBatch<HeightfieldOperationGPUBrush,
                                                         HeightfieldOperationBrushBase> >()
    , mShowDebug(false)
{
    //	mBrushName = _T("HeightfieldOperationGPUBrush");
    mBrushName = _T("HeightfieldBrush");

    LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();
}

// ----------------------------------------------------------------------------
HeightfieldOperationTaskPage* HeightfieldOperationGPUBrush::createTaskPage(
    const BrushPageCoords& pageCoords, const BrushInstances& brushInstances)
{
    HeightfieldOperationTaskPageGPUBrush* page = new HeightfieldOperationTaskPageGPUBrush(
        _T("GPU2DOperationBrush"), _T("GPU2DOperationAddMask"), 16, false, this, this, pageCoords, brushInstances);
    //	HeightfieldOperationTaskPageGPUBrush* page = new
    //HeightfieldOperationTaskPageGPUBrush(_T("GPU2DOperationAddMask"), _T(""), true, this, this, pageCoords,
    //brushInstances);

    page->setBorderSize(64, 64);
    return page;
}

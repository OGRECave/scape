/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
  */

#include "PCH/stdafx.h"
#include "EngineCore/SettingsDatasetManager.h"
#include "HeightfieldOperationCPUBrush2.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"
#include "HeightfieldOperationTaskDirect.h"

#include "HeightfieldSerializer/HeightfieldSerializerIncremental.h"

using namespace ScapeEngine;

IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationCPUBrush2);

// ----------------------------------------------------------------------------
class HeightfieldOperationTaskPageCPUBrush : public HeightfieldOperationTaskPage
{
public:
    // ----------------------------------------------------------------------------
    HeightfieldOperationTaskPageCPUBrush(
        HeightfieldOperationBrush* operation, const BrushPageCoords pageCoords, const BrushInstances& brushInstances)
        : HeightfieldOperationTaskPage(operation, pageCoords, brushInstances)
    {
    }

    // ----------------------------------------------------------------------------
    virtual void tickPending() { mCurrentState = STATE_ACTIVE; }
    // ----------------------------------------------------------------------------
    virtual void tickActive()
    {
        HeightfieldBufferPage* inOutPage = mOperation->getInOutHeightfieldBuffer()->getPage(
            mPageCoords.first, mPageCoords.second, HeightfieldBuffer::PAGEACCESSMODE_READONLY);
        if (!inOutPage)
            return;

        Ogre::Rect pageRect = inOutPage->getAbsoluteRect();

        // important to make sure all quad textures are unlocked!
        // inOutPage->unlockAll();

        // GPU2DOperationQuadBrushPtr brushQuadPtr;

        unsigned long startMillis = Ogre::Root::getSingleton().getTimer()->getMilliseconds();

        while (!mBrushInstances.empty())
        {
            BrushInstance& brushInstance = mBrushInstances.front();

            Ogre::Vector3 position = brushInstance.position;

            Ogre::Real sizeChangeScale = brushInstance.strength
                * mOperation->getStrength(); // * 0.01f;//* changeSpeed;// * (1.0f -
                                             // expf(-getEngineCore()->getTimeSinceLastFrame() / changeSpeed));
            Ogre::Real innerRadius = brushInstance.innerRadius;
            Ogre::Real outerRadius = brushInstance.outerRadius;

            Ogre::Rect newUpdateRect((long)(position.x - brushInstance.outerRadius - 2),
                (long)(position.z - brushInstance.outerRadius - 2), (long)(position.x + brushInstance.outerRadius + 2),
                (long)(position.z + brushInstance.outerRadius + 2));

            mUpdatedRect = Utils::unionTRect(mUpdatedRect, newUpdateRect);

            Ogre::Rect relativeRect
                = Utils::translatedRect(Utils::intersectTRect(newUpdateRect, pageRect), -pageRect.left, -pageRect.top);
            Ogre::Real cx = brushInstance.position.x - pageRect.left;
            Ogre::Real cz = brushInstance.position.z - pageRect.top;

            Ogre::Real scale = 1.0f / ((innerRadius * innerRadius) - (outerRadius * outerRadius));
            Ogre::Real bias = -(outerRadius * outerRadius) * scale;

            // Ogre::Real heightscale = inOutPage->getHeightfieldBufferSet()->getHeightRangeMax() -
            // inOutPage->getHeightfieldBufferSet()->getHeightRangeMin();
            // Ogre::Real cx = mBrushInstance.position.x - (Ogre::Real)editRect.left;
            // Ogre::Real cy = mBrushInstance.position.y;
            // Ogre::Real cz = mBrushInstance.position.z - (Ogre::Real)editRect.top;

            Ogre::PixelBox pixelBox = inOutPage->getRawHeightData();
            inOutPage->increaseVersion();

            for (int x = (int)relativeRect.left; x < (int)relativeRect.right; ++x)
            {
                for (int z = (int)relativeRect.top; z < (int)relativeRect.bottom; ++z)
                {
                    Ogre::ColourValue colourValue;
                    void* pixelPtr = (Ogre::uint8*)pixelBox.data
                        + Ogre::PixelUtil::getNumElemBytes(pixelBox.format) * (z * pageRect.width() + x);
                    Ogre::PixelUtil::unpackColour(&colourValue, pixelBox.format, pixelPtr);
                    Ogre::Real hy = colourValue.r;
                    Ogre::Real dx = (Ogre::Real)x - cx;
                    // Ogre::Real dy = hy * heightscale - cy;
                    Ogre::Real dz = (Ogre::Real)z - cz;

                    // Ogre::Real s = powf(dx * dx + dz * dz, 0.5f);
                    // Ogre::Real delta = Utils::clamp(s * scale + bias, 0.0f, 1.0f);
                    // delta = sizeChangeScale * delta * delta;

                    Ogre::Real s = dx * dx + dz * dz;
                    Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
                    // delta = delta * Utils::clamp(1.0f - dy / innerRadius, 0.0f, 1.0f); //tunneler
                    colourValue.r = Utils::clamp(hy + delta, 0.0f, 1.0f);
                    Ogre::PixelUtil::packColour(colourValue, pixelBox.format, pixelPtr);
                }
            }
            mBrushInstances.pop_front();

            if (Ogre::Root::getSingleton().getTimer()->getMilliseconds() - startMillis >= MINWORKMILLIS)
            {
                break;
            }
        }

        if (mBrushInstances.empty())
        {
            mCurrentState = STATE_COMPLETED;
        }
    }
};

// ----------------------------------------------------------------------------
enum EPropertyId
{
    PROPERTYID_STRENGTH,
    PROPERTYID_PENCILMODE,
    PROPERTYID_PATHSPACING,
    PROPERTYID_LEDGEMODE,
};

// ----------------------------------------------------------------------------
bool HeightfieldOperationCPUBrush2::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString));
    return true;
}

// ----------------------------------------------------------------------------
bool HeightfieldOperationCPUBrush2::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    ADD_STRINGENUM(map, PROPERTYID_, STRENGTH);
    ADD_STRINGENUM(map, PROPERTYID_, PENCILMODE);
    ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);
    ADD_STRINGENUM(map, PROPERTYID_, LEDGEMODE);

    return true;
}

// ----------------------------------------------------------------------------
string HeightfieldOperationCPUBrush2::setUIElementPropertyValue(const string& elementName, const string& value)
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
    case PROPERTYID_LEDGEMODE:
    {
        mLedgeMode = boolValue;
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
HeightfieldOperationCPUBrush2::HeightfieldOperationCPUBrush2()
    : HeightfieldOperationPolicyBrushInstancerSpacer<HeightfieldOperationCPUBrush2,
          HeightfieldOperationPolicySchedulerTaskPageBatch<HeightfieldOperationCPUBrush2,
                                                         HeightfieldOperationBrushBase> >()
{
    allowMultipleBrushInstances(true);

    //	mBrushName = _T("HeightfieldOperationCPUBrush");
    mBrushName = _T("HeightfieldBrush");

    LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();
}

// ----------------------------------------------------------------------------
HeightfieldOperationTaskPage* HeightfieldOperationCPUBrush2::createTaskPage(
    const BrushPageCoords& pageCoords, const BrushInstances& brushInstances)
{
    return new HeightfieldOperationTaskPageCPUBrush(this, pageCoords, brushInstances);
}

/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
  */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"

#include "HeightfieldSerializer/HeightfieldSerializerIncremental.h"

#include "HeightfieldOperationCPUSmooth.h"

using namespace ScapeEngine;

enum EPropertyId
{
    PROPERTYID_STRENGTH,
    PROPERTYID_PENCILMODE,
    PROPERTYID_PATHSPACING,
};

bool HeightfieldOperationCPUSmooth::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString));
    return true;
}

bool HeightfieldOperationCPUSmooth::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    ADD_STRINGENUM(map, PROPERTYID_, STRENGTH);
    ADD_STRINGENUM(map, PROPERTYID_, PENCILMODE);
    ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);

    return true;
}

string HeightfieldOperationCPUSmooth::setUIElementPropertyValue(const string& elementName,
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

HeightfieldOperationCPUSmooth::HeightfieldOperationCPUSmooth() : HeightfieldOperationCPU()
{
    mBrushName = "HeightfieldOperationCPUSmooth";

    loadPersistentProperties();
}

void HeightfieldOperationCPUSmooth::applyPrimary(const Ogre::Vector3& position, Ogre::Real strength)
{
    assert(mBrush);
    HeightfieldBuffer* heightfieldBuffer = mBrush->getHeightfieldGeom()->getHeightfieldBuffer();

    Ogre::Real changeSpeed = 0.5f * mStrength;
    Ogre::Real sizeChangeScale =
        strength * changeSpeed; // * (1.0f - expf(-getEngineCore()->getTimeSinceLastFrame() / changeSpeed));
    Ogre::Real innerRadius = mHeightfieldBrushSettings.getInnerRadius();
    Ogre::Real outerRadius = mHeightfieldBrushSettings.getOuterRadius();
    int size = (int)(2 * outerRadius) + 2;
    int left = (int)position.x;
    int top = (int)position.z;
    Ogre::Rect editRect(
        std::max(0, left - size / 2), std::max(0, top - size / 2),
        std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCount(), left + size / 2),
        std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCount(), top + size / 2));
    int elementCount = std::max((long)1, editRect.width() * editRect.height());
    Ogre::Real* dataIn = new Ogre::Real[3 * elementCount];
    Ogre::Real* dataOut = new Ogre::Real[3 * elementCount];

    Ogre::PixelBox pixelBox(Utils::rectToBox(editRect), Ogre::PF_FLOAT32_R, dataIn);
    heightfieldBuffer->copyTo(pixelBox);
    Ogre::Real scale = 1.0f / (innerRadius * innerRadius - outerRadius * outerRadius);
    Ogre::Real bias = -outerRadius * outerRadius * scale;

    // Ogre::Real heightscale = heightfieldBuffer->getHeightRangeMax() -
    // heightfieldBuffer->getHeightRangeMin();
    Ogre::Real cx = position.x - (Ogre::Real)editRect.left;
    // Ogre::Real cy = position.y;
    Ogre::Real cz = position.z - (Ogre::Real)editRect.top;

    int width = editRect.width();

    if (editRect.height() > 0)
    {
        int lastRowIndex = ((int)editRect.height() - 1) * width;
        for (int x = 0; x < (int)editRect.width(); ++x)
        {
            dataOut[x] = dataIn[x];
            dataOut[x + lastRowIndex] = dataIn[x + lastRowIndex];
        }
    }

    if (editRect.width() > 0)
    {
        int lastColumnIndex = (int)editRect.width() - 1;
        int index = width;
        for (int z = 1; z < (int)editRect.height() - 1; ++z)
        {
            dataOut[index] = dataIn[index];
            dataOut[index + lastColumnIndex] = dataIn[index + lastColumnIndex];
            index += width;
        }
    }

    for (int x = 1; x < (int)editRect.width() - 1; ++x)
    {
        for (int z = 1; z < (int)editRect.height() - 1; ++z)
        {
            int index = z * width + x;
            Ogre::Real hy = dataIn[index];
            Ogre::Real avgHeight = 0.2f * (hy + dataIn[index + 1] + dataIn[index - 1] +
                                           dataIn[index + width] + dataIn[index - width]);

            Ogre::Real dx = (Ogre::Real)x - cx;
            Ogre::Real dz = (Ogre::Real)z - cz;

            Ogre::Real s = dx * dx + dz * dz;
            Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
            delta = delta * (avgHeight - hy);
            dataOut[index] = Utils::clamp(hy + delta, 0.0f, 1.0f);
        }
    }

    pixelBox.data = reinterpret_cast<Ogre::uchar*>(dataOut);
    heightfieldBuffer->updateFrom(pixelBox);
    heightfieldBuffer->revalidate();
    delete dataIn;
    delete dataOut;
}

void HeightfieldOperationCPUSmooth::applySecondary(const Ogre::Vector3& position, Ogre::Real strength)
{
    assert(mBrush);
    HeightfieldBuffer* heightfieldBuffer = mBrush->getHeightfieldGeom()->getHeightfieldBuffer();

    Ogre::Real changeSpeed = 0.25f * mStrength;
    Ogre::Real sizeChangeScale =
        strength * changeSpeed; // * (1.0f - expf(-getEngineCore()->getTimeSinceLastFrame() / changeSpeed));
    Ogre::Real innerRadius = mHeightfieldBrushSettings.getInnerRadius();
    Ogre::Real outerRadius = mHeightfieldBrushSettings.getOuterRadius();
    int size = (int)(2 * outerRadius) + 2;
    int left = (int)position.x;
    int top = (int)position.z;
    Ogre::Rect editRect(
        std::max(0, left - size / 2), std::max(0, top - size / 2),
        std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCount(), left + size / 2),
        std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCount(), top + size / 2));
    int elementCount = std::max((long)1, editRect.width() * editRect.height());
    Ogre::Real* dataIn = new Ogre::Real[3 * elementCount];
    Ogre::Real* dataOut = new Ogre::Real[3 * elementCount];

    Ogre::PixelBox pixelBox(Utils::rectToBox(editRect), Ogre::PF_FLOAT32_R, dataIn);
    heightfieldBuffer->copyTo(pixelBox);
    Ogre::Real scale = 1.0f / (innerRadius * innerRadius - outerRadius * outerRadius);
    Ogre::Real bias = -outerRadius * outerRadius * scale;

    // Ogre::Real heightscale = heightfieldBuffer->getHeightRangeMax() -
    // heightfieldBuffer->getHeightRangeMin();
    Ogre::Real cx = position.x - (Ogre::Real)editRect.left;
    // Ogre::Real cy = position.y;
    Ogre::Real cz = position.z - (Ogre::Real)editRect.top;

    int width = editRect.width();

    if (editRect.height() > 0)
    {
        int lastRowIndex = ((int)editRect.height() - 1) * width;
        for (int x = 0; x < (int)editRect.width(); ++x)
        {
            dataOut[x] = dataIn[x];
            dataOut[x + lastRowIndex] = dataIn[x + lastRowIndex];
        }
    }

    if (editRect.width() > 0)
    {
        int lastColumnIndex = (int)editRect.width() - 1;
        int index = width;
        for (int z = 1; z < (int)editRect.height() - 1; ++z)
        {
            dataOut[index] = dataIn[index];
            dataOut[index + lastColumnIndex] = dataIn[index + lastColumnIndex];
            index += width;
        }
    }

    for (int x = 1; x < (int)editRect.width() - 1; ++x)
    {
        for (int z = 1; z < (int)editRect.height() - 1; ++z)
        {
            int index = z * width + x;
            Ogre::Real hy = dataIn[index];
            Ogre::Real avgHeight = 0.2f * (hy + dataIn[index + 1] + dataIn[index - 1] +
                                           dataIn[index + width] + dataIn[index - width]);

            Ogre::Real dx = (Ogre::Real)x - cx;
            Ogre::Real dz = (Ogre::Real)z - cz;

            Ogre::Real s = dx * dx + dz * dz;
            Ogre::Real delta = sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
            delta = delta * (hy - avgHeight);
            dataOut[index] = Utils::clamp(hy + delta, 0.0f, 1.0f);
        }
    }

    pixelBox.data = reinterpret_cast<Ogre::uchar*>(dataOut);
    heightfieldBuffer->updateFrom(pixelBox);
    heightfieldBuffer->revalidate();
    delete dataIn;
    delete dataOut;
}

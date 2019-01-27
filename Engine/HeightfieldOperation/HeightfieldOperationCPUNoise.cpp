/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
  */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldOperationCPUNoise.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"

#include "HeightfieldSerializer/HeightfieldSerializerIncremental.h"

using namespace ScapeEngine;


Ogre::Vector3 HeightfieldOperationCPUNoise::mNoiseOrigin;

enum EPropertyId
{
    PROPERTYID_STRENGTH,
    PROPERTYID_PENCILMODE,
    PROPERTYID_PATHSPACING,
    PROPERTYID_MIN_RANGE,
    PROPERTYID_MAX_RANGE,
    PROPERTYID_HI_AMP,
    PROPERTYID_MID_AMP,
};

bool HeightfieldOperationCPUNoise::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString));
    return true;
}

bool HeightfieldOperationCPUNoise::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    ADD_STRINGENUM(map, PROPERTYID_, STRENGTH);
    ADD_STRINGENUM(map, PROPERTYID_, PENCILMODE);
    ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);
    ADD_STRINGENUM(map, PROPERTYID_, MIN_RANGE);
    ADD_STRINGENUM(map, PROPERTYID_, MAX_RANGE);
    ADD_STRINGENUM(map, PROPERTYID_, HI_AMP);
    ADD_STRINGENUM(map, PROPERTYID_, MID_AMP);

    mNoiseOrigin = Ogre::Vector3(0.0f);

    return true;
}

string HeightfieldOperationCPUNoise::setUIElementPropertyValue(const string& elementName,
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
    case PROPERTYID_MIN_RANGE:
    {
        intValue = Utils::clamp(intValue, 1, 1000);
        mPropMinRange = (Ogre::Real)intValue;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_MAX_RANGE:
    {
        intValue = Utils::clamp(intValue, 1, 1000);
        mPropMaxRange = (Ogre::Real)intValue;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_HI_AMP:
    {
        intValue = Utils::clamp(intValue, 1, 199);
        mPropHiAmp = (Ogre::Real)intValue * 0.01f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_MID_AMP:
    {
        intValue = Utils::clamp(intValue, 1, 199);
        mPropMidAmp = (Ogre::Real)intValue * 0.01f;
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

HeightfieldOperationCPUNoise::HeightfieldOperationCPUNoise() : HeightfieldOperationCPU()
{
    //	mBrushName = "HeightfieldOperationCPUNoise";
    mBrushName = _T("HeightfieldBrush");

    loadPersistentProperties();
}

void HeightfieldOperationCPUNoise::applyPrimary(const Ogre::Vector3& position, Ogre::Real strength)
{
    assert(mBrush);
    HeightfieldBuffer* heightfieldBuffer = mBrush->getHeightfieldGeom()->getHeightfieldBuffer();

    Ogre::Real changeSpeed = 0.02f * mStrength;
    Ogre::Real sizeChangeScale =
        strength * changeSpeed; // * (1.0f - expf(-getEngineCore()->getTimeSinceLastFrame() / changeSpeed));
    Ogre::Real innerRadius = mBrush->getInnerRadius();
    Ogre::Real outerRadius = mBrush->getOuterRadius();
    int size = (int)(2 * outerRadius) + 2;
    int left = (int)position.x;
    int top = (int)position.z;
    Ogre::Rect editRect(
        std::max(0, left - size / 2), std::max(0, top - size / 2),
        std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCount(), left + size / 2),
        std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCount(), top + size / 2));
    Ogre::Real* data = new Ogre::Real[std::max((long)1, editRect.width() * editRect.height())];

    Ogre::PixelBox pixelBox(Utils::rectToBox(editRect), Ogre::PF_FLOAT32_R, data);
    heightfieldBuffer->copyTo(pixelBox);
    Ogre::Real scale = 1.0f / (innerRadius * innerRadius - outerRadius * outerRadius);
    Ogre::Real bias = -outerRadius * outerRadius * scale;

    // Ogre::Real heightscale = heightfieldBuffer->getHeightRangeMax() -
    // heightfieldBuffer->getHeightRangeMin();
    Ogre::Real cx = position.x - (Ogre::Real)editRect.left;
    // Ogre::Real cy = position.y;
    Ogre::Real cz = position.z - (Ogre::Real)editRect.top;

    int offsetx = editRect.left - (int)mNoiseOrigin.x;
    int offsetz = editRect.top - (int)mNoiseOrigin.z;

    Ogre::Real smallestLevelReal = log(std::max((Ogre::Real)1, mPropMinRange)) / log(2.0f);
    Ogre::Real largestLevelReal = std::max(
        smallestLevelReal + 1E-6f, std::log(std::max((Ogre::Real)1, mPropMaxRange)) / std::log(2.0f));
    int largestLevel = (int)ceil(largestLevelReal);
    int smallestLevel = (int)floor(smallestLevelReal);

    Ogre::Real high =
        std::min(1.0f - 1E-6f, std::pow(std::max(1E-6f, 0.5f * mPropHiAmp), largestLevelReal));
    Ogre::Real mid =
        std::min(1.0f - 1E-6f, std::pow(std::max(1E-6f, 0.5f * mPropMidAmp), 0.5f * largestLevelReal));
    Ogre::Real discriminant =
        std::max(0.0f, 0.25f * high * high + 0.5f * high + 0.25f - high * mid + mid * mid - mid);
    Ogre::Real half = std::max(1E-6f, (0.5f - 0.5f * high - sqrtf(discriminant)) / (1 - mid));
    Ogre::Real levelScale = pow(half, 2.0f / (largestLevelReal - smallestLevelReal));
    Ogre::Real levelBias = (1 - levelScale) * (mid - half) / (1.0f - half);

    // std::cout << "####" << levelScale << "  " << levelBias << "\n";

    Ogre::Real* amplitudes = new Ogre::Real[largestLevel + 1];
    Ogre::Real amp = 1.0f;
    for (int level = largestLevel; level >= smallestLevel; level--)
    {
        amplitudes[level] = amp;
        amp = amp * levelScale + levelBias;
    }
    amplitudes[largestLevel] *= std::max(0.0f, 1.0f - (largestLevel - largestLevelReal));
    amplitudes[smallestLevel] *= std::max(0.0f, 1.0f - (smallestLevelReal - smallestLevel));

    for (int x = 0; x < (int)editRect.width(); ++x)
    {
        for (int z = 0; z < (int)editRect.height(); ++z)
        {
            Ogre::Real hy = data[z * editRect.width() + x];
            Ogre::Real dx = (Ogre::Real)x - cx;
            // Ogre::Real dy = hy * heightscale - cy;
            Ogre::Real dz = (Ogre::Real)z - cz;

            Ogre::Real s = dx * dx + dz * dz;
            Ogre::Real delta = 0.0f;

            // float power = 0.35f;
            for (int level = largestLevel; level >= smallestLevel; level--)
            {
                int spacing = (1 << level);
                int invmask = spacing - 1;
                int mask = ~invmask;
                int px = (offsetx + x) & mask;
                int pz = (offsetz + z) & mask;

                int delta1 = -spacing, delta2 = 0, delta3 = spacing, delta4 = spacing + spacing;

                int rp1x = Utils::randHash(px + delta1);
                int rp2x = Utils::randHash(px + delta2);
                int rp3x = Utils::randHash(px + delta3);
                int rp4x = Utils::randHash(px + delta4);

                int rp1z = Utils::randHash(13 * (pz + delta1));
                int rp2z = Utils::randHash(13 * (pz + delta2));
                int rp3z = Utils::randHash(13 * (pz + delta3));
                int rp4z = Utils::randHash(13 * (pz + delta4));

                int h11 = Utils::randHash(rp1x + rp1z);
                int h12 = Utils::randHash(rp1x + rp2z);
                int h13 = Utils::randHash(rp1x + rp3z);
                int h14 = Utils::randHash(rp1x + rp4z);

                int h21 = Utils::randHash(rp2x + rp1z);
                int h22 = Utils::randHash(rp2x + rp2z);
                int h23 = Utils::randHash(rp2x + rp3z);
                int h24 = Utils::randHash(rp2x + rp4z);

                int h31 = Utils::randHash(rp3x + rp1z);
                int h32 = Utils::randHash(rp3x + rp2z);
                int h33 = Utils::randHash(rp3x + rp3z);
                int h34 = Utils::randHash(rp3x + rp4z);

                int h41 = Utils::randHash(rp4x + rp1z);
                int h42 = Utils::randHash(rp4x + rp2z);
                int h43 = Utils::randHash(rp4x + rp3z);
                int h44 = Utils::randHash(rp4x + rp4z);

                Ogre::Real wx = Ogre::Real((offsetx + x) & invmask) / spacing;
                Ogre::Real wz = Ogre::Real((offsetz + z) & invmask) / spacing;

                Ogre::Vector4 crx = Utils::getCatmullRomeSplineWeights(wx);
                Ogre::Vector4 crz = Utils::getCatmullRomeSplineWeights(wz);

                Ogre::Real h1 = crx.x * h11 + crx.y * h21 + crx.z * h31 + crx.w * h41;
                Ogre::Real h2 = crx.x * h12 + crx.y * h22 + crx.z * h32 + crx.w * h42;
                Ogre::Real h3 = crx.x * h13 + crx.y * h23 + crx.z * h33 + crx.w * h43;
                Ogre::Real h4 = crx.x * h14 + crx.y * h24 + crx.z * h34 + crx.w * h44;

                Ogre::Real h = h1 * crz.x + h2 * crz.y + h3 * crz.z + h4 * crz.w;

                // int hz1 = ((h1 << 12) + (h2 - h1) * wx) >> 12;
                // int hz2 = ((h3 << 12) + (h4 - h3) * wx) >> 12;
                // int h = ((hz1 << 12) + (hz2 - hz1) * wz) >> 12;

                delta += amplitudes[level] * (h / 32768.0f - 0.5f);
                // delta += power * (h / 32768.0f - 0.5f);
                // power = power * 0.45f + 0.00005f;
            }

            delta *= sizeChangeScale * Utils::clamp(s * scale + bias, 0.0f, 1.0f);
            data[z * editRect.width() + x] = Utils::clamp(hy + delta, 0.0f, 1.0f);
        }
    }
    delete amplitudes;
    heightfieldBuffer->updateFrom(pixelBox);
    heightfieldBuffer->revalidate();
    delete data;
}

/*
void HeightfieldOperationCPUNoise::applySecondary(const Ogre::Vector3& position, Ogre::Real strength, const
Ogre::Vector3& delta)
{
        mNoiseOrigin += delta;
        printf("%f %f %f\n", mNoiseOrigin.x, mNoiseOrigin.y, mNoiseOrigin.z);
}
*/

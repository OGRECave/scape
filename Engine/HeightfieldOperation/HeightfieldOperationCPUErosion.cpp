/**
  * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
  */

#include "ScapeEngineStableHeaders.h"
#include "EngineCore/SettingsDatasetManager.h"
#include "HeightfieldOperationCPUErosion.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"

#include "HeightfieldSerializer/HeightfieldSerializerIncremental.h"

using namespace ScapeEngine;

enum EPropertyId
{
    PROPERTYID_STRENGTH,
    PROPERTYID_PENCILMODE,
    PROPERTYID_PATHSPACING,
    PROPERTYID_KR,
    PROPERTYID_KD,
    PROPERTYID_KC,
    PROPERTYID_KS,
    PROPERTYID_KA,
};

bool HeightfieldOperationCPUErosion::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString));
    return true;
}

bool HeightfieldOperationCPUErosion::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    ADD_STRINGENUM(map, PROPERTYID_, STRENGTH);
    ADD_STRINGENUM(map, PROPERTYID_, PENCILMODE);
    ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);
    ADD_STRINGENUM(map, PROPERTYID_, KR);
    ADD_STRINGENUM(map, PROPERTYID_, KD);
    ADD_STRINGENUM(map, PROPERTYID_, KC);
    ADD_STRINGENUM(map, PROPERTYID_, KS);
    ADD_STRINGENUM(map, PROPERTYID_, KA);
    ADD_STRINGENUM(map, PROPERTYID_, PATHSPACING);

    return true;
}

string HeightfieldOperationCPUErosion::setUIElementPropertyValue(const string& elementName,
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
    case PROPERTYID_KR:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mKR = intValue * 0.0000005f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_KD:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mKD = intValue * 0.0005f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_KC:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mKC = intValue * 0.1f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_KS:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mKS = intValue * 0.00025f;
        outValue = Ogre::StringConverter::toString(intValue);
        break;
    }
    case PROPERTYID_KA:
    {
        intValue = Utils::clamp(intValue, 0, 1000);
        mKA = intValue * 0.000001f;
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

HeightfieldOperationCPUErosion::HeightfieldOperationCPUErosion() : HeightfieldOperationCPU()
{
    mBrushName = _T("HeightfieldOperationCPUErosion");

    loadPersistentProperties();
}

void HeightfieldOperationCPUErosion::applyPrimary(const Ogre::Vector3& position, Ogre::Real strength)
{
    assert(mBrush);

    HeightfieldBuffer* heightfieldBuffer = mBrush->getHeightfieldGeom()->getHeightfieldBuffer();
    assert(heightfieldBuffer);

    Ogre::Real innerRadius = mHeightfieldBrushSettings.getInnerRadius();
    Ogre::Real outerRadius = mHeightfieldBrushSettings.getOuterRadius();
    int size = (int)(2 * outerRadius) + 2;
    int left = (int)position.x;
    int top = (int)position.z;
    Ogre::Rect editRect(
        std::max(0, left - size / 2), std::max(0, top - size / 2),
        std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCount(), left + size / 2),
        std::min(heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCount(), top + size / 2));
    int elementCount = std::max<int>(1, editRect.width() * editRect.height());
    Ogre::Real* dataIn = new Ogre::Real[3 * elementCount];
    Ogre::Real* dataOut = new Ogre::Real[3 * elementCount];

    Ogre::PixelBox pixelBox(Utils::rectToBox(editRect), Ogre::PF_FLOAT32_RGB, dataIn);
    heightfieldBuffer->copyTo(pixelBox);

    // Ogre::Real scale = 1.0f / (innerRadius*innerRadius-outerRadius*outerRadius);
    // Ogre::Real bias = -outerRadius * outerRadius * scale;

    // Ogre::Real heightscale = heightfieldBuffer->getHeightRangeMax() -
    // heightfieldBuffer->getHeightRangeMin();
    Ogre::Real cx = position.x - (Ogre::Real)editRect.left;
    // Ogre::Real cy = position.y;
    Ogre::Real cz = position.z - (Ogre::Real)editRect.top;

    int dataRowPitch = (int)editRect.width() * 3;

    int index = 0;
    for (int z = 0; z < (int)editRect.height(); ++z)
    {
        for (int x = 0; x < (int)editRect.width(); ++x, index += 3)
        {
            // Ogre::Real dx = (Ogre::Real)x - cx;
            // Ogre::Real dz = (Ogre::Real)z - cz;
            // Ogre::Real s = dx * dx + dz * dz;

            // float water = strength * Utils::max(0.0f, 1.0f - ((x - cx) * (x - cx) + (z - cz) * (z - cz))
            // / innerRadius / innerRadius);//data[index + 1] * mCapacity;

            // dataIn[index] += water * mCapacity;
            dataIn[index + 1] = 0; // water * mKR;
            dataIn[index + 2] = 0;
        }
    }

    Ogre::PixelUtil::bulkPixelConversion(dataIn, pixelBox.format, dataOut, pixelBox.format, elementCount);

    for (int it = 0; it < outerRadius - innerRadius - 1; ++it)
    {
        int index = 0;
        for (int z = 0; z < (int)editRect.height(); ++z)
        {
            for (int x = 0; x < (int)editRect.width(); ++x, index += 3)
            {
                // Ogre::Real dx = (Ogre::Real)x - cx;
                // Ogre::Real dz = (Ogre::Real)z - cz;
                // Ogre::Real s = dx * dx + dz * dz;
                float water =
                    Utils::max(0.0f, 1.0f -
                                         ((x - cx) * (x - cx) + (z - cz) * (z - cz)) / innerRadius /
                                             innerRadius); // data[index + 1] * mCapacity;
                // dataIn[index] += water * mCapacity;
                dataIn[index + 1] += water * mKR;
                // dataIn[index+2] = 0;
            }
        }

        index = ((int)editRect.width() + 1) * 3;
        for (int z = 1; z < (int)editRect.height() - 1; ++z)
        {
            for (int x = 1; x < (int)editRect.width() - 1; ++x, index += 3)
            {
                // Ogre::Real mKA2 = 0.005f;
                // Ogre::Real kT = 0.25f;

                Ogre::Real water = dataIn[index + 1];
                if (water > 0.0f)
                {
                    Ogre::Real height = dataIn[index];
                    Ogre::Real sediment = dataIn[index + 2];

                    Ogre::Real dheight1 = dataIn[index - 3];
                    Ogre::Real dheight2 = dataIn[index + 3];
                    Ogre::Real dheight3 = dataIn[index - dataRowPitch];
                    Ogre::Real dheight4 = dataIn[index + dataRowPitch];
                    Ogre::Real dheightavg = (dheight1 + dheight2 + dheight3 + dheight4) * 0.25f;

                    // Ogre::Real steepscale = std::min(1.0f, 2000.0f * abs(dheight1 - dheight2) +
                    // abs(dheight3 - dheight4));

                    Ogre::Real blur = (dheightavg - height) * mKA *
                                      Utils::max(0.0f, 1.0f -
                                                           ((x - cx) * (x - cx) + (z - cz) * (z - cz)) /
                                                               innerRadius / innerRadius);

                    dataOut[index] += 4.0f * blur;
                    dataOut[index - 3] -= blur;
                    dataOut[index + 3] -= blur;
                    dataOut[index - dataRowPitch] -= blur;
                    dataOut[index + dataRowPitch] -= blur;

                    /*
                    Ogre::Real waterBlur = 0.10f * dataIn[index+1];
                    dataOut[index+1] -= 4.0f * waterBlur;
                    dataOut[index-2] += waterBlur;
                    dataOut[index+4] += waterBlur;
                    dataOut[index-dataRowPitch+1] += waterBlur;
                    dataOut[index+dataRowPitch+1] += waterBlur;


                    Ogre::Real sedBlur = 0.10f * dataIn[index+2];
                    dataOut[index+2] -= 4.0f * sedBlur;
                    dataOut[index-1] += sedBlur;
                    dataOut[index+5] += sedBlur;
                    dataOut[index-dataRowPitch+2] += sedBlur;
                    dataOut[index+dataRowPitch+2] += sedBlur;
                    */

                    Ogre::Real delta1 =
                        0.257f * std::max(0.0f, height + water - dataIn[index - 3] - dataIn[index - 2]);
                    Ogre::Real delta2 =
                        0.257f * std::max(0.0f, height + water - dataIn[index + 3] - dataIn[index + 4]);
                    Ogre::Real delta3 =
                        0.257f * std::max(0.0f, height + water - dataIn[index - dataRowPitch] -
                                                    dataIn[index - dataRowPitch + 1]);
                    Ogre::Real delta4 =
                        0.257f * std::max(0.0f, height + water - dataIn[index + dataRowPitch] -
                                                    dataIn[index + dataRowPitch + 1]);

                    Ogre::Real delta5 =
                        0.103f * std::max(0.0f, height + water - dataIn[index - dataRowPitch - 3] -
                                                    dataIn[index - dataRowPitch - 2]);
                    Ogre::Real delta6 =
                        0.103f * std::max(0.0f, height + water - dataIn[index - dataRowPitch + 3] -
                                                    dataIn[index - dataRowPitch + 4]);
                    Ogre::Real delta7 =
                        0.103f * std::max(0.0f, height + water - dataIn[index + dataRowPitch - 3] -
                                                    dataIn[index + dataRowPitch - 2]);
                    Ogre::Real delta8 =
                        0.103f * std::max(0.0f, height + water - dataIn[index + dataRowPitch + 3] -
                                                    dataIn[index + dataRowPitch + 4]);

                    Ogre::Real deltaSum =
                        delta1 + delta2 + delta3 + delta4 + delta5 + delta6 + delta7 + delta8;
                    Ogre::Real weight1 = (delta1 / deltaSum);
                    Ogre::Real weight2 = (delta2 / deltaSum);
                    Ogre::Real weight3 = (delta3 / deltaSum);
                    Ogre::Real weight4 = (delta4 / deltaSum);
                    Ogre::Real weight5 = (delta5 / deltaSum);
                    Ogre::Real weight6 = (delta6 / deltaSum);
                    Ogre::Real weight7 = (delta7 / deltaSum);
                    Ogre::Real weight8 = (delta8 / deltaSum);

                    /*
                    // thermal erosion
                    if (deltaSum > mKA)
                    {
                            Ogre::Real deltaHeight = (deltaSum - mKA2) * kT;
                            dataOut[index] -= deltaHeight;
                            dataOut[index-3] += deltaHeight * weight1;
                            dataOut[index+3] += deltaHeight * weight2;
                            dataOut[index-dataRowPitch] += deltaHeight * weight3;
                            dataOut[index+dataRowPitch] += deltaHeight * weight4;
                            dataOut[index-dataRowPitch-3] += deltaHeight * weight5;
                            dataOut[index-dataRowPitch+3] += deltaHeight * weight6;
                            dataOut[index+dataRowPitch-3] += deltaHeight * weight7;
                            dataOut[index+dataRowPitch+3] += deltaHeight * weight8;
                    }
                    */

                    // fluvial erosion
                    Ogre::Real deltaWater = std::min(water, deltaSum); // * steepscale;

                    if (deltaWater <= 1E-20f)
                    {
                        dataOut[index] += mKD * sediment;
                        dataOut[index + 2] -= mKD * sediment;
                    }
                    else
                    {
                        dataOut[index + 1] -= deltaWater;

                        dataOut[index - 2] += deltaWater * weight1;
                        dataOut[index + 4] += deltaWater * weight2;
                        dataOut[index - dataRowPitch + 1] += deltaWater * weight3;
                        dataOut[index + dataRowPitch + 1] += deltaWater * weight4;
                        dataOut[index - dataRowPitch - 2] += deltaWater * weight5;
                        dataOut[index - dataRowPitch + 4] += deltaWater * weight6;
                        dataOut[index + dataRowPitch - 2] += deltaWater * weight7;
                        dataOut[index + dataRowPitch + 4] += deltaWater * weight8;

                        Ogre::Real capacity = mKC * deltaWater;

                        if (sediment > capacity)
                        {
                            dataOut[index] += mKD * (sediment - capacity);
                            dataOut[index + 2] += -capacity + mKD * (sediment - capacity);

                            dataOut[index - 1] += capacity * weight1;
                            dataOut[index + 5] += capacity * weight2;
                            dataOut[index - dataRowPitch + 2] += capacity * weight3;
                            dataOut[index + dataRowPitch + 2] += capacity * weight4;
                            dataOut[index - dataRowPitch - 1] += capacity * weight5;
                            dataOut[index - dataRowPitch + 5] += capacity * weight6;
                            dataOut[index + dataRowPitch - 1] += capacity * weight7;
                            dataOut[index + dataRowPitch + 5] += capacity * weight8;
                        }
                        else
                        {
                            Ogre::Real deltaSediment = mKS * (capacity - sediment);
                            dataOut[index] -= deltaSediment;
                            dataOut[index + 2] -= sediment; // 0.0f;

                            dataOut[index - 1] += (sediment + deltaSediment) * weight1;
                            dataOut[index + 5] += (sediment + deltaSediment) * weight2;
                            dataOut[index - dataRowPitch + 2] += (sediment + deltaSediment) * weight3;
                            dataOut[index + dataRowPitch + 2] += (sediment + deltaSediment) * weight4;
                            dataOut[index - dataRowPitch - 1] += (sediment + deltaSediment) * weight5;
                            dataOut[index - dataRowPitch + 5] += (sediment + deltaSediment) * weight6;
                            dataOut[index + dataRowPitch - 1] += (sediment + deltaSediment) * weight7;
                            dataOut[index + dataRowPitch + 5] += (sediment + deltaSediment) * weight8;
                        }
                    }

                    // if (deltaSum > 0.0f)
                    //{
                    // Ogre::Real dx = (Ogre::Real)x - cx;
                    // Ogre::Real dz = (Ogre::Real)z - cz;
                    // Ogre::Real s = dx * dx + dz * dz;
                    // Ogre::Real deltaWater = 0.25f * Utils::min(1.0f, water / deltaSum) * Utils::clamp(s *
                    // scale + bias, 0.0f, 1.0f);
                    //}
                }
            }
            index += 6;
        }

        index = ((int)editRect.width() + 1) * 3;
        for (int z = 1; z < (int)editRect.height() - 1; ++z)
        {
            for (int x = 1; x < (int)editRect.width() - 1; ++x, index += 3)
            {
                Ogre::Real waterBlur = 0.01f * dataOut[index + 1];
                dataOut[index + 1] -= 4.0f * waterBlur;
                dataOut[index - 2] += waterBlur;
                dataOut[index + 4] += waterBlur;
                dataOut[index - dataRowPitch + 1] += waterBlur;
                dataOut[index + dataRowPitch + 1] += waterBlur;
            }
        }

        index = 0;
        for (int z = 0; z < (int)editRect.height(); ++z)
        {
            for (int x = 0; x < (int)editRect.width(); ++x, index += 3)
            {
                dataIn[index] = Utils::clamp(dataOut[index], 0.0f, 1.0f);
                dataIn[index + 1] = dataOut[index + 1];
                dataIn[index + 2] = dataOut[index + 2];
            }
        }
    }

    index = 0;
    for (int z = 0; z < (int)editRect.height(); ++z)
    {
        for (int x = 0; x < (int)editRect.width(); ++x, index += 3)
        {
            dataIn[index] = Utils::clamp(dataIn[index] + dataIn[index + 2], 0.0f,
                                         1.0f); // - dataIn[index+1];// - dataIn[index+1];
        }
    }

    heightfieldBuffer->updateFrom(pixelBox);
    heightfieldBuffer->revalidate();
    delete dataIn;
    delete dataOut;
}

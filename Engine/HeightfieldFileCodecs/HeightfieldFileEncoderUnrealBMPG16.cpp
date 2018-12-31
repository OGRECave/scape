/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldFileEncoderUnrealBMPG16.h"
#include "HeightfieldFileEncoderRaw.h"
#include "Utils/SerialMemoryBuffer.h"
#include "Utils/BMPHeader.h"

#define UIELEMENTPROPERTY_FLIPX _T("FLIPX")
#define UIELEMENTPROPERTY_FLIPY _T("FLIPY")

enum EPropertyId
{
    PROPERTYID_FLIPX,
    PROPERTYID_FLIPY,
};

using namespace ScapeEngine;

IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldFileEncoderUnrealBMPG16);

// ----------------------------------------------------------------------------
HeightfieldFileEncoderUnrealBMPG16::HeightfieldFileEncoderUnrealBMPG16()
    : mPixelFormat(Ogre::PF_SHORT_L), mBigEndian(false)
{
    LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();
}

// ----------------------------------------------------------------------------
const string& HeightfieldFileEncoderUnrealBMPG16::getFileExtension()
{
    static string fileExtension(_T("BMP"));
    return fileExtension;
}

// ----------------------------------------------------------------------------
const string& HeightfieldFileEncoderUnrealBMPG16::getFileFilter()
{
    static string fileFilter(_T("*.BMP;*.T3D"));
    return fileFilter;
}

// ----------------------------------------------------------------------------
bool HeightfieldFileEncoderUnrealBMPG16::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValue(UIELEMENTPROPERTY_FLIPX, _T("0"));
    setUIElementPropertyValue(UIELEMENTPROPERTY_FLIPY, _T("0"));
    return true;
}

// ----------------------------------------------------------------------------
bool HeightfieldFileEncoderUnrealBMPG16::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    map[UIELEMENTPROPERTY_FLIPX] = PROPERTYID_FLIPX;
    map[UIELEMENTPROPERTY_FLIPY] = PROPERTYID_FLIPY;
    return true;
}

// ----------------------------------------------------------------------------
string HeightfieldFileEncoderUnrealBMPG16::setUIElementPropertyValue(const string& elementName,
                                                                     const string& value)
{
    bool someValue = Ogre::StringConverter::parseBool(value);
    string outValue = Ogre::StringConverter::toString(someValue);

    if (elementName.compare(UIELEMENTPROPERTY_FLIPX) == 0)
        mFlipX = someValue;
    else if (elementName.compare(UIELEMENTPROPERTY_FLIPY) == 0)
        mFlipY = someValue;

    getPersistentElementValueMap()[elementName] = outValue;
    return outValue;
}

// ----------------------------------------------------------------------------
bool HeightfieldFileEncoderUnrealBMPG16::encode(HeightfieldBuffer* inBuffer, const string& fileName,
                                                string* error)
{
    Ogre::LogManager::getSingleton().logMessage(_T("Exporting heightfield '") + inBuffer->getName() +
                                                _T("' to file '") + fileName + _T("' using encoder '") +
                                                getClassName() + _T("'"));
    assert(inBuffer);

    FILE* file = fopen(fileName.c_str(), _T("wb"));
    if (file)
    {
        Ogre::PixelBox rawPixelBox(inBuffer->getHeightfieldBufferSet()->getElementColumnCount(),
                                   inBuffer->getHeightfieldBufferSet()->getElementRowCount(), 1,
                                   mPixelFormat);
        rawPixelBox.data = new Ogre::uint8[rawPixelBox.getConsecutiveSize()];
        inBuffer->copyTo(rawPixelBox);

        size_t pixelByteCount = Ogre::PixelUtil::getNumElemBytes(mPixelFormat);
        bool flipEndian = ((OGRE_ENDIAN == OGRE_ENDIAN_BIG) != mBigEndian) && pixelByteCount > 1;

        Utils::BitmapFileHeader bmpFH;
        Utils::BitmapInfoHeader bmpIH;

        memset(&bmpFH, 0, sizeof(Utils::BitmapFileHeader));
        bmpFH.type = 'B' + (256 * 'M');
        bmpFH.offBits = sizeof(Utils::BitmapFileHeader) + sizeof(Utils::BitmapInfoHeader);
        bmpFH.size = bmpFH.offBits + rawPixelBox.getConsecutiveSize();

        memset(&bmpIH, 0, sizeof(Utils::BitmapInfoHeader));
        bmpIH.size = sizeof(Utils::BitmapInfoHeader);
        bmpIH.width = inBuffer->getHeightfieldBufferSet()->getElementColumnCount();
        bmpIH.height = inBuffer->getHeightfieldBufferSet()->getElementRowCount();
        bmpIH.planes = 1;
        bmpIH.bitCount = 16;
        bmpIH.compression = Utils::BITMAPCOMPRESSION_RGB;
        bmpIH.sizeImage = rawPixelBox.getConsecutiveSize();

        if (flipEndian)
        {
            bmpFH.flipByteOrder();
            bmpIH.flipByteOrder();
        }

        fwrite(&bmpFH, sizeof(Utils::BitmapFileHeader), 1, file);
        fwrite(&bmpIH, sizeof(Utils::BitmapInfoHeader), 1, file);

        bool successful = HeightfieldFileEncoderRawBase::encode(inBuffer, file, mFlipX, mFlipY, mBigEndian,
                                                                mPixelFormat, error);

        fclose(file);

        return successful;
    }
    return false;
}

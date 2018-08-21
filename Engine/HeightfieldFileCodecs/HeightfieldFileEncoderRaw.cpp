/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "HeightfieldFileEncoderRaw.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "PCH/stdafx.h"
#include "Utils/SerialMemoryBuffer.h"

#define UIELEMENTPROPERTY_FLIPX _T("FLIPX")
#define UIELEMENTPROPERTY_FLIPY _T("FLIPY")

using namespace ScapeEngine;

enum EPropertyId
{
    PROPERTYID_FLIPX,
    PROPERTYID_FLIPY,
};

// ----------------------------------------------------------------------------
bool HeightfieldFileEncoderRawBase::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValue(UIELEMENTPROPERTY_FLIPX, _T("0"));
    setUIElementPropertyValue(UIELEMENTPROPERTY_FLIPY, _T("0"));
    return true;
}

// ----------------------------------------------------------------------------
bool HeightfieldFileEncoderRawBase::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    map[UIELEMENTPROPERTY_FLIPX] = PROPERTYID_FLIPX;
    map[UIELEMENTPROPERTY_FLIPY] = PROPERTYID_FLIPY;
    return true;
}

// ----------------------------------------------------------------------------
string HeightfieldFileEncoderRawBase::setUIElementPropertyValue(const string& elementName,
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
bool HeightfieldFileEncoderRawBase::encode(HeightfieldBuffer* inBuffer, const string& fileName,
                                           string* error)
{
    Ogre::LogManager::getSingleton().logMessage(_T("Exporting heightfield '") + inBuffer->getName() +
                                                _T("' to file '") + fileName + _T("' using encoder '") +
                                                getClassName() + _T("'"));
    assert(inBuffer);

    FILE* file = fopen(fileName.c_str(), _T("wb"));
    if (file)
    {
        bool successful = encode(inBuffer, file, mFlipX, mFlipY, mBigEndian, mPixelFormat, error);

        fclose(file);

        return successful;
    }
    return false;
}

// ----------------------------------------------------------------------------
bool HeightfieldFileEncoderRawBase::encode(HeightfieldBuffer* inBuffer, FILE* fileHandle, bool flipX,
                                           bool flipY, bool bigEndian, Ogre::PixelFormat pixelFormat,
                                           string* error)
{
    if (fileHandle)
    {
        Ogre::PixelBox rawPixelBox(inBuffer->getHeightfieldBufferSet()->getElementColumnCount(),
                                   inBuffer->getHeightfieldBufferSet()->getElementRowCount(), 1,
                                   pixelFormat);
        rawPixelBox.data = new Ogre::uint8[rawPixelBox.getConsecutiveSize()];
        inBuffer->copyTo(rawPixelBox);

        size_t lines = inBuffer->getHeightfieldBufferSet()->getElementRowCount();
        size_t pixelByteCount = Ogre::PixelUtil::getNumElemBytes(pixelFormat);
        size_t rowBytePitch = (int)rawPixelBox.rowPitch * pixelByteCount;
        bool flipEndian = ((OGRE_ENDIAN == OGRE_ENDIAN_BIG) != bigEndian) && pixelByteCount > 1;

        for (size_t line = 0; line < lines; ++line)
        {
            Ogre::uint8* rowData =
                (Ogre::uint8*)rawPixelBox.data + (flipY ? lines - 1 - line : line) * rowBytePitch;

            if (flipX)
            {
                if (flipEndian)
                {
                    Utils::flipByteOrder(rowData, rowBytePitch);
                }
                else
                {
                    Utils::flipSuperWordOrder(rowData, rawPixelBox.rowPitch, pixelByteCount);
                }
            }
            else if (flipEndian)
            {
                Utils::flipSubWordOrder(rowData, rawPixelBox.rowPitch, pixelByteCount);
            }

            fwrite(rowData, 1, rowBytePitch, fileHandle);
        }

        delete rawPixelBox.data;

        return true;
    }
    else if (error)
    {
        *error = _T("Invalid filehandle");
    }
    return false;
}

// ----------------------------------------------------------------------------
#define IMPLEMENT_HEIGHTFIELDFILEENCODERRAW_CLASS(className, extension, filter, format, bigendian)         \
    className::className() : HeightfieldFileEncoderRawBase(format, bigendian)                              \
    {                                                                                                      \
        LOADPROPERTIES_UIELEMENTCONTAINERSIMPLE_CLASS();                                                   \
    }                                                                                                      \
    IMPLEMENT_UIELEMENTCONTAINERSIMPLE_CLASS(className)                                                    \
    const string& className::getFileExtension()                                                            \
    {                                                                                                      \
        static string fileExtension(extension);                                                            \
        return fileExtension;                                                                              \
    }                                                                                                      \
    const string& className::getFileFilter()                                                               \
    {                                                                                                      \
        static string fileFilter(filter);                                                                  \
        return fileFilter;                                                                                 \
    }

IMPLEMENT_HEIGHTFIELDFILEENCODERRAW_CLASS(HeightfieldFileEncoderRaw8, _T("RAW"), _T("*.RAW;*.R8"),
                                          Ogre::PF_L8, false);
IMPLEMENT_HEIGHTFIELDFILEENCODERRAW_CLASS(HeightfieldFileEncoderRaw16L, _T("RAW"), _T("*.RAW;*.R16"),
                                          Ogre::PF_SHORT_L, false);
IMPLEMENT_HEIGHTFIELDFILEENCODERRAW_CLASS(HeightfieldFileEncoderRaw16B, _T("RAW"), _T("*.RAW;*.R16"),
                                          Ogre::PF_SHORT_L, true);
IMPLEMENT_HEIGHTFIELDFILEENCODERRAW_CLASS(HeightfieldFileEncoderRaw32F, _T("RAW"), _T("*.RAW;*.R32"),
                                          Ogre::PF_FLOAT32_R, false);

/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "HeightfieldFileEncoderImage.h"
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
bool HeightfieldFileEncoderImageBase::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValue(UIELEMENTPROPERTY_FLIPX, _T("0"));
    setUIElementPropertyValue(UIELEMENTPROPERTY_FLIPY, _T("0"));
    return true;
}

// ----------------------------------------------------------------------------
bool HeightfieldFileEncoderImageBase::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    map[UIELEMENTPROPERTY_FLIPX] = PROPERTYID_FLIPX;
    map[UIELEMENTPROPERTY_FLIPY] = PROPERTYID_FLIPY;
    return true;
}

// ----------------------------------------------------------------------------
string HeightfieldFileEncoderImageBase::setUIElementPropertyValue(const string& elementName,
                                                                  const string& value)
{
    bool someValue = Ogre::StringConverter::parseBool(value);
    string outValue = Ogre::StringConverter::toString(someValue);

    if (elementName.compare(_T("FLIPX")) == 0)
        mFlipX = someValue;
    else if (elementName.compare(_T("FLIPY")) == 0)
        mFlipY = someValue;

    getPersistentElementValueMap()[elementName] = outValue;
    return outValue;
}

// ----------------------------------------------------------------------------
bool HeightfieldFileEncoderImageBase::encode(HeightfieldBuffer* inBuffer, const string& fileName,
                                             string* error)
{
    Ogre::LogManager::getSingleton().logMessage(_T("Exporting heightfield '") + inBuffer->getName() +
                                                _T("' to file '") + fileName + _T("' using encoder '") +
                                                getClassName() + _T("'"));
    assert(inBuffer);

    Ogre::PixelBox rawPixelBox(inBuffer->getHeightfieldBufferSet()->getElementColumnCount(),
                               inBuffer->getHeightfieldBufferSet()->getElementRowCount(), 1, mPixelFormat);
    rawPixelBox.data = new Ogre::uint8[rawPixelBox.getConsecutiveSize()];
    inBuffer->copyTo(rawPixelBox);

    size_t lines = inBuffer->getHeightfieldBufferSet()->getElementRowCount();
    size_t pixelByteCount = Ogre::PixelUtil::getNumElemBytes(mPixelFormat);
    size_t rowBytePitch = (int)rawPixelBox.rowPitch * pixelByteCount;

    if (mFlipY)
    {
        for (size_t line = 0; line < lines / 2; ++line)
        {
            Ogre::uint8* rowData = (Ogre::uint8*)rawPixelBox.data + line * rowBytePitch;
            Ogre::uint8* altData = (Ogre::uint8*)rawPixelBox.data + (lines - 1 - line) * rowBytePitch;
            for (Ogre::uint8 *data = rowData; data < rowData + rowBytePitch; ++data, ++altData)
            {
                unsigned char tmp = *data;
                *data = *altData;
                *altData = tmp;
            }
        }
    }

    if (mFlipX)
    {
        for (size_t line = 0; line < lines; ++line)
        {
            Ogre::uint8* rowData = (Ogre::uint8*)rawPixelBox.data + line * rowBytePitch;
            Utils::flipSuperWordOrder(rowData, rawPixelBox.rowPitch, pixelByteCount);
        }
    }

    bool saved = false;
    try
    {
        Ogre::Image image;
        image.loadDynamicImage((Ogre::uchar*)rawPixelBox.data,
                               inBuffer->getHeightfieldBufferSet()->getElementColumnCount(),
                               inBuffer->getHeightfieldBufferSet()->getElementRowCount(), 1, mPixelFormat);
        image.save(fileName);
        saved = true;
    }
    catch (...)
    {
    }

    delete rawPixelBox.data;

    if (!saved)
    {
        if (error)
        {
            *error = _T("File could not be saved");
        }
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------------
#define IMPLEMENT_HEIGHTFIELDFILEENCODERIMAGE_CLASS(className, extension, filter, format)                  \
    className::className() : HeightfieldFileEncoderImageBase(extension, format)                            \
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

IMPLEMENT_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageBMP8U, _T("BMP"), _T("*.BMP"),
                                            Ogre::PF_L8);
IMPLEMENT_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImagePGM8U, _T("PGM"), _T("*.PGM"),
                                            Ogre::PF_L8);
IMPLEMENT_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImagePNG8U, _T("PNG"), _T("*.PNG"),
                                            Ogre::PF_L8);
IMPLEMENT_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageTGA8U, _T("TGA"), _T("*.TGA"),
                                            Ogre::PF_L8);
IMPLEMENT_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageTIFF8U, _T("TIFF"),
                                            _T("*.TIF;*.TIFF"), Ogre::PF_L8);

IMPLEMENT_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImagePGM16U, _T("PGM"), _T("*.PGM"),
                                            Ogre::PF_SHORT_L);
IMPLEMENT_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImagePNG16U, _T("PNG"), _T("*.PNG"),
                                            Ogre::PF_SHORT_L);
IMPLEMENT_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageTIFF16U, _T("TIFF"),
                                            _T("*.TIF;*.TIFF"), Ogre::PF_SHORT_L);

IMPLEMENT_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageTIFF32F, _T("TIFF"),
                                            _T("*.TIF;*.TIFF"), Ogre::PF_FLOAT32_R);

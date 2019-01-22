/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldFileDecoderImage.h"
#include "HeightfieldFileDecoderRaw.h"
#include "Utils/SerialMemoryBuffer.h"
#include "Utils/BMPHeader.h"

#define UIELEMENTPROPERTY_FLIPX _T("FLIPX")
#define UIELEMENTPROPERTY_FLIPY _T("FLIPY")

using namespace ScapeEngine;

enum EPropertyId
{
    PROPERTYID_FLIPX,
    PROPERTYID_FLIPY,
};

bool HeightfieldFileDecoderImageBase::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValue(UIELEMENTPROPERTY_FLIPX, _T("0"));
    setUIElementPropertyValue(UIELEMENTPROPERTY_FLIPY, _T("0"));
    return true;
}

bool HeightfieldFileDecoderImageBase::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    map[UIELEMENTPROPERTY_FLIPX] = PROPERTYID_FLIPX;
    map[UIELEMENTPROPERTY_FLIPY] = PROPERTYID_FLIPY;
    return true;
}

string HeightfieldFileDecoderImageBase::setUIElementPropertyValue(const string& elementName,
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

bool HeightfieldFileDecoderImageBase::decode(HeightfieldBuffer* outBuffer, const string& fileName,
                                             string* error)
{
    Ogre::LogManager::getSingleton().logMessage(_T("Importing heightfield '") + outBuffer->getName() +
                                                _T("' from file '") + fileName + _T("' using decoder '") +
                                                getClassName() + _T("'"));
    assert(outBuffer);

    std::pair<void*, size_t> fileData = Utils::readBinaryFile(fileName);
    if (fileData.first)
    {
        bool bigEndian = (OGRE_ENDIAN == OGRE_ENDIAN_BIG);
        Ogre::PixelBox rawPixelBox;
        Ogre::Image image;
        rawPixelBox.data = NULL;

        if (fileData.second >= sizeof(Utils::BitmapFileHeader) + sizeof(Utils::BitmapInfoHeader))
        {
            char* dataPtr = (char*)fileData.first;
            Utils::BitmapFileHeader bmpFH = *(Utils::BitmapFileHeader*)dataPtr;
            Utils::BitmapInfoHeader bmpIH =
                *(Utils::BitmapInfoHeader*)(dataPtr + sizeof(Utils::BitmapFileHeader));
            size_t headerSize = sizeof(Utils::BitmapFileHeader) + sizeof(Utils::BitmapInfoHeader);

            if (bigEndian)
            {
                bmpFH.flipByteOrder();
                bmpIH.flipByteOrder();
            }

            if (bmpFH.type == ('B' + 256 * 'M') && bmpIH.compression == Utils::BITMAPCOMPRESSION_RGB &&
                bmpIH.planes == 1 && bmpIH.bitCount == 16 &&
                fileData.second == (headerSize + bmpIH.width * bmpIH.height * 2))
            {
                rawPixelBox =
                    Ogre::PixelBox(bmpIH.width, bmpIH.height, 1, Ogre::PF_SHORT_L, dataPtr + headerSize);
            }
        }

        if (!rawPixelBox.data)
        {
            string splitFileName, splitExtension;
            Ogre::StringUtil::splitBaseFilename(fileName, splitFileName, splitExtension);
            Ogre::DataStreamPtr stream(new Ogre::MemoryDataStream(fileData.first, fileData.second));

            try
            {
                image.load(stream, splitExtension);
                rawPixelBox = image.getPixelBox();
            }
            catch (...)
            {
            }
        }

        if (rawPixelBox.data)
        {
            HeightfieldFileDecoderRaw::decode(&rawPixelBox, mFlipX, mFlipY, bigEndian, NULL);

            outBuffer->updateFrom(rawPixelBox);

            delete fileData.first;

            return true;
        }
        else if (error)
        {
            *error = _T("File could not be decoded succesfully");
        }

        delete fileData.first;
    }
    else if (error)
    {
        *error = _T("File could not be opened");
    }
    return false;
}

#define IMPLEMENT_HEIGHTFIELDFILEDECODERIMAGE_CLASS(className, extension, filter)                          \
    className::className() : HeightfieldFileDecoderImageBase()                                             \
    {                                                                                                      \
        loadPersistentProperties();                                                                        \
    }                                                                                                      \
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

IMPLEMENT_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImageBMP, _T("BMP"), _T("*.BMP;*.T3D"));
IMPLEMENT_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImagePGM, _T("PGM"), _T("*.PGM"));
IMPLEMENT_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImagePNG, _T("PNG"), _T("*.PNG"));
IMPLEMENT_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImageTGA, _T("TGA"), _T("*.TGA"));
IMPLEMENT_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImageTIFF, _T("TIFF"),
                                            _T("*.TIF;*.TIFF"));
IMPLEMENT_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImageEXR, _T("EXR"), _T("*.EXR"));

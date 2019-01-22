/**
 * ScapeEngine::HeightfieldFileEncoderImage classes
 *
 * Classes for exporting heightfields to different file types which are
 * supported by Ogre's image saving capabilities.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDFILEENCODERIMAGE_H__
#define __HEIGHTFIELDFILEENCODERIMAGE_H__

#include "ScapeEnginePrerequisites.h"

#include "EngineCore/UIElementClasses.h"
#include "HeightfieldFileEncoder.h"

namespace ScapeEngine
{

class HeightfieldFileEncoderImageBase : public HeightfieldFileEncoder
{
public:
    virtual bool encode(HeightfieldBuffer* inBuffer, const string& fileName, string* error = NULL);

protected:
    HeightfieldFileEncoderImageBase(const string& codecExtension, Ogre::PixelFormat pixelFormat)
        : mCodecExtension(codecExtension), mPixelFormat(pixelFormat), mFlipX(false), mFlipY(false)
    {
    }

    string mCodecExtension;
    Ogre::PixelFormat mPixelFormat;
    bool mFlipX, mFlipY;

    bool initPersistentElementValueMap(StringStringMap& map);
    bool initPersistentElementStringEnumMap(StringEnumMap& map);
    string setUIElementPropertyValue(const string& elementName, const string& value);
};

#define DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(className)                                                \
    class className : public HeightfieldFileEncoderImageBase                                               \
    {                                                                                                      \
        DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(className)                                                   \
    public:                                                                                                \
        className();                                                                                       \
        DEFINE_FACTORYCLASS(className, HeightfieldFileEncoder)                                             \
        virtual const string& getFileExtension();                                                          \
        virtual const string& getFileFilter();                                                             \
    };

DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageBMP8U);
DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImagePGM8U);
DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImagePNG8U);
DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageTGA8U);
DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageTIFF8U);

DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImagePGM16U);
DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImagePNG16U);
DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageTIFF16U);

DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageTIFF32F);
DEFINE_HEIGHTFIELDFILEENCODERIMAGE_CLASS(HeightfieldFileEncoderImageEXR32F);
}

#endif // __HEIGHTFIELDFILEENCODERIMAGE_H__

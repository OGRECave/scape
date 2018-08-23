/**
 * ScapeEngine::HeightfieldFileDecoderImage classes
 *
 * Classes for exporting heightfields to different file types which are
 * supported by Ogre's image saving capabilities.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDFILEDECODERIMAGE_H__
#define __HEIGHTFIELDFILEDECODERIMAGE_H__

#include "EngineCore/UIElementClasses.h"
#include "HeightfieldFileDecoder.h"

// ----------------------------------------------------------------------------
namespace ScapeEngine
{
class HeightfieldBuffer;

// ----------------------------------------------------------------------------
class HeightfieldFileDecoderImageBase : public HeightfieldFileDecoder
{
public:
    virtual bool decode(HeightfieldBuffer* inBuffer, const string& fileName, string* error = NULL);

protected:
    HeightfieldFileDecoderImageBase() : mFlipX(false), mFlipY(false) {}

    bool mFlipX, mFlipY;

    bool initPersistentElementValueMap(StringStringMap& map);
    bool initPersistentElementStringEnumMap(StringEnumMap& map);
    string setUIElementPropertyValue(const string& elementName, const string& value);
};

// ----------------------------------------------------------------------------
#define DEFINE_HEIGHTFIELDFILEDECODERIMAGE_CLASS(className)                                                \
    class className : public HeightfieldFileDecoderImageBase                                               \
    {                                                                                                      \
        DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(className)                                                   \
    public:                                                                                                \
        className();                                                                                       \
        DEFINE_FACTORYCLASS(className, HeightfieldFileDecoder)                                             \
        virtual const string& getFileExtension();                                                          \
        virtual const string& getFileFilter();                                                             \
    };

// ----------------------------------------------------------------------------
DEFINE_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImageBMP);
DEFINE_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImagePGM);
DEFINE_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImagePNG);
DEFINE_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImageTGA);
DEFINE_HEIGHTFIELDFILEDECODERIMAGE_CLASS(HeightfieldFileDecoderImageTIFF);
}

#endif // __HEIGHTFIELDFILEDECODERIMAGE_H__
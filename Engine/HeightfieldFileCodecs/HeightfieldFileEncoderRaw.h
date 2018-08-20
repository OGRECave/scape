/**
 * ScapeEngine::HeightfieldFileEncoderRaw classes
 *
 * Classes for exporting heightfields to different RAW file types
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDFILEENCODERRAW_H__
#define __HEIGHTFIELDFILEENCODERRAW_H__

#include "HeightfieldFileEncoder.h"

// ----------------------------------------------------------------------------
namespace ScapeEngine {
    class HeightfieldBuffer;

    // ----------------------------------------------------------------------------
    class HeightfieldFileEncoderRawBase : public HeightfieldFileEncoder
    {
    public:
        virtual bool encode(HeightfieldBuffer* inBuffer, const string& fileName, string* error = NULL);

        static bool encode(HeightfieldBuffer* inBuffer, FILE* fileHandle, bool flipX, bool flipY, bool bigEndian,
            Ogre::PixelFormat pixelFormat, string* error = NULL);

    protected:
        HeightfieldFileEncoderRawBase(Ogre::PixelFormat pixelFormat, bool bigEndian)
            : mPixelFormat(pixelFormat), mBigEndian(bigEndian), mFlipX(false), mFlipY(false)
        {
        }

        Ogre::PixelFormat mPixelFormat;
        bool mBigEndian;
        bool mFlipX, mFlipY;

        virtual bool initPersistentElementValueMap(StringStringMap& map);
        virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
        virtual string setUIElementPropertyValue(const string& elementName, const string& value);
    };

// ----------------------------------------------------------------------------
#define DEFINE_HEIGHTFIELDFILEENCODERRAW_CLASS(className)                                                              \
    class className : public HeightfieldFileEncoderRawBase                                                             \
    {                                                                                                                  \
        DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(className)                                                               \
    public:                                                                                                            \
        className();                                                                                                   \
        DEFINE_FACTORYCLASS(className, HeightfieldFileEncoder)                                                         \
        virtual const string& getFileExtension();                                                                      \
        virtual const string& getFileFilter();                                                                         \
    };

    // ----------------------------------------------------------------------------
    DEFINE_HEIGHTFIELDFILEENCODERRAW_CLASS(HeightfieldFileEncoderRaw8);
    DEFINE_HEIGHTFIELDFILEENCODERRAW_CLASS(HeightfieldFileEncoderRaw16L);
    DEFINE_HEIGHTFIELDFILEENCODERRAW_CLASS(HeightfieldFileEncoderRaw16B);
    DEFINE_HEIGHTFIELDFILEENCODERRAW_CLASS(HeightfieldFileEncoderRaw32F);
}

#endif // __HEIGHTFIELDFILEENCODERG16_H__
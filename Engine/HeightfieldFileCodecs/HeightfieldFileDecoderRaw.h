/**
 * ScapeEngine::HeightfieldFileDecoderRaw classes
 *
 * Classes for exporting heightfields to different file types which are
 * supported by Ogre's raw saving capabilities.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDFILEDECODERRAW_H__
#define __HEIGHTFIELDFILEDECODERRAW_H__

#include "ScapeEnginePrerequisites.h"

#include "EngineCore/UIElementClasses.h"
#include "HeightfieldFileDecoder.h"

namespace ScapeEngine
{

class HeightfieldFileDecoderRaw : public HeightfieldFileDecoder
{
    DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldFileDecoderRaw)
public:
    HeightfieldFileDecoderRaw();

    virtual bool decode(HeightfieldBuffer* inBuffer, const string& fileName, string* error = NULL);

    static bool decode(Ogre::PixelBox* pixelBox, bool flipX, bool flipY, bool bigEndian,
                       string* error = NULL);

protected:
    DEFINE_FACTORYCLASS(HeightfieldFileDecoderRaw, HeightfieldFileDecoder);

    string mCodecExtension;
    bool mFlipX, mFlipY;
    bool mBigEndian;
    int mBpp;
    int mWidth;
    int mHeight;

    virtual const string& getFileExtension();
    virtual const string& getFileFilter();

    bool initPersistentElementValueMap(StringStringMap& map);
    bool initPersistentElementStringEnumMap(StringEnumMap& map);
    string setUIElementPropertyValue(const string& elementName, const string& value);
};
}

#endif // __HEIGHTFIELDFILEDECODERRAW_H__
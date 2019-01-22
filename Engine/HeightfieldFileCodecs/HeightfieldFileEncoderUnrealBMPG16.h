/**
 * ScapeEngine::HeightfieldFileEncoderRaw classes
 *
 * Classes for exporting heightfields to different RAW file types
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDFILEENCODERUNREALBMPG16_H__
#define __HEIGHTFIELDFILEENCODERUNREALBMPG16_H__

#include "ScapeEnginePrerequisites.h"

#include "HeightfieldFileEncoder.h"

namespace ScapeEngine
{

class HeightfieldFileEncoderUnrealBMPG16 : public HeightfieldFileEncoder
{
    DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(className)
public:
    virtual bool encode(HeightfieldBuffer* inBuffer, const string& fileName, string* error = NULL);

    HeightfieldFileEncoderUnrealBMPG16();

protected:
    Ogre::PixelFormat mPixelFormat;
    bool mBigEndian;
    bool mFlipX, mFlipY;

    DEFINE_FACTORYCLASS(HeightfieldFileEncoderUnrealBMPG16, HeightfieldFileEncoder)
    virtual const string& getFileExtension();
    virtual const string& getFileFilter();

    virtual bool initPersistentElementValueMap(StringStringMap& map);
    virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
    virtual string setUIElementPropertyValue(const string& elementName, const string& value);
};
}

#endif // __HEIGHTFIELDFILEENCODERUNREALBMPG16_H__
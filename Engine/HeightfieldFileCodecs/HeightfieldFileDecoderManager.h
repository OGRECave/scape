/**
 * ScapeEngine::HeightfieldFileDecoderManager class
 *
 * Manages internally registered HeightfieldFileDecoder class objects
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDFILEDECODERMANAGER_H__
#define __HEIGHTFIELDFILEDECODERMANAGER_H__

#include "EngineCore/UIElementClasses.h"

namespace ScapeEngine {
    class HeightfieldFileDecoder;

    class HeightfieldFileDecoderManager
    {
    public:
        HeightfieldFileDecoderManager();
        ~HeightfieldFileDecoderManager();

        StringList getDecoderNameList();
        StringStringStringPairMap getFileFilterMap();

        HeightfieldFileDecoder* getDecoder(const string& name);
        string getDecoderNameFromExtension(const string& extension);

    protected:
        typedef std::map<string, HeightfieldFileDecoder*> DecoderMap;
        DecoderMap mDecoderMap;
        StringStringStringPairMap mFileFilterMap;

        void registerDecoder(HeightfieldFileDecoder* decoder);
    };
}

#endif // __HEIGHTFIELDFILEDECODERMANAGER_H__
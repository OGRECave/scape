/**
 * ScapeEngine::HeightfieldFileEncoderManager class
 *
 * Manages internally registered HeightfieldFileEncoder class objects
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDFILEENCODERMANAGER_H__
#define __HEIGHTFIELDFILEENCODERMANAGER_H__

#include "EngineCore/UIElementClasses.h"

namespace ScapeEngine {
    class HeightfieldFileEncoder;

    class HeightfieldFileEncoderManager
    {
    public:
        HeightfieldFileEncoderManager();
        ~HeightfieldFileEncoderManager();

        StringList getEncoderNameList();
        StringStringStringPairMap getFileFilterMap();

        HeightfieldFileEncoder* getEncoder(const string& name);

    protected:
        typedef std::map<string, HeightfieldFileEncoder*> EncoderMap;
        EncoderMap mEncoderMap;
        StringStringStringPairMap mFileFilterMap;

        void registerEncoder(HeightfieldFileEncoder* encoder);
    };
}

#endif // __HEIGHTFIELDFILEENCODERMANAGER_H__
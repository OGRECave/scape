/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldFileDecoderManager.h"
#include "HeightfieldFileDecoder.h"
#include "HeightfieldFileDecoderImage.h"
#include "HeightfieldFileDecoderRaw.h"

#define FILEFILTERSEPERATOR _T(";")

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldFileDecoderManager::HeightfieldFileDecoderManager()
{
    registerDecoder(new HeightfieldFileDecoderImageBMP);
    registerDecoder(new HeightfieldFileDecoderImagePGM);
    registerDecoder(new HeightfieldFileDecoderImagePNG);
    registerDecoder(new HeightfieldFileDecoderImageTGA);
    registerDecoder(new HeightfieldFileDecoderImageTIFF);
    registerDecoder(new HeightfieldFileDecoderImageEXR);
    registerDecoder(new HeightfieldFileDecoderRaw);
}

// ----------------------------------------------------------------------------
HeightfieldFileDecoderManager::~HeightfieldFileDecoderManager()
{
    DecoderMap::iterator decoderIt, decoderItEnd = mDecoderMap.end();
    for (decoderIt = mDecoderMap.begin(); decoderIt != decoderItEnd; ++decoderIt)
    {
        delete decoderIt->second;
    }
}

// ----------------------------------------------------------------------------
StringList HeightfieldFileDecoderManager::getDecoderNameList()
{
    StringList list;

    DecoderMap::iterator decoderIt, decoderItEnd = mDecoderMap.end();
    for (decoderIt = mDecoderMap.begin(); decoderIt != decoderItEnd; ++decoderIt)
    {
        list.push_back(decoderIt->first);
    }

    return list;
}

// ----------------------------------------------------------------------------
StringStringStringPairMap HeightfieldFileDecoderManager::getFileFilterMap() { return mFileFilterMap; }

// ----------------------------------------------------------------------------
HeightfieldFileDecoder* HeightfieldFileDecoderManager::getDecoder(const string& name)
{
    HeightfieldFileDecoder* decoder = NULL;

    DecoderMap::iterator decoderIt = mDecoderMap.find(name);
    if (decoderIt != mDecoderMap.end())
    {
        decoder = decoderIt->second;
    }

    return decoder;
}

// ----------------------------------------------------------------------------
string HeightfieldFileDecoderManager::getDecoderNameFromExtension(const string& extension)
{
    DecoderMap::iterator decoderIt, decoderItEnd = mDecoderMap.end();
    for (decoderIt = mDecoderMap.begin(); decoderIt != decoderItEnd; ++decoderIt)
    {
        string fileFilterString = decoderIt->second->getFileFilter();
        std::vector<Ogre::String> fileFilters = Ogre::StringUtil::split(fileFilterString, _T(";"));
        std::vector<Ogre::String>::iterator fileFilterIt, fileFilterItEnd = fileFilters.end();
        for (fileFilterIt = fileFilters.begin(); fileFilterIt != fileFilterItEnd; ++fileFilterIt)
        {
            if (Ogre::StringUtil::match(extension, *fileFilterIt, false))
            {
                return decoderIt->first;
            }
        }
    }
    return Utils::emptyString;
}

// ----------------------------------------------------------------------------
void HeightfieldFileDecoderManager::registerDecoder(HeightfieldFileDecoder* decoder)
{
    assert(mDecoderMap.find(decoder->getClassName()) == mDecoderMap.end());

    mDecoderMap[decoder->getClassName()] = decoder;

    mFileFilterMap[decoder->getClassName()] =
        std::pair<string, string>(decoder->getFileExtension(), decoder->getFileFilter());

    string& combinedFileFilterString = mFileFilterMap[Utils::emptyString].second;
    std::vector<Ogre::String> combinedFileFilters =
        Ogre::StringUtil::split(combinedFileFilterString, FILEFILTERSEPERATOR);

    string fileFilterString = decoder->getFileFilter();
    std::vector<Ogre::String> fileFilters = Ogre::StringUtil::split(fileFilterString, FILEFILTERSEPERATOR);
    std::vector<Ogre::String>::iterator fileFilterIt, fileFilterItEnd = fileFilters.end();
    for (fileFilterIt = fileFilters.begin(); fileFilterIt != fileFilterItEnd; ++fileFilterIt)
    {
        bool unique = true;
        std::vector<Ogre::String>::iterator combinedFileFilterIt,
            combinedFileFilterItEnd = combinedFileFilters.end();
        for (combinedFileFilterIt = combinedFileFilters.begin();
             combinedFileFilterIt != combinedFileFilterItEnd; ++combinedFileFilterIt)
        {
            if (Ogre::StringUtil::match(*combinedFileFilterIt, *fileFilterIt, false))
            {
                unique = false;
            }
        }
        if (unique)
        {
            if (combinedFileFilterString.length())
                combinedFileFilterString += FILEFILTERSEPERATOR;
            combinedFileFilterString += *fileFilterIt;
        }
    }
}

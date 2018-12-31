/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldBuffer.h"
#include "HeightfieldBufferSetManager.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldBufferSetManager::HeightfieldBufferSetManager() {}

// ----------------------------------------------------------------------------
HeightfieldBufferSetManager::~HeightfieldBufferSetManager()
{
    HeightfieldBufferSetMap::iterator it = mHeightfieldBufferSetMap.begin(),
                                      itEnd = mHeightfieldBufferSetMap.end();
    for (; it != itEnd; ++it)
    {
        delete it->second;
    }
}

// ----------------------------------------------------------------------------
HeightfieldBufferSetHandle
HeightfieldBufferSetManager::create(const string& name, int elementColumnCount, int elementRowCount,
                                    Ogre::Real heightRangeMin, Ogre::Real heightRangeMax,
                                    int elementColumnCountPerPage, int elementRowCountPerPage)
{
    HeightfieldBufferSet* heightfieldBufferSet =
        new HeightfieldBufferSet(name, elementColumnCount, elementRowCount, heightRangeMin, heightRangeMax,
                                 elementColumnCountPerPage, elementRowCountPerPage);

    HeightfieldBufferSetHandle handle = heightfieldBufferSet->getHandle();
    mHeightfieldBufferSetMap[handle] = heightfieldBufferSet;

    return handle;
}

// ----------------------------------------------------------------------------
HeightfieldBufferSetHandle HeightfieldBufferSetManager::findHandle(const string& name)
{
    HeightfieldBufferSetMap::iterator it = mHeightfieldBufferSetMap.begin(),
                                      itEnd = mHeightfieldBufferSetMap.end();
    for (; it != itEnd; ++it)
    {
        if (it->second->getName() == name)
        {
            return it->first;
        }
    }
    return 0;
}

// ----------------------------------------------------------------------------
bool HeightfieldBufferSetManager::destroy(HeightfieldBufferHandle handle)
{
    HeightfieldBufferSetMap::iterator it = mHeightfieldBufferSetMap.find(handle);

    if (it != mHeightfieldBufferSetMap.end())
    {
        delete it->second;
        mHeightfieldBufferSetMap.erase(it);
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
HeightfieldBufferSet* HeightfieldBufferSetManager::get(HeightfieldBufferSetHandle handle)
{
    HeightfieldBufferSetMap::iterator it = mHeightfieldBufferSetMap.find(handle);

    return it != mHeightfieldBufferSetMap.end() ? it->second : NULL;
}

// ----------------------------------------------------------------------------
void HeightfieldBufferSetManager::unlockAll()
{
    HeightfieldBufferSetMap::iterator it = mHeightfieldBufferSetMap.begin(),
                                      itEnd = mHeightfieldBufferSetMap.end();
    for (; it != itEnd; ++it)
    {
        it->second->unlockAll();
    }
}
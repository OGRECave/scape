/**
 * ScapeEngine::HeightfieldBufferSet class
 *
 * Manages HeightfieldBuffer objects
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDBUFFERSET_H__
#define __HEIGHTFIELDBUFFERSET_H__

#include "HeightfieldBuffer.h"

namespace ScapeEngine
{
typedef Utils::GUID HeightfieldBufferSetHandle;

class HeightfieldBufferSet
{
public:
    HeightfieldBufferSet(const string& name, int elementColumnCount, int elementRowCount,
                         Ogre::Real heightRangeMin, Ogre::Real heightRangeMax,
                         int elementColumnCountPerPage, int elementRowCountPerPage);

    ~HeightfieldBufferSet();

    inline const string& getName() { return mName; }

    inline HeightfieldBufferHandle getHandle() const { return mHandle; }

    // Add a new HeightfieldBuffer object to the set and return its handle
    HeightfieldBufferHandle create(const string& name, Ogre::PixelFormat format);

    // Add a new HeightfieldBuffer object to the set and return its handle
    HeightfieldBufferHandle createFrom(const string& name, HeightfieldBufferHandle originalBufferHandle,
                                       bool sharePages);

    HeightfieldBufferHandle findHandle(const string& name);

    inline int getElementColumnCountPerPage() const { return mElementColumnCountPerPage; }
    inline int getElementRowCountPerPage() const { return mElementRowCountPerPage; }
    inline int getElementColumnCount() const { return mElementColumnCount; }
    inline int getElementRowCount() const { return mElementRowCount; }
    inline int getPageColumnCount() const { return mPageColumnCount; }
    inline int getPageRowCount() const { return mPageRowCount; }
    inline Ogre::Real getHeightRangeMin() const { return mHeightRangeMin; }
    inline Ogre::Real getHeightRangeMax() const { return mHeightRangeMax; }

    bool destroy(HeightfieldBufferHandle handle);

    // Return heightfieldBuffer for some (PLSM) scene
    HeightfieldBuffer* get(HeightfieldBufferHandle handle);

    void unlockAll();

private:
    typedef std::map<HeightfieldBufferHandle, HeightfieldBuffer*> HeightfieldBufferMap;

    HeightfieldBufferMap mHeightfieldBufferMap;

    string mName;

    HeightfieldBufferSetHandle mHandle;

    int mElementColumnCountPerPage;
    int mElementRowCountPerPage;
    int mElementColumnCount;
    int mElementRowCount;
    int mPageRowCount;
    int mPageColumnCount;
    Ogre::Real mHeightRangeMin;
    Ogre::Real mHeightRangeMax;
};
}

#endif // __HEIGHTFIELDBUFFERSET_H__

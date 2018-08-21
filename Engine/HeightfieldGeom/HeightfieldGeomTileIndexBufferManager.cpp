/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "HeightfieldGeomTileIndexBufferManager.h"
#include "PCH/stdafx.h"

#define PACK4UINT8(a, b, c, d) ((d << 24) | (c << 16) | (b << 8) | (a))
#define UNPACK4UINT8(u32, index) ((u32 >> (index << 3)) & 0xFF)

// ----------------------------------------------------------------------------
namespace ScapeEngine
{
namespace HeightfieldGeomTileIndexBufferManagerNS
{
/**
 * Sortable 'key' description to a cachable vertex UV buffer
 */
struct CachedBufferDescription
{
    Ogre::HardwareIndexBuffer::IndexType type;
    int quadColumnCount;
    int quadRowCount;
    int spacingNorth;
    int spacingEast;
    int spacingSouth;
    int spacingWest;

    // ----------------------------------------------------------------------------
    inline bool operator==(const CachedBufferDescription& rhs) const
    {
        return type == rhs.type && quadColumnCount == rhs.quadColumnCount &&
               quadRowCount == rhs.quadRowCount && spacingNorth == rhs.spacingNorth &&
               spacingEast == rhs.spacingEast && spacingSouth == rhs.spacingSouth &&
               spacingWest == rhs.spacingWest;
    }

    // ----------------------------------------------------------------------------
    inline bool operator<(const CachedBufferDescription& rhs) const
    {
        if (type == rhs.type)
        {
            if (quadColumnCount == rhs.quadColumnCount)
            {
                if (quadRowCount == rhs.quadRowCount)
                {
                    if (spacingNorth == rhs.spacingNorth)
                    {
                        if (spacingEast == rhs.spacingEast)
                        {
                            if (spacingSouth == rhs.spacingSouth)
                            {
                                return spacingWest < rhs.spacingWest;
                            }
                            return spacingSouth < rhs.spacingSouth;
                        }
                        return spacingEast < rhs.spacingEast;
                    }
                    return spacingNorth < rhs.spacingNorth;
                }
                return quadRowCount < rhs.quadRowCount;
            }
            return quadColumnCount < rhs.quadColumnCount;
        }
        return type < rhs.type;
    }
};

// ----------------------------------------------------------------------------
// Shared pointer to and TTL for a cached vertex UV buffer
struct CachedBufferReference
{
    Ogre::HardwareIndexBufferSharedPtr buffer;
    float timeToLive;
};

// ----------------------------------------------------------------------------
class IndexSpacer
{
public:
    IndexSpacer(int spacing, int max, bool reverse)
    {
        if (reverse)
        {
            mBias = spacing - ((spacing + 1) >> 1);
        }
        else
        {
            mBias = ((spacing + 1) >> 1) - 1;
        }

        mMask = -spacing;
        mMax = max;
    }

    inline int space(int index)
    {
        int diff = ((index + mBias) & mMask) - mMax;
        return mMax + (diff & (diff >> 31));
    }

private:
    int mBias;
    int mMask;
    int mMax;
};
}
}

// ----------------------------------------------------------------------------
using namespace ScapeEngine;
using namespace HeightfieldGeomTileIndexBufferManagerNS;

// ----------------------------------------------------------------------------
HeightfieldGeomTileIndexBufferManager::HeightfieldGeomTileIndexBufferManager() : mBufferTimeToLive(1.0f)
{
    wantsPostFrameTick();

    mCachedBuffers = new CachedBuffers();
}

// ----------------------------------------------------------------------------
HeightfieldGeomTileIndexBufferManager::~HeightfieldGeomTileIndexBufferManager() { delete mCachedBuffers; }

// ----------------------------------------------------------------------------
Ogre::HardwareIndexBufferSharedPtr
HeightfieldGeomTileIndexBufferManager::getBuffer(int quadColumnCount, int quadRowCount, int spacingNorth,
                                                 int spacingEast, int spacingSouth, int spacingWest)
{
    Ogre::HardwareIndexBuffer::IndexType type;
    if ((quadColumnCount + 1) * (quadRowCount + 1) <= 65536)
    {
        type = Ogre::HardwareIndexBuffer::IT_16BIT;
    }
    else
    {
        type = Ogre::HardwareIndexBuffer::IT_32BIT;
    }

    CachedBufferDescription key = {type,        quadColumnCount, quadRowCount, spacingNorth,
                                   spacingEast, spacingSouth,    spacingWest};
    CachedBuffers::iterator bufferIt = mCachedBuffers->find(key);

    if (bufferIt == mCachedBuffers->end())
    {
        CachedBufferReference value;
        value.timeToLive = mBufferTimeToLive;

        switch (type)
        {
        case Ogre::HardwareIndexBuffer::IT_16BIT:
            value.buffer = createBuffer<Ogre::uint16>(type, quadColumnCount, quadRowCount, spacingNorth,
                                                      spacingEast, spacingSouth, spacingWest);
            break;

        case Ogre::HardwareIndexBuffer::IT_32BIT:
            value.buffer = createBuffer<Ogre::uint32>(type, quadColumnCount, quadRowCount, spacingNorth,
                                                      spacingEast, spacingSouth, spacingWest);
            break;
        }
        (*mCachedBuffers)[key] = value;

        return value.buffer;
    }

    return bufferIt->second.buffer;
}

// ----------------------------------------------------------------------------
void HeightfieldGeomTileIndexBufferManager::onPostFrameTick()
{
    float deltaTime = getEngineCore()->getTimeSinceLastFrame();

    CachedBuffers::iterator bufferItEnd = mCachedBuffers->end();
    for (CachedBuffers::iterator bufferIt = mCachedBuffers->begin(); bufferIt != bufferItEnd;)
    {
        if (bufferIt->second.buffer.unique())
        {
            bufferIt->second.timeToLive -= deltaTime;

            if (bufferIt->second.timeToLive <= 0)
            {
                mCachedBuffers->erase(bufferIt++);
                continue;
            }
        }
        else
        {
            bufferIt->second.timeToLive = mBufferTimeToLive;
        }
        ++bufferIt;
    }
}

// ----------------------------------------------------------------------------
template <class IndexType>
Ogre::HardwareIndexBufferSharedPtr
HeightfieldGeomTileIndexBufferManager::createBuffer(Ogre::HardwareIndexBuffer::IndexType type,
                                                    int quadColumnCount, int quadRowCount, int spacingNorth,
                                                    int spacingEast, int spacingSouth, int spacingWest)
{
    size_t indexCount;
    indexCount = 2 * (quadColumnCount + 1) * quadRowCount; // sum of seperate triangle row-strips
    indexCount += 2 * (quadRowCount - 1);                  // sum of degenerate triangle strip stitching

    assert((quadColumnCount + 1) * (quadRowCount + 1) <= ((long long)1 << (sizeof(IndexType) << 3)) &&
           _T("Index bitsize insufficient"));

    // only support power-of-two spacings
    assert((spacingNorth & (spacingNorth - 1)) == 0 && spacingNorth > 0);
    assert((spacingEast & (spacingEast - 1)) == 0 && spacingEast > 0);
    assert((spacingSouth & (spacingSouth - 1)) == 0 && spacingSouth > 0);
    assert((spacingWest & (spacingWest - 1)) == 0 && spacingWest > 0);

    spacingNorth = Utils::clamp(spacingNorth, 1, quadColumnCount);
    spacingEast = Utils::clamp(spacingEast, 1, quadRowCount);
    spacingSouth = Utils::clamp(spacingSouth, 1, quadColumnCount);
    spacingWest = Utils::clamp(spacingWest, 1, quadRowCount);

    Ogre::HardwareIndexBufferSharedPtr buffer =
        Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
            type, indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);

    IndexType* indexPtr = static_cast<IndexType*>(buffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
    assert(indexPtr);

    IndexSpacer spacerNorth(spacingNorth, quadColumnCount, false);
    IndexSpacer spacerEast(spacingEast, quadRowCount, true);
    IndexSpacer spacerSouth(spacingSouth, quadColumnCount, true);
    IndexSpacer spacerWest(spacingWest, quadRowCount, false);

    int row = 0;
    int column = 0;
    int rowNext = row + 1;
    int indexRowPitch = quadColumnCount + 1;
    int indexRow = 0;
    int indexWest = 0;
    int indexEast = quadColumnCount;
    int indexNextRow = indexRowPitch;

    int indexNextWest = spacerWest.space(rowNext) * indexRowPitch;
    int indexNextEast = spacerEast.space(rowNext) * indexRowPitch + quadColumnCount;

    // first row
    // first column
    *indexPtr++ = (IndexType)(indexWest);
    *indexPtr++ = (IndexType)(indexNextWest);

    if (quadRowCount == 1)
    {
        // second column to secondlast column (inclusive) in the case of only one quad-row
        for (column = 1; column < quadColumnCount; ++column)
        {
            *indexPtr++ = (IndexType)(spacerNorth.space(column));
            *indexPtr++ = (IndexType)(spacerSouth.space(column) + indexNextRow);
        }
    }
    else
    {
        // second column to secondlast column (inclusive) in the case of quad-rows following after this one
        for (column = 1; column < quadColumnCount; ++column)
        {
            *indexPtr++ = (IndexType)(spacerNorth.space(column));
            *indexPtr++ = (IndexType)(indexNextRow + column);
        }
    }
    // last column
    *indexPtr++ = (IndexType)(indexEast);
    *indexPtr++ = (IndexType)(indexNextEast);

    // second row to secondlast row (inclusive)
    while (rowNext < quadRowCount - 1)
    {
        row = rowNext;
        indexRow = indexNextRow;
        indexEast = indexNextEast;
        indexWest = indexNextWest;

        ++rowNext;
        indexNextRow += indexRowPitch;
        indexNextWest = spacerWest.space(rowNext) * indexRowPitch;
        indexNextEast = spacerEast.space(rowNext) * indexRowPitch + quadColumnCount;

        // triangle strip stitching by adding two degenerate triangles
        *indexPtr++ = (IndexType)(indexEast);
        *indexPtr++ = (IndexType)(indexWest);

        // first column
        *indexPtr++ = (IndexType)(indexWest);
        *indexPtr++ = (IndexType)(indexNextWest);

        // second column to secondlast column (inclusive)
        for (column = 1; column < quadColumnCount; ++column)
        {
            *indexPtr++ = (IndexType)(indexRow + column);
            *indexPtr++ = (IndexType)(indexNextRow + column);
        }

        // last column
        *indexPtr++ = (IndexType)(indexEast);
        *indexPtr++ = (IndexType)(indexNextEast);
    }

    // last row, in the case of more than one quad-rows
    if (rowNext < quadRowCount)
    {
        row = rowNext;
        indexRow = indexNextRow;
        indexEast = indexNextEast;
        indexWest = indexNextWest;

        ++rowNext;
        indexNextRow += indexRowPitch;
        indexNextWest = indexNextRow;
        indexNextEast = indexNextRow + quadColumnCount;

        // triangle strip stitching by adding two degenerate triangles
        *indexPtr++ = (IndexType)(indexEast);
        *indexPtr++ = (IndexType)(indexWest);

        // first column
        *indexPtr++ = (IndexType)(indexWest);
        *indexPtr++ = (IndexType)(indexNextWest);

        // second column to secondlast column (inclusive)
        for (column = 1; column < quadColumnCount; ++column)
        {
            *indexPtr++ = (IndexType)(indexRow + column);
            *indexPtr++ = (IndexType)(spacerSouth.space(column) + indexNextRow);
        }

        // last column
        *indexPtr++ = (IndexType)indexEast;
        *indexPtr++ = (IndexType)indexNextEast;
    }

    buffer->unlock();

    return buffer;
}

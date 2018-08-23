/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
#include "HeightfieldBuffer.h"
#include "HeightfieldBufferPage.h"
#include "HeightfieldBufferSet.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldBufferPage::HeightfieldBufferPage(HeightfieldBuffer* heightfieldBuffer, int pageColumn,
                                             int pageRow)
    : mVersion(Utils::createGUID()), mPageColumn(pageColumn), mPageRow(pageRow)
{
    _mHeightTexture.setNull();
    mHeightfieldBuffers.push_back(heightfieldBuffer);
    mHeightfieldBufferSet = heightfieldBuffer->getHeightfieldBufferSet();
    mHeightElementFormat = heightfieldBuffer->getHeightElementFormat();
}

// ----------------------------------------------------------------------------
HeightfieldBufferPage::~HeightfieldBufferPage() { clearData(); }

// ----------------------------------------------------------------------------
void HeightfieldBufferPage::addHeightfieldBuffer(HeightfieldBuffer* heightfieldBuffer)
{
    std::list<HeightfieldBuffer *>::iterator bufferIt = mHeightfieldBuffers.begin(),
                                             bufferItEnd = mHeightfieldBuffers.end();
    for (; bufferIt != bufferItEnd; ++bufferIt)
    {
        assert(*bufferIt != heightfieldBuffer);
    }
    assert(mHeightfieldBufferSet == heightfieldBuffer->getHeightfieldBufferSet());
    assert(mHeightElementFormat == heightfieldBuffer->getHeightElementFormat());
    mHeightfieldBuffers.push_back(heightfieldBuffer);
}

// ----------------------------------------------------------------------------
void HeightfieldBufferPage::removeAllHeightfieldBuffers() { mHeightfieldBuffers.clear(); }

// ----------------------------------------------------------------------------
void HeightfieldBufferPage::removeHeightfieldBuffer(HeightfieldBuffer* heightfieldBuffer)
{
    std::list<HeightfieldBuffer *>::iterator bufferIt = mHeightfieldBuffers.begin(),
                                             bufferItEnd = mHeightfieldBuffers.end();
    for (; bufferIt != bufferItEnd; ++bufferIt)
    {
        if (*bufferIt == heightfieldBuffer)
        {
            mHeightfieldBuffers.erase(bufferIt);
            return;
        }
    }
    assert(false);
}

// ----------------------------------------------------------------------------
bool HeightfieldBufferPage::containsData() { return !_mHeightTexture.isNull(); }

// ----------------------------------------------------------------------------
void HeightfieldBufferPage::clearData()
{
    unlockAll();
    if (containsData())
    {
        Ogre::TextureManager::getSingleton().remove(_mHeightTexture->getHandle());
        assert(_mHeightTexture.useCount() == 1);
        _mHeightTexture.setNull();
    }
}

// ----------------------------------------------------------------------------
Ogre::TexturePtr HeightfieldBufferPage::getHeightTexture()
{
    if (!containsData())
    {
        // Create the texture
        static int createdPageNumber = 0;
        ++createdPageNumber;

        int bufferWidth = getHeightfieldBufferSet()->getElementColumnCountPerPage();
        int bufferHeight = getHeightfieldBufferSet()->getElementRowCountPerPage();

        string textureName =
            _T("HeightfieldBufferPage") + Ogre::StringConverter::toString(createdPageNumber);

        _mHeightTexture = Ogre::TextureManager::getSingleton().createManual(
            textureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
            bufferWidth, bufferHeight, 1, getPackedFormat(), Ogre::TU_STATIC);

        /*
        getHeightTexture()->getBuffer()->lock(Ogre::HardwareBuffer::HBL_DISCARD);
        Ogre::PixelBox pixelBox = getHeightTexture()->getBuffer()->getCurrentLock();
        assert(pixelBox.data);
        memset(pixelBox.data, 0, Ogre::PixelUtil::getNumElemBytes(pixelBox.format) * pixelBox.slicePitch);
        getHeightTexture()->getBuffer()->unlock();
        */

        printf("Created page '%s'\n", textureName.c_str());
    }

    return _mHeightTexture;
}

// ----------------------------------------------------------------------------
void HeightfieldBufferPage::increaseVersion()
{
    mVersion = Utils::createGUID();

    std::list<HeightfieldBuffer *>::iterator bufferIt = mHeightfieldBuffers.begin(),
                                             bufferItEnd = mHeightfieldBuffers.end();
    for (; bufferIt != bufferItEnd; ++bufferIt)
    {
        (*bufferIt)->increaseVersion();
    }
}

// ----------------------------------------------------------------------------
Ogre::PixelBox HeightfieldBufferPage::getRawHeightData()
{
    if (!getHeightTexture()->getBuffer()->isLocked())
    {
        getHeightTexture()->getBuffer()->lock(Ogre::HardwareBuffer::HBL_NORMAL);
    }

    Ogre::PixelBox box = getHeightTexture()->getBuffer()->getCurrentLock();
    box.format = getHeightElementFormat();
    return box;
}

// ----------------------------------------------------------------------------
void HeightfieldBufferPage::unlockAll()
{
    if (containsData())
    {
        if (getHeightTexture()->getBuffer()->isLocked())
        {
            getHeightTexture()->getBuffer()->unlock();
        }
    }
}

// ----------------------------------------------------------------------------
bool HeightfieldBufferPage::revalidatePass() { return false; }

// ----------------------------------------------------------------------------
void HeightfieldBufferPage::cloneDataFrom(HeightfieldBufferPage* const heightfieldBufferPage)
{
    assert(heightfieldBufferPage->getHeightfieldBufferSet() == getHeightfieldBufferSet());

    if (heightfieldBufferPage->containsData())
    {
        heightfieldBufferPage->unlockAll();
        unlockAll();
        getHeightTexture()->getBuffer()->blit(heightfieldBufferPage->getHeightTexture()->getBuffer());
        // printf("Cloned pagedata '%s' to '%s'\n",
        // heightfieldBufferPage->getHeightTexture()->getName().c_str(),
        // getHeightTexture()->getName().c_str());
    }
    else
    {
        _mHeightTexture.setNull();
    }

    //	increaseVersion();

    mVersion = heightfieldBufferPage->getVersion();
    mPageColumn = heightfieldBufferPage->getPageColumn();
    mPageRow = heightfieldBufferPage->getPageRow();

    std::list<HeightfieldBuffer *>::iterator bufferIt = mHeightfieldBuffers.begin(),
                                             bufferItEnd = mHeightfieldBuffers.end();
    for (; bufferIt != bufferItEnd; ++bufferIt)
    {
        (*bufferIt)->increaseVersion();
    }
}

// ----------------------------------------------------------------------------
void HeightfieldBufferPage::updateFrom(const Ogre::PixelBox& relativePixelBox)
{
    // source pixel box (from relativePixelBox)
    Ogre::PixelBox sourceBox = Utils::clipPixelBox(relativePixelBox, Utils::boxToRect(relativePixelBox));

    // destination pixel box (from mLockedHeightTextureData)
    Ogre::PixelBox destBox = Utils::clipPixelBox(getRawHeightData(), Utils::boxToRect(relativePixelBox));

    // blit from source to destination pixel box
    Ogre::PixelUtil::bulkPixelConversion(sourceBox, destBox);

    // mark changed area as dirty
    // Ogre::Rect invalidRect(relativePixelBox.left, relativePixelBox.top, relativePixelBox.right,
    // relativePixelBox.bottom);

    increaseVersion();

    Ogre::Rect absoluteRect = getAbsoluteRect();

    updateGeoms(
        Utils::translatedRect(Utils::boxToRect(relativePixelBox), absoluteRect.left, absoluteRect.top));

    // mDirtyRect = Utils::unionTRect(mDirtyRect, invalidRect);
}

// ----------------------------------------------------------------------------
void HeightfieldBufferPage::updateGeoms(const Ogre::Rect& rect)
{
    std::list<HeightfieldBuffer *>::iterator bufferIt = mHeightfieldBuffers.begin(),
                                             bufferItEnd = mHeightfieldBuffers.end();
    for (; bufferIt != bufferItEnd; ++bufferIt)
    {
        (*bufferIt)->updateGeoms(rect);
    }
}

// ----------------------------------------------------------------------------
void HeightfieldBufferPage::copyTo(Ogre::PixelBox& relativePixelBox)
{
    // source pixel box (from relativePixelBox)
    Ogre::PixelBox sourceBox = Utils::clipPixelBox(getRawHeightData(), Utils::boxToRect(relativePixelBox));

    // destination pixel box (from mLockedHeightTextureData)
    Ogre::PixelBox destBox = Utils::clipPixelBox(relativePixelBox, Utils::boxToRect(relativePixelBox));

    // blit from source to destination pixel box
    Ogre::PixelUtil::bulkPixelConversion(sourceBox, destBox);
}

// ----------------------------------------------------------------------------
Ogre::Rect HeightfieldBufferPage::getAbsoluteRect()
{
    Ogre::Rect rect;
    rect.left = getPageColumn() * getHeightfieldBufferSet()->getElementColumnCountPerPage();
    rect.top = getPageRow() * getHeightfieldBufferSet()->getElementRowCountPerPage();
    rect.right = rect.left + getHeightfieldBufferSet()->getElementColumnCountPerPage();
    rect.bottom = rect.top + getHeightfieldBufferSet()->getElementRowCountPerPage();
    return rect;
}

// ----------------------------------------------------------------------------
Ogre::PixelFormat HeightfieldBufferPage::getPackedFormat()
{
    switch (mHeightElementFormat)
    {
    case Ogre::PF_SHORT_L:
        return Ogre::PF_R5G6B5;
    case Ogre::PF_SHORT_GR:
        return Ogre::PF_R8G8B8;
    case Ogre::PF_FLOAT32_R:
        return Ogre::PF_FLOAT32_R;
    case Ogre::PF_FLOAT32_GR:
        return Ogre::PF_FLOAT32_GR;
    case Ogre::PF_FLOAT32_RGBA:
        return Ogre::PF_FLOAT32_RGBA;
    default:
        assert(false && _T("HeightfieldBufferPage::getPackedFormat(): Unsupported format"));
    }
    return Ogre::PF_UNKNOWN;
}

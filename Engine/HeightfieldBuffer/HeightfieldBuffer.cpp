/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "HeightfieldBuffer.h"
#include "HeightfieldBufferPage.h"
#include "HeightfieldBufferSet.h"
#include "HeightfieldGeom/HeightfieldGeom.h"
#include "HeightfieldGeom/HeightfieldGeomManager.h"
#include "PCH/stdafx.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldBuffer::HeightfieldBuffer(HeightfieldBufferSet* set)
    : mHeightfieldBufferSet(set), mName(_T("empty")), mVersion(Utils::createGUID()),
      // mElementColumnCountPerPage(0),
      // mElementRowCountPerPage(0),
      // mElementColumnCount(0),
      // mElementRowCount(0),
      // mHeightRangeMin(0.0f),
      // mHeightRangeMax(0.0f),
      mHeightElementFormat(Ogre::PF_UNKNOWN)
{
    mHandle = Utils::createGUID();
}

// ----------------------------------------------------------------------------
HeightfieldBuffer::HeightfieldBuffer(HeightfieldBufferSet* set, const string& name,
                                     Ogre::PixelFormat pixelFormat, int elementColumnCountPerPage,
                                     int elementRowCountPerPage, int elementColumnCount,
                                     int elementRowCount, Ogre::Real heightRangeMin,
                                     Ogre::Real heightRangeMax)
    : mHeightfieldBufferSet(set), mName(name), mVersion(Utils::createGUID()),
      // mElementColumnCountPerPage(elementColumnCountPerPage),
      // mElementRowCountPerPage(elementRowCountPerPage),
      // mElementColumnCount(elementColumnCount),
      // mElementRowCount(elementRowCount),
      // mHeightRangeMin(heightRangeMin),
      // mHeightRangeMax(heightRangeMax),
      mHeightElementFormat(pixelFormat)
{
    mHandle = Utils::createGUID();
    // printf("Created heightfieldbuffer '%s'\n", mName.c_str());

    // mPageColumnCount = (int)ceil((Ogre::Real)elementColumnCount / elementColumnCountPerPage);
    // mPageRowCount = (int)ceil((Ogre::Real)elementRowCount / elementRowCountPerPage);

    int pageCount =
        getHeightfieldBufferSet()->getPageColumnCount() * getHeightfieldBufferSet()->getPageRowCount();
    mPages.resize(pageCount, NULL);

    for (int row = 0; row < getHeightfieldBufferSet()->getPageRowCount(); ++row)
    {
        for (int column = 0; column < getHeightfieldBufferSet()->getPageColumnCount(); ++column)
        {
            mPages[row * getHeightfieldBufferSet()->getPageColumnCount() + column] =
                new HeightfieldBufferPage(this, column, row);
        }
    }
}

// ----------------------------------------------------------------------------
HeightfieldBuffer::~HeightfieldBuffer()
{
    // printf("Deleted heightfieldbuffer '%s'\n", mName.c_str());
    for (int row = 0; row < getHeightfieldBufferSet()->getPageRowCount(); ++row)
    {
        for (int column = 0; column < getHeightfieldBufferSet()->getPageColumnCount(); ++column)
        {
            HeightfieldBufferPage* page =
                mPages[row * getHeightfieldBufferSet()->getPageColumnCount() + column];
            page->removeHeightfieldBuffer(this);
            if (page->getHeightfieldBufferCount() == 0)
            {
                delete page;
            }
        }
    }

    while (mHeightfieldGeoms.begin() != mHeightfieldGeoms.end())
    {
        getEngineCore()->getHeightfieldGeomManager()->destroy(*mHeightfieldGeoms.begin());
    }
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::cloneFrom(HeightfieldBuffer* heightfieldBuffer, bool sharePages)
{
    assert(heightfieldBuffer);

    mVersion = heightfieldBuffer->mVersion;
    mHeightElementFormat = heightfieldBuffer->mHeightElementFormat;
    mHeightfieldBufferSet = heightfieldBuffer->getHeightfieldBufferSet();
    // mElementColumnCountPerPage	= heightfieldBuffer->mElementColumnCountPerPage;
    // mElementRowCountPerPage		= heightfieldBuffer->mElementRowCountPerPage;
    // mElementColumnCount			= heightfieldBuffer->mElementColumnCount;
    // mElementRowCount			= heightfieldBuffer->mElementRowCount;
    // mPageRowCount				= heightfieldBuffer->mPageRowCount;
    // mPageColumnCount			= heightfieldBuffer->mPageColumnCount;
    // mHeightRangeMin				= heightfieldBuffer->mHeightRangeMin;
    // mHeightRangeMax				= heightfieldBuffer->mHeightRangeMax;

    // printf("Cloned heightfieldbuffer '%s' to '%s'\n", heightfieldBuffer->getName().c_str(),
    // getName().c_str());

    assert(heightfieldBuffer->mPages.size() == (size_t)(getHeightfieldBufferSet()->getPageRowCount() *
                                                        getHeightfieldBufferSet()->getPageColumnCount()));
    mPages.resize(std::max(mPages.size(), heightfieldBuffer->mPages.size()), NULL);

    for (int row = 0; row < getHeightfieldBufferSet()->getPageRowCount(); ++row)
    {
        for (int column = 0; column < getHeightfieldBufferSet()->getPageColumnCount(); ++column)
        {
            int pageIndex = row * getHeightfieldBufferSet()->getPageColumnCount() + column;
            if (mPages[pageIndex] != heightfieldBuffer->mPages[pageIndex])
            {
                if (mPages[pageIndex])
                {
                    mPages[pageIndex]->removeHeightfieldBuffer(this);
                    if (mPages[pageIndex]->getHeightfieldBufferCount() == 0)
                    {
                        delete mPages[pageIndex];
                        // printf("Deleted page from '%s'\n", getName().c_str());
                    }
                    mPages[pageIndex] = NULL;
                }
                mPages[pageIndex] = heightfieldBuffer->mPages[pageIndex];
                mPages[pageIndex]->addHeightfieldBuffer(this);
                // mPages[pageIndex]->updateGeoms();
            }

            if (!sharePages)
            {
                unsharePage(pageIndex, true);
            }
        }
    }
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::registerHeightfieldGeom(HeightfieldGeom* heightfieldGeom)
{
    HeightfieldGeoms::iterator it = mHeightfieldGeoms.begin(), itEnd = mHeightfieldGeoms.end();
    for (; it != itEnd; ++it)
    {
        if (*it == heightfieldGeom)
        {
            return;
        }
    }

    mHeightfieldGeoms.push_back(heightfieldGeom);
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::unregisterHeightfieldGeom(HeightfieldGeom* heightfieldGeom)
{
    HeightfieldGeoms::iterator it = mHeightfieldGeoms.begin(), itEnd = mHeightfieldGeoms.end();
    for (; it != itEnd; ++it)
    {
        if (*it == heightfieldGeom)
        {
            mHeightfieldGeoms.erase(it);
            return;
        }
    }
}

// ----------------------------------------------------------------------------
int HeightfieldBuffer::getPageIndexFromCoords(int pageColumnIndex, int pageRowIndex)
{
    if (pageColumnIndex >= 0 && pageColumnIndex < getHeightfieldBufferSet()->getPageColumnCount() &&
        pageRowIndex >= 0 && pageRowIndex < getHeightfieldBufferSet()->getPageRowCount())
    {
        return pageRowIndex * getHeightfieldBufferSet()->getPageColumnCount() + pageColumnIndex;
    }
    return -1;
}

// ----------------------------------------------------------------------------
HeightfieldBufferPage* HeightfieldBuffer::getPage(int pageColumnIndex, int pageRowIndex,
                                                  EPageAccessMode accessMode)
{
    int pageIndex = getPageIndexFromCoords(pageColumnIndex, pageRowIndex);
    if (pageIndex >= 0)
    {
        if (mPages[pageIndex]->getHeightfieldBufferCount() > 1)
        {
            if (accessMode == PAGEACCESSMODE_READWRITE)
            {
                unsharePage(pageIndex, true);
            }
            else if (accessMode == PAGEACCESSMODE_WRITE_DISCARD)
            {
                unsharePage(pageIndex, false);
            }
        }
        return mPages[pageIndex];
    }
    return NULL;
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::unsharePage(int index, bool copyData)
{
    assert((size_t)index < mPages.size());
    if (copyData)
    {
        // printf("Unsharing: copying page %d\n", index);
    }
    else
    {
        // printf("Unsharing: constructing page %d\n", index);
    }

    HeightfieldBufferPage* originalPage = mPages[index];
    if (originalPage->getHeightfieldBufferCount() > 1)
    {
        originalPage->removeHeightfieldBuffer(this);
        mPages[index] =
            new HeightfieldBufferPage(this, mPages[index]->getPageColumn(), mPages[index]->getPageRow());
        if (copyData)
        {
            mPages[index]->cloneDataFrom(originalPage);
            mPages[index]->removeAllHeightfieldBuffers();
            mPages[index]->addHeightfieldBuffer(this);
        }
        if (originalPage->getHeightfieldBufferCount() == 0)
        {
            delete originalPage;
        }
    }
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::replacePage(int pageColumnIndex, int pageRowIndex, HeightfieldBuffer* srcBuffer)
{
    HeightfieldBufferPage* srcPage =
        srcBuffer->getPage(pageColumnIndex, pageRowIndex, PAGEACCESSMODE_READONLY);
    int pageIndex = getPageIndexFromCoords(pageColumnIndex, pageRowIndex);
    if (pageIndex >= 0 && srcPage)
    {
        HeightfieldBufferPage* originalPage = mPages[pageIndex];
        if (originalPage != srcPage)
        {
            originalPage->removeHeightfieldBuffer(this);
            if (originalPage->getHeightfieldBufferCount() == 0)
            {
                delete originalPage;
            }
            mPages[pageIndex] = srcPage;

            srcPage->addHeightfieldBuffer(this);
        }
    }
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::swapPage(int pageColumnIndex, int pageRowIndex, HeightfieldBuffer* srcBuffer)
{
    int pageIndex = getPageIndexFromCoords(pageColumnIndex, pageRowIndex);

    int srcPageIndex = srcBuffer->getPageIndexFromCoords(pageColumnIndex, pageRowIndex);
    if (pageIndex >= 0 && srcPageIndex >= 0)
    {
        HeightfieldBufferPage* originalPage = mPages[pageIndex];
        HeightfieldBufferPage* srcPage = srcBuffer->mPages[srcPageIndex];
        if (originalPage != srcPage)
        {
            originalPage->removeHeightfieldBuffer(this);
            srcPage->removeHeightfieldBuffer(srcBuffer);

            originalPage->addHeightfieldBuffer(srcBuffer);
            srcBuffer->mPages[srcPageIndex] = originalPage;

            srcPage->addHeightfieldBuffer(this);
            mPages[pageIndex] = srcPage;
        }
    }
}

// ----------------------------------------------------------------------------
int HeightfieldBuffer::getPageColumnFromElement(int elementColumnIndex)
{
    if (elementColumnIndex >= 0 && elementColumnIndex < getHeightfieldBufferSet()->getElementColumnCount())
    {
        return elementColumnIndex / getHeightfieldBufferSet()->getElementColumnCountPerPage();
    }
    return -1;
}

// ----------------------------------------------------------------------------
int HeightfieldBuffer::getPageRowFromElement(int elementRowIndex)
{
    if (elementRowIndex >= 0 && elementRowIndex < getHeightfieldBufferSet()->getElementRowCount())
    {
        return elementRowIndex / getHeightfieldBufferSet()->getElementRowCountPerPage();
    }
    return -1;
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::revalidate()
{
    bool needsAnotherPass = true;
    while (needsAnotherPass)
    {
        needsAnotherPass = false;
        for (int pageRowIndex = 0; pageRowIndex < getHeightfieldBufferSet()->getPageRowCount();
             ++pageRowIndex)
        {
            for (int pageColumnIndex = 0; pageColumnIndex < getHeightfieldBufferSet()->getPageColumnCount();
                 ++pageColumnIndex)
            {
                needsAnotherPass =
                    getPage(pageColumnIndex, pageRowIndex, PAGEACCESSMODE_READONLY)->revalidatePass() ||
                    needsAnotherPass;
            }
        }
    }
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::updateFrom(const Ogre::PixelBox& relativePixelBox)
{
    Ogre::Rect pageRect;
    for (int pageRowIndex = 0; pageRowIndex < getHeightfieldBufferSet()->getPageRowCount(); ++pageRowIndex)
    {
        long pixelYStart = pageRowIndex * getHeightfieldBufferSet()->getElementRowCountPerPage();
        pageRect.top = std::max<long>(pixelYStart, relativePixelBox.top);
        pageRect.bottom = std::min<long>(
            pixelYStart + getHeightfieldBufferSet()->getElementRowCountPerPage(), relativePixelBox.bottom);

        if (pageRect.height() > 0)
        {
            for (int pageColumnIndex = 0; pageColumnIndex < getHeightfieldBufferSet()->getPageColumnCount();
                 ++pageColumnIndex)
            {
                long pixelXStart =
                    pageColumnIndex * getHeightfieldBufferSet()->getElementColumnCountPerPage();
                pageRect.left = std::max<long>(pixelXStart, relativePixelBox.left);
                pageRect.right =
                    std::min<long>(pixelXStart + getHeightfieldBufferSet()->getElementColumnCountPerPage(),
                                   relativePixelBox.right);

                if (pageRect.width() > 0)
                {
                    long offsetX = pageRect.left - pixelXStart;
                    long offsetY = pageRect.top - pixelYStart;
                    Ogre::PixelBox clipBox =
                        Utils::clipPixelBox(relativePixelBox, pageRect, offsetX, offsetY);

                    EPageAccessMode pageAccessMode;
                    if (clipBox.getWidth() ==
                            (size_t)getHeightfieldBufferSet()->getElementColumnCountPerPage() &&
                        clipBox.getHeight() ==
                            (size_t)getHeightfieldBufferSet()->getElementRowCountPerPage())
                    {
                        pageAccessMode = PAGEACCESSMODE_WRITE_DISCARD;
                    }
                    else
                    {
                        pageAccessMode = PAGEACCESSMODE_READWRITE;
                    }

                    getPage(pageColumnIndex, pageRowIndex, pageAccessMode)->updateFrom(clipBox);
                }
            }
        }
    }

    updateGeoms(Utils::boxToRect(relativePixelBox));
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::updateGeoms(const Ogre::Rect& rect)
{
    HeightfieldGeoms::iterator it = mHeightfieldGeoms.begin(), itEnd = mHeightfieldGeoms.end();
    for (; it != itEnd; ++it)
    {
        (*it)->markDirtyRect(rect);
    }
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::copyTo(Ogre::PixelBox& relativePixelBox)
{
    Ogre::Rect pageRect;
    for (int pageRowIndex = 0; pageRowIndex < getHeightfieldBufferSet()->getPageRowCount(); ++pageRowIndex)
    {
        long pixelYStart = pageRowIndex * getHeightfieldBufferSet()->getElementRowCountPerPage();
        pageRect.top = std::max<long>(pixelYStart, relativePixelBox.top);
        pageRect.bottom = std::min<long>(
            pixelYStart + getHeightfieldBufferSet()->getElementRowCountPerPage(), relativePixelBox.bottom);

        if (pageRect.height() > 0)
        {
            for (int pageColumnIndex = 0; pageColumnIndex < getHeightfieldBufferSet()->getPageColumnCount();
                 ++pageColumnIndex)
            {
                long pixelXStart =
                    pageColumnIndex * getHeightfieldBufferSet()->getElementColumnCountPerPage();
                pageRect.left = std::max<long>(pixelXStart, relativePixelBox.left);
                pageRect.right =
                    std::min<long>(pixelXStart + getHeightfieldBufferSet()->getElementColumnCountPerPage(),
                                   relativePixelBox.right);

                if (pageRect.width() > 0)
                {
                    long offsetX = pageRect.left - pixelXStart;
                    long offsetY = pageRect.top - pixelYStart;
                    Ogre::PixelBox clipBox =
                        Utils::clipPixelBox(relativePixelBox, pageRect, offsetX, offsetY);
                    getPage(pageColumnIndex, pageRowIndex, PAGEACCESSMODE_READONLY)->copyTo(clipBox);
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------
void HeightfieldBuffer::unlockAll()
{
    for (int pageRowIndex = 0; pageRowIndex < getHeightfieldBufferSet()->getPageRowCount(); ++pageRowIndex)
    {
        for (int pageColumnIndex = 0; pageColumnIndex < getHeightfieldBufferSet()->getPageColumnCount();
             ++pageColumnIndex)
        {
            mPages[pageRowIndex * getHeightfieldBufferSet()->getPageColumnCount() + pageColumnIndex]
                ->unlockAll();
        }
    }
}
/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "HeightfieldSerializer.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"

using namespace ScapeEngine;

Utils::SerialMemoryBufferPtr HeightfieldSerializer::serialize(HeightfieldBuffer* inBuffer,
                                                              Ogre::PixelFormat pixelFormat)
{
    assert(inBuffer);
    int elementCount = inBuffer->getHeightfieldBufferSet()->getElementColumnCount() *
                       inBuffer->getHeightfieldBufferSet()->getElementRowCount();
    int elementPageCount = inBuffer->getHeightfieldBufferSet()->getElementColumnCountPerPage() *
                           inBuffer->getHeightfieldBufferSet()->getElementRowCountPerPage();
    int bytesPerElement = Ogre::PixelUtil::getNumElemBytes(pixelFormat);
    int dataByteCount = elementCount * bytesPerElement;
    int dataPageByteCount = elementPageCount * bytesPerElement;
    int headerByteCount = 0;

    Utils::SerialMemoryBufferPtr serialBufferPtr(new Utils::SerialMemoryBuffer());
    serialBufferPtr.get()->setSize(headerByteCount + dataByteCount, false);

    Utils::SerialMemoryBufferWriter writer(serialBufferPtr);

    for (int pageRowIndex = 0; pageRowIndex < inBuffer->getHeightfieldBufferSet()->getPageRowCount();
         ++pageRowIndex)
    {
        for (int pageColumnIndex = 0;
             pageColumnIndex < inBuffer->getHeightfieldBufferSet()->getPageColumnCount(); ++pageColumnIndex)
        {
            HeightfieldBufferPage* page = inBuffer->getPage(pageColumnIndex, pageRowIndex,
                                                            HeightfieldBuffer::PAGEACCESSMODE_READONLY);
            Ogre::PixelBox pixelBox(inBuffer->getHeightfieldBufferSet()->getElementColumnCountPerPage(),
                                    inBuffer->getHeightfieldBufferSet()->getElementRowCountPerPage(), 1,
                                    pixelFormat, writer.getData());
            if (page->containsData())
            {
                page->copyTo(pixelBox);
            }
            else
            {
                memset(writer.getData(), 0, dataPageByteCount);
            }
            writer.advancePosition(dataPageByteCount);
        }
    }
    assert(writer.getPosition() == serialBufferPtr.get()->getSize());
    return serialBufferPtr;
}

bool HeightfieldSerializer::deserialize(Utils::SerialMemoryBufferConstPtr inSerial,
                                        HeightfieldBuffer* outBuffer, Ogre::PixelFormat pixelFormat,
                                        string* error)
{
    assert(outBuffer);
    // int elementCount = outBuffer->getElementColumnCount() * outBuffer->getElementRowCount();
    int elementPageCount = outBuffer->getHeightfieldBufferSet()->getElementColumnCountPerPage() *
                           outBuffer->getHeightfieldBufferSet()->getElementRowCountPerPage();
    int bytesPerElement = Ogre::PixelUtil::getNumElemBytes(pixelFormat);
    int dataPageByteCount = elementPageCount * bytesPerElement;
    // int dataByteCount = elementCount * bytesPerElement;
    // int headerByteCount = 0;

    assert(inSerial.get()->getSize() ==
           (size_t)(0 +
                    outBuffer->getHeightfieldBufferSet()->getElementColumnCount() *
                        outBuffer->getHeightfieldBufferSet()->getElementRowCount() * bytesPerElement));
    Utils::SerialMemoryBufferReader reader(inSerial);

    for (int pageRowIndex = 0; pageRowIndex < outBuffer->getHeightfieldBufferSet()->getPageRowCount();
         ++pageRowIndex)
    {
        for (int pageColumnIndex = 0;
             pageColumnIndex < outBuffer->getHeightfieldBufferSet()->getPageColumnCount();
             ++pageColumnIndex)
        {
            HeightfieldBufferPage* page = outBuffer->getPage(
                pageColumnIndex, pageRowIndex, HeightfieldBuffer::PAGEACCESSMODE_WRITE_DISCARD);
            Ogre::PixelBox pixelBox(outBuffer->getHeightfieldBufferSet()->getElementColumnCountPerPage(),
                                    outBuffer->getHeightfieldBufferSet()->getElementRowCountPerPage(), 1,
                                    pixelFormat, reader.getData());
            page->updateFrom(pixelBox);
            reader.advancePosition(dataPageByteCount);
        }
    }
    assert(reader.getPosition() ==
           (size_t)(0 +
                    outBuffer->getHeightfieldBufferSet()->getElementColumnCount() *
                        outBuffer->getHeightfieldBufferSet()->getElementRowCount() * bytesPerElement));
    return true;
}

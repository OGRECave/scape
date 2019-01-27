/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "Utils/ConverterFunctor.h"
#include "HeightfieldGeom.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldGeomTileBufferFactory.h"
#include "HeightfieldGeomTileVertexUVBufferManager.h"
#include "HeightfieldGeomTileIndexBufferManager.h"

using namespace ScapeEngine;

Ogre::HardwareVertexBufferSharedPtr
HeightfieldGeomTileBufferFactory::createVertexHeightBuffer(HeightfieldGeomTile* heightfieldGeomTile)
{
    HeightfieldGeom* heightfieldGeom = heightfieldGeomTile->getHeightfieldGeom();
    assert(heightfieldGeom);

    Ogre::VertexElementType elementBaseType =
        Ogre::VertexElement::getBaseType(heightfieldGeom->getVertexElementTypeHeight());
    Ogre::PixelFormat heightElementFormat =
        heightfieldGeom->getHeightfieldBuffer()->getHeightElementFormat();

    if (Ogre::PixelUtil::isFloatingPoint(heightElementFormat) && elementBaseType == Ogre::VET_FLOAT1)
    {
        return _createHeightBuffer<Ogre::Real, Ogre::Real>(heightfieldGeomTile);
    }
    else if (Ogre::PixelUtil::isFloatingPoint(heightElementFormat) && elementBaseType != Ogre::VET_FLOAT1)
    {
        return _createHeightBuffer<Ogre::Real, short>(heightfieldGeomTile);
    }
    else if (!Ogre::PixelUtil::isFloatingPoint(heightElementFormat) && elementBaseType == Ogre::VET_FLOAT1)
    {
        return _createHeightBuffer<Ogre::uint16, Ogre::Real>(heightfieldGeomTile);
    }
    else if (!Ogre::PixelUtil::isFloatingPoint(heightElementFormat) && elementBaseType != Ogre::VET_FLOAT1)
    {
        return _createHeightBuffer<Ogre::uint16, short>(heightfieldGeomTile);
    }

    assert(false && _T("Unsupported combination of formats"));
    return Ogre::HardwareVertexBufferSharedPtr();
}

Ogre::HardwareVertexBufferSharedPtr
HeightfieldGeomTileBufferFactory::createVertexUVBuffer(HeightfieldGeomTile* heightfieldGeomTile)
{
    HeightfieldGeom* heightfieldGeom = heightfieldGeomTile->getHeightfieldGeom();
    assert(heightfieldGeom);

    Ogre::Rect visibleQuadRect = heightfieldGeomTile->getVisibleQuadRect();
    size_t quadColumnSpacing = heightfieldGeomTile->getCurrentLODSpacing();
    size_t quadRowSpacing = heightfieldGeomTile->getCurrentLODSpacing();

    Ogre::HardwareVertexBufferSharedPtr vertexUVBuffer =
        getEngineCore()->getHeightfieldGeomTileVertexUVBufferManager()->getBuffer(
            heightfieldGeom->getVertexElementTypeUV(),
            Ogre::Rect(0, 0, visibleQuadRect.width(), visibleQuadRect.height()),
            heightfieldGeomTile->getTemplateQuadColumnCount(),
            heightfieldGeomTile->getTemplateQuadRowCount(), quadColumnSpacing, quadRowSpacing);

    return vertexUVBuffer;
}

Ogre::HardwareIndexBufferSharedPtr
HeightfieldGeomTileBufferFactory::createIndexBuffer(HeightfieldGeomTile* heightfieldGeomTile)
{
    const std::list<HeightfieldGeomTile*>& neighborTiles = heightfieldGeomTile->getNeighbors();

    Ogre::Rect visibleQuadRect = heightfieldGeomTile->getVisibleQuadRect();
    int currentLodSpacing = heightfieldGeomTile->getCurrentLODSpacing();
    int quadColumnSpacing = currentLodSpacing;
    int quadRowSpacing = currentLodSpacing;

    int spacedVisibleQuadColumnCount =
        (visibleQuadRect.width() + quadColumnSpacing - 1) / quadColumnSpacing;
    int spacedVisibleQuadRowCount = (visibleQuadRect.height() + quadRowSpacing - 1) / quadRowSpacing;

    int spacingNorth = 1;
    int spacingEast = 1;
    int spacingSouth = 1;
    int spacingWest = 1;

    std::list<HeightfieldGeomTile *>::const_iterator neighborTileIt,
        neighborTileItEnd = neighborTiles.end();
    for (neighborTileIt = neighborTiles.begin(); neighborTileIt != neighborTileItEnd; ++neighborTileIt)
    {
        Ogre::Rect neighborRect = (*neighborTileIt)->getVisibleQuadRect();
        if (visibleQuadRect.top == neighborRect.bottom)
            spacingNorth = (*neighborTileIt)->getCurrentLODSpacing();
        else if (visibleQuadRect.right == neighborRect.left)
            spacingEast = (*neighborTileIt)->getCurrentLODSpacing();
        else if (visibleQuadRect.bottom == neighborRect.top)
            spacingSouth = (*neighborTileIt)->getCurrentLODSpacing();
        else if (visibleQuadRect.left == neighborRect.right)
            spacingWest = (*neighborTileIt)->getCurrentLODSpacing();
        else
            assert(false);
    }

    Ogre::HardwareIndexBufferSharedPtr indexBuffer =
        getEngineCore()->getHeightfieldGeomTileIndexBufferManager()->getBuffer(
            spacedVisibleQuadColumnCount, spacedVisibleQuadRowCount,
            std::max(1, spacingNorth / currentLodSpacing), std::max(1, spacingEast / currentLodSpacing),
            std::max(1, spacingSouth / currentLodSpacing), std::max(1, spacingWest / currentLodSpacing));

    return indexBuffer;
}

template <class PageDataType, class VertexDataType>
Ogre::HardwareVertexBufferSharedPtr
HeightfieldGeomTileBufferFactory::_createHeightBuffer(HeightfieldGeomTile* heightfieldGeomTile)
{
    Utils::NormalizedScalarConverterFunctor<PageDataType, VertexDataType> heightConverterFunctor;

    HeightfieldGeom* heightfieldGeom = heightfieldGeomTile->getHeightfieldGeom();
    assert(heightfieldGeom);

    HeightfieldBuffer* heightfieldBuffer = heightfieldGeom->getHeightfieldBuffer();
    assert(heightfieldBuffer);

    Ogre::Rect visibleQuadRect = heightfieldGeomTile->getVisibleQuadRect();
    int quadColumnSpacing = heightfieldGeomTile->getCurrentLODSpacing();
    int quadRowSpacing = heightfieldGeomTile->getCurrentLODSpacing();

    int spacedVisibleQuadColumnCount =
        (visibleQuadRect.width() + quadColumnSpacing - 1) / quadColumnSpacing;
    int spacedVisibleQuadRowCount = (visibleQuadRect.height() + quadRowSpacing - 1) / quadRowSpacing;

    int vertexCount = (spacedVisibleQuadColumnCount + 1) * (spacedVisibleQuadRowCount + 1);

    Ogre::HardwareVertexBufferSharedPtr buffer =
        Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            Ogre::VertexElement::getTypeSize(heightfieldGeom->getVertexElementTypeHeight()), vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);

    assert(sizeof(VertexDataType) == Ogre::VertexElement::getTypeSize(Ogre::VertexElement::getBaseType(
                                         heightfieldGeom->getVertexElementTypeHeight())));
    assert(sizeof(PageDataType) *
               Ogre::PixelUtil::getComponentCount(heightfieldBuffer->getHeightElementFormat()) ==
           Ogre::PixelUtil::getNumElemBytes(heightfieldBuffer->getHeightElementFormat()));

    void* vertexBufferDataPtr = buffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
    assert(vertexBufferDataPtr);

    VertexDataType* vertexElementPtr = static_cast<VertexDataType*>(vertexBufferDataPtr);
    size_t vertexElementPtrPitch =
        Ogre::VertexElement::getTypeCount(heightfieldGeom->getVertexElementTypeHeight());

    Ogre::Rect availableDataRect(
        0, 0, heightfieldBuffer->getHeightfieldBufferSet()->getPageColumnCount() *
                  heightfieldBuffer->getHeightfieldBufferSet()->getElementColumnCountPerPage(),
        heightfieldBuffer->getHeightfieldBufferSet()->getPageRowCount() *
            heightfieldBuffer->getHeightfieldBufferSet()->getElementRowCountPerPage());

    typedef std::list<Ogre::PixelBox> PageElementRowBoxes;
    PageElementRowBoxes pageElementRowBoxes;

    Ogre::Rect requestedPixelBoxRect(std::max<long>(0, visibleQuadRect.left),
                                     std::max<long>(0, visibleQuadRect.top),
                                     std::min<long>(visibleQuadRect.right + 2, availableDataRect.right),
                                     std::min<long>(visibleQuadRect.bottom + 2, availableDataRect.bottom));

    Ogre::Rect pageElementRect;
    int tempProcessedRows = 0, tempProcessedColumns = 0;

    typedef std::list<std::list<Ogre::PixelBox>> PagePixelBoxRows;
    PagePixelBoxRows pagePixelBoxRows;

    Ogre::Rect remainingPixelBoxRect =
        Ogre::Rect(requestedPixelBoxRect.left, requestedPixelBoxRect.top, requestedPixelBoxRect.right,
                   requestedPixelBoxRect.bottom);

    while (remainingPixelBoxRect.top < remainingPixelBoxRect.bottom)
    {
        remainingPixelBoxRect.left = requestedPixelBoxRect.left;

        pagePixelBoxRows.push_back(std::list<Ogre::PixelBox>());
        std::list<Ogre::PixelBox>& pagePixelBoxesRow = *pagePixelBoxRows.rbegin();
        Ogre::PixelBox pixelBox;
        pixelBox.data = NULL;

        while (remainingPixelBoxRect.left < remainingPixelBoxRect.right)
        {
            HeightfieldBufferPage* page = heightfieldBuffer->getPage(
                heightfieldBuffer->getPageColumnFromElement(remainingPixelBoxRect.left),
                heightfieldBuffer->getPageRowFromElement(remainingPixelBoxRect.top),
                HeightfieldBuffer::PAGEACCESSMODE_READONLY);
            assert(page);
            pixelBox = page->getRawHeightData();
            assert(pixelBox.data);
            Ogre::Rect absoluteRect = page->getAbsoluteRect();
            Utils::translateBox(pixelBox, absoluteRect.left, absoluteRect.top);
            Ogre::Rect pixelBoxRect =
                Utils::intersectTRect(Utils::boxToRect(pixelBox), remainingPixelBoxRect);
            pixelBox = Utils::clipPixelBox(pixelBox, pixelBoxRect, pixelBoxRect.left, pixelBoxRect.top);
            pagePixelBoxesRow.push_back(pixelBox);
            remainingPixelBoxRect.left = pixelBox.right;
        }

        remainingPixelBoxRect.top = pixelBox.bottom;
    }

    PagePixelBoxRows::const_iterator pagePixelBoxRowIt = pagePixelBoxRows.begin();
    PagePixelBoxRows::const_iterator pagePixelBoxRowDYIt = pagePixelBoxRows.begin();

    int rowEnd = visibleQuadRect.bottom + quadRowSpacing;

    int pageElementSize = Ogre::PixelUtil::getComponentCount(pagePixelBoxRows.begin()->begin()->format);
    int pageElementSpacing = pageElementSize * quadColumnSpacing;

    for (int row = visibleQuadRect.top; row < rowEnd; row += quadRowSpacing)
    {
        row = std::min<int>(row, visibleQuadRect.bottom);
        int rowDY = std::min<int>(row + 1, requestedPixelBoxRect.bottom - 1);

        while (row >= (int)pagePixelBoxRowIt->begin()->bottom)
            pagePixelBoxRowIt++;
        while (rowDY >= (int)pagePixelBoxRowDYIt->begin()->bottom)
            pagePixelBoxRowDYIt++;

        std::list<Ogre::PixelBox>::const_iterator pixelBoxColumnIt = pagePixelBoxRowIt->begin();
        std::list<Ogre::PixelBox>::const_iterator pixelBoxColumnItEnd = pagePixelBoxRowIt->end();
        std::list<Ogre::PixelBox>::const_iterator pixelBoxDXColumnIt = pagePixelBoxRowIt->begin();
        std::list<Ogre::PixelBox>::const_iterator pixelBoxDXColumnItEnd = pagePixelBoxRowIt->end();
        std::list<Ogre::PixelBox>::const_iterator pixelBoxDYColumnIt = pagePixelBoxRowDYIt->begin();
        std::list<Ogre::PixelBox>::const_iterator pixelBoxDYColumnItEnd = pagePixelBoxRowDYIt->end();

        int column = requestedPixelBoxRect.left;
        int columnEnd = visibleQuadRect.right + quadColumnSpacing;

        while (column < columnEnd)
        {
            column = std::min<int>(column, visibleQuadRect.right);
            while (column >= (int)(*pixelBoxColumnIt).right)
            {
                pixelBoxColumnIt++;
                pixelBoxDYColumnIt++;
            }

            int columnDX = std::min<int>(column + 1, requestedPixelBoxRect.right - 1);
            while (columnDX >= (int)(*pixelBoxDXColumnIt).right)
            {
                pixelBoxDXColumnIt++;
            }

            const Ogre::PixelBox& pixelBox = *pixelBoxColumnIt;
            const Ogre::PixelBox& pixelBoxDX = *pixelBoxDXColumnIt;
            const Ogre::PixelBox& pixelBoxDY = *pixelBoxDYColumnIt;

            const PageDataType* pixelRowData =
                (const PageDataType*)pixelBox.data +
                pageElementSize * (pixelBox.rowPitch * (row - pixelBox.top) - pixelBox.left);
            const PageDataType* pixelRowDXData =
                (const PageDataType*)pixelBoxDX.data +
                pageElementSize * (pixelBoxDX.rowPitch * (row - pixelBoxDX.top) - pixelBoxDX.left);
            const PageDataType* pixelRowDYData =
                (const PageDataType*)pixelBoxDY.data +
                pageElementSize * (pixelBoxDY.rowPitch * (rowDY - pixelBoxDY.top) - pixelBoxDY.left);

            int columnPageIndex = column * pageElementSize;
            int columnDXPageIndex = columnDX * pageElementSize;

            int columnInnerEnd = std::min<int>(visibleQuadRect.right,
                                               std::min<int>(pixelBox.right - 1, availableDataRect.right));

            do
            {
                VertexDataType height = heightConverterFunctor((PageDataType)pixelRowData[columnPageIndex]);
                VertexDataType gradientX =
                    heightConverterFunctor((PageDataType)pixelRowDXData[columnDXPageIndex]) - height;
                VertexDataType gradientY =
                    heightConverterFunctor((PageDataType)pixelRowDYData[columnPageIndex]) - height;

                vertexElementPtr[0] = height;
                vertexElementPtr[1] = gradientX;
                ;
                vertexElementPtr[2] = gradientY;
                vertexElementPtr += vertexElementPtrPitch;

                ++tempProcessedColumns;

                columnPageIndex += pageElementSpacing;
                columnDXPageIndex += pageElementSpacing;
                column += quadColumnSpacing;
            } while (column < columnInnerEnd);
        }

        assert((int)tempProcessedColumns == spacedVisibleQuadColumnCount + 1);
        tempProcessedRows++;
        tempProcessedColumns = 0;
    }
    assert((int)tempProcessedRows == spacedVisibleQuadRowCount + 1);

    buffer->unlock();

    return buffer;
}

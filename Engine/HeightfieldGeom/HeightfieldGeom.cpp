/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
#include "HeightfieldGeom.h"
#include "HeightfieldGeomTile.h"
#include "Utils/ShaderCustomAutoConstants.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldGeomMaterialSplat.h"
#include "HeightfieldBrush/HeightfieldBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"

#include "Input/InputManager.h" //TODO GdCarpentier 10/29/2007 : remove

std::pair<bool, Ogre::Real> intersects(const Ogre::Ray& ray, const Ogre::Vector3& a, const Ogre::Vector3& b,
                                       const Ogre::Vector3& c, bool positiveSide = true,
                                       bool negativeSide = true)
{
    //
    // Calculate intersection with plane.
    //

    Ogre::Vector3 normal = Ogre::Math::calculateBasicFaceNormalWithoutNormalize(a, b, c);

    Ogre::Real t;
    {
        Ogre::Real denom = normal.dotProduct(ray.getDirection());

        // Check intersect side
        if (denom > +std::numeric_limits<Ogre::Real>::epsilon())
        {
            if (!negativeSide)
                return std::pair<bool, Ogre::Real>(false, 0);
        }
        else if (denom < -std::numeric_limits<Ogre::Real>::epsilon())
        {
            if (!positiveSide)
                return std::pair<bool, Ogre::Real>(false, 0);
        }
        else
        {
            // Parallel or triangle area is close to zero when
            // the plane normal not normalised.
            return std::pair<bool, Ogre::Real>(false, 0);
        }

        t = normal.dotProduct(a - ray.getOrigin()) / denom;

        if (t < 0)
        {
            // Intersection is behind origin
            return std::pair<bool, Ogre::Real>(false, 0);
        }
    }

    //
    // Calculate the largest area projection plane in X, Y or Z.
    //
    size_t i0, i1;
    {
        Ogre::Real n0 = Ogre::Math::Abs(normal[0]);
        Ogre::Real n1 = Ogre::Math::Abs(normal[1]);
        Ogre::Real n2 = Ogre::Math::Abs(normal[2]);

        i0 = 1;
        i1 = 2;
        if (n1 > n2)
        {
            if (n1 > n0)
                i0 = 0;
        }
        else
        {
            if (n2 > n0)
                i1 = 0;
        }
    }

    //
    // Check the intersection point is inside the triangle.
    //
    {
        Ogre::Real u1 = b[i0] - a[i0];
        Ogre::Real v1 = b[i1] - a[i1];
        Ogre::Real u2 = c[i0] - a[i0];
        Ogre::Real v2 = c[i1] - a[i1];
        Ogre::Real u0 = t * ray.getDirection()[i0] + ray.getOrigin()[i0] - a[i0];
        Ogre::Real v0 = t * ray.getDirection()[i1] + ray.getOrigin()[i1] - a[i1];

        Ogre::Real alpha = u0 * v2 - u2 * v0;
        Ogre::Real beta = u1 * v0 - u0 * v1;
        Ogre::Real area = u1 * v2 - u2 * v1;

        // epsilon to avoid float precision error
        // const Ogre::Real EPSILON = 1.0e-3;//1.0f;//1.0f;//1e-1f;

        // Ogre::Real tolerance = - EPSILON * area;
        Ogre::Real tolerance = -1E0f * (area > 0 ? 1.0f : -1.0f);

        if (area > 0)
        {
            if (alpha < tolerance || beta < tolerance || alpha + beta > area - tolerance)
                return std::pair<bool, Ogre::Real>(false, 0);
        }
        else
        {
            if (alpha > tolerance || beta > tolerance || alpha + beta < area - tolerance)
                return std::pair<bool, Ogre::Real>(false, 0);
        }
    }

    return std::pair<bool, Ogre::Real>(true, t);
}

// ----------------------------------------------------------------------------
namespace ScapeEngine
{
namespace HeightfieldGeomNS
{
/**
 * Sortable 'key' distance to tile pointer
 */
struct TileDistance
{
    int idealSpacing;
    HeightfieldGeomTile* tile;

    inline bool operator==(const TileDistance& rhs) const { return idealSpacing == rhs.idealSpacing; }

    inline bool operator<(const TileDistance& rhs) const { return idealSpacing < rhs.idealSpacing; }
};
}
}

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldGeom::HeightfieldGeom(class HeightfieldBuffer* heightfieldBuffer,
                                 Ogre::SceneNode* parentSceneNode,
                                 Ogre::VertexElementType vertexElementTypeUV,
                                 Ogre::VertexElementType vertexElementTypeHeight)
    : mHeightfieldBuffer(heightfieldBuffer), mParentSceneNode(parentSceneNode),
      mVertexElementTypeUV(vertexElementTypeUV), mVertexElementTypeHeight(vertexElementTypeHeight),
      mGeomAutoConstantsDirty(true), mMillisLastRevalidate(0)
{

    mHeightfieldBuffer->registerHeightfieldGeom(this);

    mTopLeftPosition = Ogre::Vector3(0, 0, 0);
    mWorldQuadSize = 1.0f;

    mGeomShaderCustomAutoConstants = new ShaderCustomAutoConstants();
    mHeightfieldGeomMaterial = new HeightfieldGeomMaterialSplat();
    updateMaterial();

    int elementColumns = getHeightfieldBuffer()->getHeightfieldBufferSet()->getElementColumnCount();
    int elementRows = getHeightfieldBuffer()->getHeightfieldBufferSet()->getElementRowCount();
    Ogre::Rect heightfieldQuadRect(0, 0, elementColumns - 1, elementRows - 1);

    int tileSizeY = 192; // 159;
    int tileSizeX = 192; // 159;

    for (int y = 0; y < elementRows; y += tileSizeY)
    {
        bool addedRow = false;

        // tileSizeY += 3;
        // tileSizeX = 1;
        for (int x = 0; x < elementColumns; x += tileSizeX)
        {
            // tileSizeX += 3;
            Ogre::Rect visibleRect =
                Utils::intersectTRect(Ogre::Rect(x, y, x + tileSizeX, y + tileSizeY), heightfieldQuadRect);

            if (visibleRect.width() > 0 && visibleRect.height() > 0)
            {
                if (!addedRow)
                {
                    mTiles.push_back(TileRow());
                    addedRow = true;
                }

                mTiles.back().push_back(createTile(visibleRect, tileSizeX, tileSizeY));
            }
        }
    }

    for (size_t pageRowIndex = 0; pageRowIndex < mTiles.size(); ++pageRowIndex)
    {
        for (size_t pageColumnIndex = 0; pageColumnIndex < mTiles[0].size(); ++pageColumnIndex)
        {
            HeightfieldGeomTile* tile = mTiles[pageRowIndex][pageColumnIndex];

            if (pageColumnIndex > 0)
            {
                mTiles[pageRowIndex][pageColumnIndex - 1]->addNeighbor(tile);
            }
            if (pageRowIndex > 0)
            {
                mTiles[pageRowIndex - 1][pageColumnIndex]->addNeighbor(tile);
            }
            if (pageColumnIndex < mTiles[0].size() - 1)
            {
                mTiles[pageRowIndex][pageColumnIndex + 1]->addNeighbor(tile);
            }
            if (pageRowIndex < mTiles.size() - 1)
            {
                mTiles[pageRowIndex + 1][pageColumnIndex]->addNeighbor(tile);
            }
        }
    }
}

// ----------------------------------------------------------------------------
HeightfieldGeom::~HeightfieldGeom()
{
    getHeightfieldBuffer()->unregisterHeightfieldGeom(this);

    Tiles::iterator tileRowIt, tileRowItEnd = mTiles.end();
    for (tileRowIt = mTiles.begin(); tileRowIt != tileRowItEnd; ++tileRowIt)
    {
        TileRow& tileRow = *tileRowIt;
        TileRow::iterator tileIt, tileItEnd = tileRow.end();
        for (tileIt = tileRow.begin(); tileIt != tileItEnd; ++tileIt)
        {
            delete *tileIt;
        }
    }

    delete mGeomShaderCustomAutoConstants;
    delete mHeightfieldGeomMaterial;
}

// ----------------------------------------------------------------------------
HeightfieldGeomTile* HeightfieldGeom::createTile(const Ogre::Rect& visibleQuadRect,
                                                 size_t bufferedQuadColumnCount,
                                                 size_t bufferedQuadRowCount)
{
    HeightfieldGeomTile* tile =
        new HeightfieldGeomTile(this, visibleQuadRect, bufferedQuadColumnCount, bufferedQuadRowCount);
    Ogre::AxisAlignedBox tileBox = tile->getBoundingBox();
    Ogre::Vector3 tileTopLeftPosition =
        mTopLeftPosition +
        Ogre::Vector3((Ogre::Real)visibleQuadRect.left, 0, (Ogre::Real)visibleQuadRect.top) *
            getWorldQuadSize();

    Ogre::SceneNode* node = mParentSceneNode->createChildSceneNode();
    node->attachObject(tile);
    node->setPosition(tileTopLeftPosition - tileBox.getMinimum());

    return tile;
}

// ----------------------------------------------------------------------------
void HeightfieldGeom::tick()
{
    mGeomAutoConstantsDirty = true;

    typedef std::multiset<HeightfieldGeomNS::TileDistance> TileDistanceSet;
    TileDistanceSet tileDistanceSet;

    Tiles::iterator tileRowIt, tileRowItEnd = mTiles.end();
    for (tileRowIt = mTiles.begin(); tileRowIt != tileRowItEnd; ++tileRowIt)
    {
        TileRow& tileRow = *tileRowIt;
        TileRow::iterator tileIt, tileItEnd = tileRow.end();
        for (tileIt = tileRow.begin(); tileIt != tileItEnd; ++tileIt)
        {
            int spacing = (*tileIt)->needsLODUpdate();
            if (spacing)
            {
                HeightfieldGeomNS::TileDistance tileDistance = {spacing, *tileIt};
                tileDistanceSet.insert(tileDistance);
            }
        }
    }

    int remainingBufferLODUpdates = std::max(1, (int)(0.1f * tileDistanceSet.size()));
    TileDistanceSet::iterator tileDistanceIt, tileDistanceItEnd = tileDistanceSet.end();
    for (tileDistanceIt = tileDistanceSet.begin();
         tileDistanceIt != tileDistanceItEnd && remainingBufferLODUpdates > 0; ++tileDistanceIt)
    {
        // Ogre::Rect visibleQuadRect = tileDistanceIt->tile->getVisibleQuadRect();
        // printf("Update LOD tile (%d,%d,%d,%d) : %d\n", visibleQuadRect.left, visibleQuadRect.top,
        // visibleQuadRect.right, visibleQuadRect.bottom, tileDistanceIt->idealSpacing);

        tileDistanceIt->tile->updateLOD();
        --remainingBufferLODUpdates;
    }

    if (getEngineCore()->getFrameMilliseconds() > mMillisLastRevalidate + 10)
    {
        for (tileRowIt = mTiles.begin(); tileRowIt != tileRowItEnd; ++tileRowIt)
        {
            TileRow& tileRow = *tileRowIt;
            TileRow::iterator tileIt, tileItEnd = tileRow.end();
            for (tileIt = tileRow.begin(); tileIt != tileItEnd; ++tileIt)
            {
                (*tileIt)->postFrameUpdate();
            }
        }
        mMillisLastRevalidate = getEngineCore()->getFrameMilliseconds();
    }

    mHeightfieldGeomMaterial->bindBrush(getEngineCore()->getHeightfieldBrushManager()->getCurrentBrush());

    // if (getEngineCore()->getInputManager()->getButton(ButtonId::MATERIAL_RESET)->isPressed())
    if (mHeightfieldGeomMaterial->hasDirtySettings())
    {
        updateMaterial();
        mHeightfieldGeomMaterial->clearDirtySettings();
    }
}

// ----------------------------------------------------------------------------
bool HeightfieldGeom::getTile(int quadColumnIndex, int quadRowIndex, IVector2& pageIndex)
{
    pageIndex = IVector2(0, 0);

    int pageRowIndex = 0, pageColumnIndex = 0;

    Tiles::iterator tileRowIt, tileRowItEnd = mTiles.end();
    for (tileRowIt = mTiles.begin(); tileRowIt != tileRowItEnd; ++tileRowIt, ++pageRowIndex)
    {
        TileRow& tileRow = *tileRowIt;
        if (tileRow.size() > 0)
        {
            Ogre::Rect tileRect = (*tileRow.begin())->getVisibleQuadRect();
            if (quadRowIndex >= tileRect.top && quadRowIndex <= tileRect.bottom)
            {
                TileRow::iterator tileIt, tileItEnd = tileRow.end();
                for (tileIt = tileRow.begin(); tileIt != tileItEnd; ++tileIt, ++pageColumnIndex)
                {
                    Ogre::Rect tileRect = (*tileIt)->getVisibleQuadRect();
                    if (quadColumnIndex >= tileRect.left && quadColumnIndex <= tileRect.right)
                    {
                        pageIndex = IVector2(pageColumnIndex, pageRowIndex);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// ----------------------------------------------------------------------------
void HeightfieldGeom::updateMaterial()
{
    mHeightfieldGeomMaterial->updateTextures();

    Tiles::iterator tileRowIt, tileRowItEnd = mTiles.end();
    for (tileRowIt = mTiles.begin(); tileRowIt != tileRowItEnd; ++tileRowIt)
    {
        TileRow& tileRow = *tileRowIt;
        TileRow::iterator tileIt, tileItEnd = tileRow.end();
        for (tileIt = tileRow.begin(); tileIt != tileItEnd; ++tileIt)
        {
            (*tileIt)->markDirtyMaterial();
        }
    }

    mHeightfieldGeomMaterial->updateShaderConstantsGeom(this, mGeomShaderCustomAutoConstants, true);
}

// ----------------------------------------------------------------------------
Ogre::Real HeightfieldGeom::getIntersectionPoint(const Ogre::Ray& worldRay)
{
    Ogre::Real intersectionParam = -1.0f;

    //	Ogre::Vector3 localRayDirection = (mParentSceneNode->_getDerivedOrientation().Inverse() *
    //worldRay.getOrigin()).normalisedCopy();
    //	Ogre::Vector3 localRayOrigin = worldRay.getOrigin() - mParentSceneNode->_getDerivedPosition();
    Ogre::Vector3 localRayDirection = worldRay.getDirection() / mWorldQuadSize;
    Ogre::Vector3 localRayOrigin = (worldRay.getOrigin() - mTopLeftPosition) / mWorldQuadSize;

    if (abs(localRayDirection.x) < 1E-15)
        localRayDirection.x = 1E-15;
    if (abs(localRayDirection.z) < 1E-15)
        localRayDirection.z = 1E-15;

    const unsigned int elementColumnCount =
        getHeightfieldBuffer()->getHeightfieldBufferSet()->getElementColumnCountPerPage() *
        getHeightfieldBuffer()->getHeightfieldBufferSet()->getPageColumnCount();
    const unsigned int elementRowCount =
        getHeightfieldBuffer()->getHeightfieldBufferSet()->getElementRowCountPerPage() *
        getHeightfieldBuffer()->getHeightfieldBufferSet()->getPageRowCount();

    Ogre::AxisAlignedBox aabb;
    aabb.setMinimum(
        Ogre::Vector3((Ogre::Real)0.0f,
                      (Ogre::Real)getHeightfieldBuffer()->getHeightfieldBufferSet()->getHeightRangeMin(),
                      (Ogre::Real)0.0f));

    aabb.setMaximum(
        Ogre::Vector3((Ogre::Real)elementColumnCount,
                      (Ogre::Real)getHeightfieldBuffer()->getHeightfieldBufferSet()->getHeightRangeMax(),
                      (Ogre::Real)elementRowCount));

    Ogre::Ray localRay(localRayOrigin, localRayDirection);
    Ogre::Real paramStart, paramEnd;
    if (Ogre::Math::intersects(localRay, aabb, &paramStart, &paramEnd) && paramEnd > paramStart)
    {
        localRayOrigin = localRayOrigin + localRayDirection * paramStart;
        paramEnd -= paramStart;

        int incrColumn = localRayDirection.x > 0 ? 1 : -1;
        int incrRow = localRayDirection.z > 0 ? 1 : -1;
        unsigned int currentColumn =
            Utils::clamp((int)floor(localRayOrigin.x), 0, (int)elementColumnCount - 2);
        unsigned int currentRow = Utils::clamp((int)floor(localRayOrigin.z), 0, (int)elementRowCount - 2);
        Ogre::Real incrParamX = (Ogre::Real)incrColumn / localRayDirection.x;
        Ogre::Real incrParamZ = (Ogre::Real)incrRow / localRayDirection.z;
        Ogre::Real gridParamNextX = (currentColumn - localRayOrigin.x) / localRayDirection.x;
        Ogre::Real gridParamNextZ = (currentRow - localRayOrigin.z) / localRayDirection.z;
        if (gridParamNextX <= 0)
            gridParamNextX += incrParamX;
        if (gridParamNextZ <= 0)
            gridParamNextZ += incrParamZ;
        Ogre::Real gridParam = 0.0f;
        while (gridParam <= paramEnd && intersectionParam < 0.0f)
        {
            Ogre::Real intersectionParam = getIntersectionParam(localRay, currentColumn, currentRow);
            if (intersectionParam >= 0.0f)
            {
                return intersectionParam;
            }

            if (gridParamNextX < gridParamNextZ)
            {
                gridParam = gridParamNextX;
                gridParamNextX += incrParamX;
                currentColumn += incrColumn;
                if (currentColumn >= elementColumnCount - 1)
                    break;
            }
            else
            {
                gridParam = gridParamNextZ;
                gridParamNextZ += incrParamZ;
                currentRow += incrRow;
                if (currentRow >= elementRowCount - 1)
                    break;
            }
        }
        // printf("\n");
    }

    return intersectionParam;
}

Ogre::Real HeightfieldGeom::getIntersectionParam(const Ogre::Ray& localRay, size_t column, size_t row)
{

    Ogre::Vector3 vector((Ogre::Real)column, getHeightAt(column, row), (Ogre::Real)row);
    Ogre::Vector3 vectorC((Ogre::Real)column + 1.0f, getHeightAt(column + 1, row), (Ogre::Real)row);
    Ogre::Vector3 vectorR((Ogre::Real)column, getHeightAt(column, row + 1), (Ogre::Real)row + 1.0f);
    Ogre::Vector3 vectorCR((Ogre::Real)column + 1.0f, getHeightAt(column + 1, row), (Ogre::Real)row + 1.0f);

    std::pair<bool, Ogre::Real> result;
    result = intersects(localRay, vectorC, vector, vectorR, true, false);
    // printf("%d %d ", column, row);
    if (!result.first)
    {
        result = intersects(localRay, vectorR, vectorCR, vectorC, true, false);
    }
    if (result.first)
    {
        Ogre::Vector3 point = localRay.getPoint(result.second);
        // printf("(%f %f %f) %d %d\n", point.x, point.y, point.z, column, row);
        return result.second;
    }
    return -1.0f;
}

// ----------------------------------------------------------------------------
Ogre::Real HeightfieldGeom::getHeightAt(size_t column, size_t row)
{
    const unsigned int elementColumnCountPerPage =
        getHeightfieldBuffer()->getHeightfieldBufferSet()->getElementColumnCountPerPage();
    const unsigned int elementRowCountPerPage =
        getHeightfieldBuffer()->getHeightfieldBufferSet()->getElementRowCountPerPage();

    size_t pageColumn = column / elementColumnCountPerPage;
    size_t pageRow = row / elementRowCountPerPage;
    HeightfieldBufferPage* page =
        getHeightfieldBuffer()->getPage(pageColumn, pageRow, HeightfieldBuffer::PAGEACCESSMODE_READONLY);
    Ogre::Real height = 0.0f;
    if (page)
    {
        size_t localColumn = column - pageColumn * elementColumnCountPerPage;
        size_t localRow = row - pageRow * elementRowCountPerPage;

        const Ogre::PixelBox& pixelBox = page->getRawHeightData();
        int elementIndex = localRow * pixelBox.rowPitch + localColumn;
        int bytesPerElement = Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
        void* elementPtr = (Ogre::uint8*)pixelBox.data + elementIndex * bytesPerElement;
        Ogre::ColourValue value;
        Ogre::PixelUtil::unpackColour(&value, pixelBox.format, elementPtr);
        Ogre::Real scale = getHeightfieldBuffer()->getHeightfieldBufferSet()->getHeightRangeMax() -
                           getHeightfieldBuffer()->getHeightfieldBufferSet()->getHeightRangeMin();
        Ogre::Real bias = getHeightfieldBuffer()->getHeightfieldBufferSet()->getHeightRangeMin();
        height = value.r * scale + bias;
    }
    return height;
}

// ----------------------------------------------------------------------------
void HeightfieldGeom::markDirtyRect(const Ogre::Rect& dirtyRect)
{
    // add extra border pixel to notify neighboring tiles when a normal at the dirtyrect's border
    // potentially changed
    // assumes normal is calculated only based on east and south element, not the north and west element
    // one for neighbor's last quad, one for neighbor's last quad normal calculation
    Ogre::Rect borderRect(dirtyRect.left - 2, dirtyRect.top - 2, dirtyRect.right, dirtyRect.bottom);

    for (size_t pageRowIndex = 0; pageRowIndex < mTiles.size(); ++pageRowIndex)
    {
        for (size_t pageColumnIndex = 0; pageColumnIndex < mTiles[0].size(); ++pageColumnIndex)
        {
            HeightfieldGeomTile* tile = mTiles[pageRowIndex][pageColumnIndex];
            Ogre::Rect visibleRect = Utils::intersectTRect(borderRect, tile->getVisibleQuadRect());
            if (visibleRect.width() > 0 && visibleRect.height() > 0)
            {
                tile->markDirtyData();
            }
        }
    }
}

// ----------------------------------------------------------------------------
Ogre::Vector3 HeightfieldGeom::getLocalPosition(const Ogre::Vector3& worldPosition) const
{
    Ogre::Vector3 localPosition = worldPosition / getWorldQuadSize();
    Ogre::Quaternion quat = getParentSceneNode()->_getDerivedOrientation();
    quat.Inverse();

    localPosition -= getParentSceneNode()->_getDerivedPosition();
    localPosition = localPosition / getParentSceneNode()->_getDerivedScale();
    localPosition = quat * localPosition;
    return localPosition;
}

// ----------------------------------------------------------------------------
const class ShaderCustomAutoConstants* HeightfieldGeom::getGeomShaderCustomAutoConstants()
{
    if (mGeomAutoConstantsDirty)
    {
        mHeightfieldGeomMaterial->updateShaderConstantsGeom(this, mGeomShaderCustomAutoConstants, false);
        mGeomAutoConstantsDirty = false;
    }
    return mGeomShaderCustomAutoConstants;
}

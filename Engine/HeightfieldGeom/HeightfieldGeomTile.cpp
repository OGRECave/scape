/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "Utils/ConverterFunctor.h"
#include "Utils/ShaderCustomAutoConstants.h"
#include "HeightfieldGeom.h"
#include "HeightfieldGeomMaterial.h"
#include "HeightfieldGeomTile.h"
#include "HeightfieldGeomTileBufferFactory.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
namespace ScapeEngine
{
	namespace _HeightfieldGeomTileShaderConstant
	{
		enum EShaderConstant
		{
			UNITQUADSCALE,
			UNITQUADBIAS,
			LOCALPOSSCALE,
			LOCALPOSBIAS,
			GRADIENTSCALEBIAS,
			LIGHTDIRECTION,
			SUNCOLOR,
			SKYCOLOR,
			ISOSCALES,
			ISODOTSCALES,
			UVSCALEGLOBAL,
			UVSCALELOCAL,
			UVSCALELOCALDISTORT,
			LAYERHEIGHTSCALE,
			LAYERHEIGHTBIAS,
			LAYERHEIGHTDISTORT,
			INTENSITYSCALEGLOBAL,
			FOGCOLOR,
			SHADERCONSTANT_COUNT
		};
	}
}

const string HeightfieldGeomTile::mMovableType = _T("HeightfieldGeomTile");

// ----------------------------------------------------------------------------
HeightfieldGeomTile::HeightfieldGeomTile(
	HeightfieldGeom* heightfieldGeom, 
	const Ogre::Rect& visibleQuadRect, 
	size_t templateQuadColumnCount, 
	size_t templateQuadRowCount, 
	const string& name)
:	Ogre::SimpleRenderable(),
	mDirtyData(false),
	mDirtyDataNeighbor(false),
	mDirtyMaterial(false),
	//mMaterial(),
	//mMaterialName(_T("HeightfieldMaterial")),
	mBoundingRadius(0),
	mHeightfieldGeom(heightfieldGeom),
	mVisibleQuadRect(visibleQuadRect),
	mTemplateQuadColumnCount(templateQuadColumnCount),
	mTemplateQuadRowCount(templateQuadRowCount),
	mFrameLeastSquaredPixelsWorldQuad(-1.0f),
	mQuadColumnSpacing(1024),
	mQuadRowSpacing(1024)
{
	setCastShadows(false);
	mMaterial.setNull(); //GDC ported

	mRenderOp.vertexData = new Ogre::VertexData();
	mRenderOp.indexData = new Ogre::IndexData();

	mGeomTileShaderCustomAutoConstants = new ShaderCustomAutoConstants();

	getEngineCore()->getSceneManager()->injectMovableObject(this);

	//TODO GdCarpentier 11/16/2007 : make this update unnecessary
	markDirtyData();
	markDirtyMaterial();
	revalidate();
}


// ----------------------------------------------------------------------------
HeightfieldGeomTile::~HeightfieldGeomTile()
{
	delete mRenderOp.vertexData;
	delete mRenderOp.indexData;
	delete mGeomTileShaderCustomAutoConstants;
}



// ----------------------------------------------------------------------------
const Ogre::MaterialPtr& HeightfieldGeomTile::getMaterial() const
{
	return getHeightfieldGeom()->getHeightfieldGeomMaterial()->getMaterial();
}



// ----------------------------------------------------------------------------
Ogre::Real HeightfieldGeomTile::getSquaredViewDepth(const Ogre::Camera* cam) const
{
	assert(mParentNode);
	return mParentNode->getSquaredViewDepth(cam);
}


//-----------------------------------------------------------------------
void HeightfieldGeomTile::_updateCustomGpuParameter(
	const Ogre::GpuProgramParameters::AutoConstantEntry& constantEntry, 
	Ogre::GpuProgramParameters* params) const
{
    if (!mGeomTileShaderCustomAutoConstants->updateShaderParameter(params, constantEntry))
    {
		if (!getHeightfieldGeom()->getGeomShaderCustomAutoConstants()->updateShaderParameter(params, constantEntry))
		{
			Renderable::_updateCustomGpuParameter(constantEntry, params);
		}
    }
}


// ----------------------------------------------------------------------------
void HeightfieldGeomTile::addNeighbor(HeightfieldGeomTile* neighborTile) 
{ 
	mNeighborTiles.push_back(neighborTile);
}


// ----------------------------------------------------------------------------
void HeightfieldGeomTile::revalidate()
{
	if (isDirtyData())
	{
		assert(mVisibleQuadRect.width() > 0 && mVisibleQuadRect.height() > 0);

		Ogre::Real heightRange = getHeightfieldGeom()->getHeightfieldBuffer()->getHeightfieldBufferSet()->getHeightRangeMax() - getHeightfieldGeom()->getHeightfieldBuffer()->getHeightfieldBufferSet()->getHeightRangeMin();
		Ogre::Vector3 aabbHalfSize = 0.5f * Ogre::Vector3((Ogre::Real)mVisibleQuadRect.width(), heightRange, (Ogre::Real)mVisibleQuadRect.height()) * getHeightfieldGeom()->getWorldQuadSize();
		mBox.merge(-aabbHalfSize);
		mBox.merge(aabbHalfSize);
		mBoundingRadius = aabbHalfSize.length();
	}

	updateRenderOperation();

	mDirtyData = false;
	mDirtyDataNeighbor = false;
	mDirtyMaterial = false;
}


// ----------------------------------------------------------------------------
void HeightfieldGeomTile::updateRenderOperation()
{
	if (isDirtyData())
	{
		mRenderOp.srcRenderable = this;
		mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP;
		Ogre::VertexDeclaration* vertexDeclaration = mRenderOp.vertexData->vertexDeclaration;
		vertexDeclaration->removeAllElements();

		// setup and bind height vertex buffer
		Ogre::HardwareVertexBufferSharedPtr vertexPositionBuffer = HeightfieldGeomTileBufferFactory::createVertexHeightBuffer(this);
		const Ogre::VertexElement& vertexHeightElement = vertexDeclaration->addElement(0, 0, getHeightfieldGeom()->getVertexElementTypeHeight(), Ogre::VES_POSITION);
		mRenderOp.vertexData->vertexBufferBinding->setBinding(vertexHeightElement.getSource(), vertexPositionBuffer);

		// setup and bind uv (xz) vertex buffer stream
		Ogre::HardwareVertexBufferSharedPtr vertexUVBuffer = HeightfieldGeomTileBufferFactory::createVertexUVBuffer(this);
		const Ogre::VertexElement& vertexUVElement = vertexDeclaration->addElement(1, 0, getHeightfieldGeom()->getVertexElementTypeUV(), Ogre::VES_TEXTURE_COORDINATES);
		mRenderOp.vertexData->vertexCount = vertexUVBuffer->getNumVertices();
		mRenderOp.vertexData->vertexStart = 0;
		mRenderOp.vertexData->vertexBufferBinding->setBinding(vertexUVElement.getSource(), vertexUVBuffer);

	}

	// bind up-to-date shaderprogram settings 
	getHeightfieldGeom()->getHeightfieldGeomMaterial()->updateShaderConstantsGeomTile(this, mGeomTileShaderCustomAutoConstants, isDirtyMaterial());

	// setup and bind position (height) vertex buffer stream
	updateRenderIndexBuffer();
}


// ----------------------------------------------------------------------------
void HeightfieldGeomTile::updateRenderIndexBuffer()
{
	if (isDirtyData() || isDirtyDataNeighbor())
	{

		mRenderOp.useIndexes = true;
		mRenderOp.indexData->indexBuffer = HeightfieldGeomTileBufferFactory::createIndexBuffer(this);
		mRenderOp.indexData->indexCount = mRenderOp.indexData->indexBuffer->getNumIndexes();
		mRenderOp.indexData->indexStart = 0;
	}
}




// ----------------------------------------------------------------------------
void HeightfieldGeomTile::_notifyCurrentCamera(Ogre::Camera* cam)
{
	Ogre::Vector3 closestBoundingBoxPoint;
	Ogre::AxisAlignedBox worldBox = getWorldBoundingBox(); //GDC port
	Ogre::Vector3 camPos = cam->getDerivedPosition(); //; Ogre::Vector3(500,0,500);
	Ogre::Vector3 minBoundingBoxPos = worldBox.getMinimum(); //GDC port
	Ogre::Vector3 maxBoundingBoxPos = worldBox.getMaximum(); //GDC port
	closestBoundingBoxPoint.x = Utils::clamp(camPos.x, minBoundingBoxPos.x, maxBoundingBoxPos.x);
	closestBoundingBoxPoint.y = Utils::clamp(camPos.y, minBoundingBoxPos.y, maxBoundingBoxPos.y);
	closestBoundingBoxPoint.z = Utils::clamp(camPos.z, minBoundingBoxPos.z, maxBoundingBoxPos.z);
	float squaredCameraDist = camPos.squaredDistance(closestBoundingBoxPoint);

	float cameraOpening = atan(0.5f * cam->getFOVy().valueRadians()) / (0.5f * cam->getViewport()->getActualHeight());
	float scale = cameraOpening / getHeightfieldGeom()->getWorldQuadSize();
	float squaredPixelsWorldQuad = squaredCameraDist * scale * scale;

	if (mFrameLeastSquaredPixelsWorldQuad < 0.0f || squaredPixelsWorldQuad < mFrameLeastSquaredPixelsWorldQuad)
	{
		mFrameLeastSquaredPixelsWorldQuad = squaredPixelsWorldQuad;
	}
}


// ----------------------------------------------------------------------------
int HeightfieldGeomTile::needsLODUpdate() const
{
	int bestSpacing = getBestLODSpacing();
	if (bestSpacing != mQuadColumnSpacing)
	{
		return bestSpacing;
	}
	return 0;
}


// ----------------------------------------------------------------------------
void HeightfieldGeomTile::postFrameUpdate()
{
	revalidate();

	mFrameLeastSquaredPixelsWorldQuad = -1.0f;
}


// ----------------------------------------------------------------------------
int HeightfieldGeomTile::getCurrentLODSpacing() const
{
	return mQuadColumnSpacing;
}


// ----------------------------------------------------------------------------
int HeightfieldGeomTile::getBestLODSpacing() const
{
	if (mFrameLeastSquaredPixelsWorldQuad >= 0.0f)
	{
		Ogre::Real squaredPixelError = getHeightfieldGeom()->getHeightfieldGeomMaterial()->getLODPixelError(); // 10.0f;
		squaredPixelError *= squaredPixelError;

		Ogre::Real lodPower = getHeightfieldGeom()->getHeightfieldGeomMaterial()->getLODDistancePower(); // 1.0f;

		Ogre::Real lodChangeThreshold = 0.7f;

		Ogre::Real idealSpacing = std::max(1.0f, powf(1.0f + squaredPixelError * mFrameLeastSquaredPixelsWorldQuad, 0.005f * lodPower));
		Ogre::Real currentSpacing = (Ogre::Real)getCurrentLODSpacing();

		Ogre::Real spacingRatio = std::min(idealSpacing / currentSpacing, currentSpacing / idealSpacing);
		
		if (spacingRatio < lodChangeThreshold)
		{
			int spacing = std::max(1, (int)Ogre::Bitwise::firstPO2From((int)idealSpacing)); // rounded power-of-two number

			return spacing;
		}
	}
	return getCurrentLODSpacing();
}


// ----------------------------------------------------------------------------
void HeightfieldGeomTile::updateLOD()
{
	int bestSpacing = getBestLODSpacing();
	if (bestSpacing != mQuadColumnSpacing)
	{
		mQuadColumnSpacing = bestSpacing;
		mQuadRowSpacing = bestSpacing;
		markDirtyData();

		std::list<HeightfieldGeomTile*>::iterator neighborTileIt, neighborTileItEnd = mNeighborTiles.end();
		for (neighborTileIt = mNeighborTiles.begin(); neighborTileIt != neighborTileItEnd; ++neighborTileIt)
		{
			(*neighborTileIt)->markDirtyDataNeighbor();
		}
	}
}
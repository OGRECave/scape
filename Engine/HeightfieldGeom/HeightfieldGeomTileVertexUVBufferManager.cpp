/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "HeightfieldGeomTileVertexUVBufferManager.h"


// ----------------------------------------------------------------------------
namespace ScapeEngine
{
	namespace HeightfieldGeomTileVertexUVBufferManagerNS
	{
		/**
		 * Sortable 'key' description to a cachable vertex UV buffer
		 */
		struct CachedBufferDescription
		{
			Ogre::VertexElementType type;
			Ogre::Rect visibleQuadRect;
			int templateQuadColumnCount;
			int templateQuadRowCount;
			int quadColumnSpacing;
			int quadRowSpacing;


			inline bool operator==(const CachedBufferDescription &rhs) const
			{
				return	type == rhs.type && 
						visibleQuadRect.left == rhs.visibleQuadRect.left &&
						visibleQuadRect.top == rhs.visibleQuadRect.top &&
						visibleQuadRect.right == rhs.visibleQuadRect.right &&
						visibleQuadRect.bottom == rhs.visibleQuadRect.bottom &&
						templateQuadColumnCount == rhs.templateQuadColumnCount && 
						templateQuadRowCount == rhs.templateQuadRowCount &&
						quadColumnSpacing == rhs.quadColumnSpacing &&
						quadRowSpacing == rhs.quadRowSpacing;

			}

			inline bool operator<(const CachedBufferDescription &rhs) const
			{
				if (type == rhs.type)
				{
					if (visibleQuadRect.left == rhs.visibleQuadRect.left)
					{
						if (visibleQuadRect.top == rhs.visibleQuadRect.top)
						{
							if (visibleQuadRect.right == rhs.visibleQuadRect.right)
							{
								if (visibleQuadRect.bottom == rhs.visibleQuadRect.bottom)
								{
									if (templateQuadColumnCount == rhs.templateQuadColumnCount)
									{
										if (templateQuadRowCount == rhs.templateQuadRowCount)
										{
											if (quadColumnSpacing == rhs.quadColumnSpacing)
											{
												return quadRowSpacing < rhs.quadRowSpacing;
											}
											return quadColumnSpacing < rhs.quadColumnSpacing;
										}
										return templateQuadRowCount < rhs.templateQuadRowCount;
									}
									return templateQuadColumnCount < rhs.templateQuadColumnCount;
								}
								return visibleQuadRect.bottom < rhs.visibleQuadRect.bottom;
							}
							return visibleQuadRect.right < rhs.visibleQuadRect.right;
						}
						return visibleQuadRect.top < rhs.visibleQuadRect.top;
					}
					return visibleQuadRect.left < rhs.visibleQuadRect.left;
				}
				return type < rhs.type;
			}
		};

		/**
		 * Shared pointer to and TTL for a cached vertex UV buffer
		 */
		struct CachedBufferReference
		{
			Ogre::HardwareVertexBufferSharedPtr buffer;
			Ogre::Real timeToLive;
		};
	}
}


// ----------------------------------------------------------------------------
using namespace ScapeEngine;
using namespace HeightfieldGeomTileVertexUVBufferManagerNS;


// ----------------------------------------------------------------------------
HeightfieldGeomTileVertexUVBufferManager::HeightfieldGeomTileVertexUVBufferManager()
:	mBufferTimeToLive(1.0f)
{
	wantsPostFrameTick();

	mCachedBuffers = new CachedBuffers();
}


// ----------------------------------------------------------------------------
HeightfieldGeomTileVertexUVBufferManager::~HeightfieldGeomTileVertexUVBufferManager()
{
	delete mCachedBuffers;
}


// ----------------------------------------------------------------------------
Ogre::HardwareVertexBufferSharedPtr HeightfieldGeomTileVertexUVBufferManager::getBuffer(
	Ogre::VertexElementType type, 
	const Ogre::Rect& visibleQuadRect, 
	int templateQuadColumnCount, 
	int templateQuadRowCount,
	int quadColumnSpacing,
	int quadRowSpacing)
{

	CachedBufferDescription key = {type, visibleQuadRect, templateQuadColumnCount, templateQuadRowCount, quadColumnSpacing, quadRowSpacing};
	CachedBuffers::iterator bufferIt = mCachedBuffers->find(key);

	if (bufferIt == mCachedBuffers->end())
	{
		CachedBufferReference value;
		value.timeToLive = mBufferTimeToLive;
		value.buffer = createBuffer(type, visibleQuadRect, templateQuadColumnCount, templateQuadRowCount, quadColumnSpacing, quadRowSpacing);
		(*mCachedBuffers)[key] = value;

		return value.buffer;
	}

	return bufferIt->second.buffer;
}


// ----------------------------------------------------------------------------
void HeightfieldGeomTileVertexUVBufferManager::onPostFrameTick()
{
	Ogre::Real deltaTime = getEngineCore()->getTimeSinceLastFrame();

	CachedBuffers::iterator bufferItEnd = mCachedBuffers->end();
	for (CachedBuffers::iterator bufferIt = mCachedBuffers->begin(); bufferIt != bufferItEnd; )
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
Ogre::HardwareVertexBufferSharedPtr HeightfieldGeomTileVertexUVBufferManager::createBuffer(
	Ogre::VertexElementType elementType, 
	const Ogre::Rect& visibleQuadRect, 
	int templateQuadColumnCount, 
	int templateQuadRowCount,
	int quadColumnSpacing,
	int quadRowSpacing)
{
	assert(templateQuadColumnCount >= 1 && templateQuadRowCount >= 1);
	assert(visibleQuadRect.width() >= 1 && visibleQuadRect.height() >= 1);

	quadColumnSpacing = Utils::clamp(quadColumnSpacing, 1, (int)visibleQuadRect.width());
	quadRowSpacing = Utils::clamp(quadRowSpacing, 1, (int)visibleQuadRect.height());

	Ogre::VertexElementType elementBaseType = Ogre::VertexElement::getBaseType(elementType);
	size_t elementTypeSize = Ogre::VertexElement::getTypeSize(elementType);
	size_t elementTypeCount = Ogre::VertexElement::getTypeCount(elementType);

	int spacedVertexColumnCount = (visibleQuadRect.width() + quadColumnSpacing - 1) / quadColumnSpacing + 1;
	int spacedVertexRowCount = (visibleQuadRect.height() + quadRowSpacing - 1) / quadRowSpacing + 1;
	size_t vertexCount = spacedVertexColumnCount * spacedVertexRowCount;

	Ogre::Real uvRangeMin = Utils::getTypicalVertexElementRangeMin(elementType);
	Ogre::Real uvRangeMax = Utils::getTypicalVertexElementRangeMax(elementType);

	Ogre::HardwareVertexBufferSharedPtr buffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		elementTypeSize,
		vertexCount, 
		Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, 
		false); 

	void* vertexBufferDataPtr = buffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	assert(vertexBufferDataPtr);

	if (elementBaseType == Ogre::VET_FLOAT1)
	{
		typedef Ogre::Real ComponentType;
		assert(sizeof(ComponentType) == Ogre::VertexElement::getTypeSize(elementBaseType));

		ComponentType uFirst = uvRangeMin;
		ComponentType uLast = uvRangeMax;
		ComponentType uStep = (uLast - uFirst) / templateQuadColumnCount;
		ComponentType uSpacing = uStep * quadColumnSpacing;
		uLast = uFirst + uStep * visibleQuadRect.right;
		uFirst = uFirst + uStep * visibleQuadRect.left;
		ComponentType uLastMargin = uLast - (uStep * 0.25f);

		ComponentType vFirst = uvRangeMin;
		ComponentType vLast = uvRangeMax;
		ComponentType vStep = (vLast - vFirst) / templateQuadRowCount;
		ComponentType vSpacing = vStep * quadRowSpacing;
		vLast = vFirst + vStep * visibleQuadRect.bottom;
		vFirst = vFirst + vStep * visibleQuadRect.top;
		ComponentType vLastMargin = vLast - (vStep * 0.25f);

		ComponentType u, v;

		ComponentType* elementPtr = static_cast< ComponentType * >(vertexBufferDataPtr);
		size_t elementPtrSkip = elementTypeCount - 1;

		for (v = vFirst; v < vLastMargin; v += vSpacing)
		{
			for (u = uFirst; u < uLastMargin; u += uSpacing)
			{
				*elementPtr++ = u; 
				*elementPtr = v; 
				elementPtr += elementPtrSkip;
			}
			*elementPtr++ = uLast;
			*elementPtr = v; 
			elementPtr += elementPtrSkip;
		}
		v = vLast;
		for (u = uFirst; u < uLastMargin; u += uSpacing)
		{
			*elementPtr++ = u; 
			*elementPtr = v; 
			elementPtr += elementPtrSkip;
		}
		*elementPtr++ = uLast;
		*elementPtr = v; 
		elementPtr += elementPtrSkip;
	}
	else if (elementBaseType == Ogre::VET_SHORT1)
	{
		typedef short ComponentType;
		assert(sizeof(ComponentType) == Ogre::VertexElement::getTypeSize(elementBaseType));

		// use an intermediate 18.14 format to calculate the 16-bit UVs
		long uFirst14 = ((long)uvRangeMin << 14) + (1 << 13);
		long uLast14 = ((long)uvRangeMax << 14) + (1 << 13);
		long uStep14 = (uLast14 - uFirst14) / templateQuadColumnCount;
		long uSpacing14 = uStep14 * quadColumnSpacing;
		uLast14 = uFirst14 + uStep14 * visibleQuadRect.right;
		uFirst14 = uFirst14 + uStep14 * visibleQuadRect.left;
		long uLastMargin14 = uLast14 - (uStep14 >> 2);

		long vFirst14 = ((long)uvRangeMin << 14) + (1 << 13);
		long vLast14 = ((long)uvRangeMax << 14) + (1 << 13);
		long vStep14 = (vLast14 - vFirst14) / templateQuadRowCount;
		long vSpacing14 = vStep14 * quadRowSpacing;
		vLast14 = vFirst14 + vStep14 * visibleQuadRect.bottom;
		vFirst14 = vFirst14 + vStep14 * visibleQuadRect.top;
		long vLastMargin14 = vLast14 - (vStep14 >> 2);

		long u14, v14;

		ComponentType* elementPtr = static_cast< ComponentType * >(vertexBufferDataPtr);
		size_t elementPtrSkip = elementTypeCount - 1;

		for (v14 = vFirst14; v14 < vLastMargin14; v14 += vSpacing14)
		{
			short v = (ComponentType)(v14 >> 14);

			for (u14 = uFirst14; u14 < uLastMargin14; u14 += uSpacing14)
			{
				short u = (ComponentType)(u14 >> 14);
				*elementPtr++ = u;
				*elementPtr = v;
				elementPtr += elementPtrSkip;
			}
			short u = (ComponentType)(uLast14 >> 14);
			*elementPtr++ = u;
			*elementPtr = v;
			elementPtr += elementPtrSkip;
		}
		short v = (ComponentType)(vLast14 >> 14);

		for (u14 = uFirst14; u14 < uLastMargin14; u14 += uSpacing14)
		{
			short u = (ComponentType)(u14 >> 14);
			*elementPtr++ = u;
			*elementPtr = v;
			elementPtr += elementPtrSkip;
		}
		short u = (ComponentType)(uLast14 >> 14);
		*elementPtr++ = u;
		*elementPtr = v;
		elementPtr += elementPtrSkip;

	}
	else
	{
		assert(false && _T("An vertex UV buffer of an unsupported element type was requested"));
	}

	buffer->unlock();

	return buffer;
}


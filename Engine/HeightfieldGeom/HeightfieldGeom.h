/**
 * ScapeEngine::HeightfieldGeom class
 *
 * Manager of HeightfieldGeomTile objects
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __HEIGHTFIELDGEOM_H__
#define __HEIGHTFIELDGEOM_H__

#include "EngineCore/Tickable.h"

namespace ScapeEngine
{

	class HeightfieldGeom
	{
	public:
		HeightfieldGeom(
			class HeightfieldBuffer* heightfieldBuffer, 
			Ogre::SceneNode* parentSceneNode,
			Ogre::VertexElementType vertexElementTypeUV,
			Ogre::VertexElementType vertexElementTypeHeight);

		~HeightfieldGeom();

		inline class HeightfieldBuffer* getHeightfieldBuffer() const {return mHeightfieldBuffer;}

		Ogre::Real getWorldQuadSize() const {return mWorldQuadSize;}

		Ogre::SceneNode* getParentSceneNode() const {return mParentSceneNode;}

		Ogre::VertexElementType getVertexElementTypeUV() const {return mVertexElementTypeUV;}

		Ogre::VertexElementType getVertexElementTypeHeight() const {return mVertexElementTypeHeight;}

		class HeightfieldGeomMaterial* getHeightfieldGeomMaterial() const {return mHeightfieldGeomMaterial;}

		void tick();

		Ogre::Real getIntersectionPoint(const Ogre::Ray& worldRay);

		Ogre::Real getHeightAt(size_t column, size_t row);

		void markDirtyRect(const Ogre::Rect& dirtyRect);

		Ogre::Vector3 getLocalPosition(const Ogre::Vector3& worldPosition) const;

		const class ShaderCustomAutoConstants* getGeomShaderCustomAutoConstants();

	private:
		class HeightfieldBuffer* mHeightfieldBuffer;

		class HeightfieldGeomMaterial* mHeightfieldGeomMaterial;

		class ShaderCustomAutoConstants* mGeomShaderCustomAutoConstants;
		bool mGeomAutoConstantsDirty;

		Ogre::Vector3 mTopLeftPosition;

		Ogre::Real mWorldQuadSize;

		Ogre::SceneNode* mParentSceneNode;

		typedef std::vector<class HeightfieldGeomTile*> TileRow;
		typedef std::vector<TileRow> Tiles;
		Tiles mTiles;

		Ogre::VertexElementType mVertexElementTypeUV;

		Ogre::VertexElementType mVertexElementTypeHeight;

		bool getTile(int columnIndex, int rowIndex, IVector2& tileIndex);

		HeightfieldGeomTile *createTile(const Ogre::Rect& visibleQuadRect, size_t bufferedQuadColumnCount, size_t bufferedQuadRowCount);

		void updateMaterial();

		Ogre::Real getIntersectionParam(const Ogre::Ray& localRay, size_t column, size_t row);

		unsigned long mMillisLastRevalidate;
	};

}

#endif // __HEIGHTFIELDGEOM_H__
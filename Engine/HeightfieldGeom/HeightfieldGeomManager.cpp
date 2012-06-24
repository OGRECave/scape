/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldGeom.h"
#include "HeightfieldGeomManager.h"
#include "Input/InputManager.h"
#include "Input/InputPointer.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
HeightfieldGeomManager::HeightfieldGeomManager()
:	mCurrentHeightfieldGeom(NULL)
{
	wantsPostFrameTick();
}


// ----------------------------------------------------------------------------
bool HeightfieldGeomManager::getClosestHeightfieldGeomRayIntersection(const Ogre::Ray &ray, const Ogre::SceneManager& sceneManager, class HeightfieldGeom*& outHeightfieldGeom, Ogre::Vector3& outPoint)
{
	HeightfieldGeoms::iterator heightfieldGeomIt = mHeightfieldGeoms.begin(), heightfieldGeomItEnd = mHeightfieldGeoms.end();
	for (; heightfieldGeomIt != heightfieldGeomItEnd; ++heightfieldGeomIt)
	{
		HeightfieldGeom* heightfieldGeom = *heightfieldGeomIt;
		if (heightfieldGeom->getParentSceneNode()->getCreator() == &sceneManager)
		{
			Ogre::Real param = heightfieldGeom->getIntersectionPoint(ray);
			if (param >= 0.0f)
			{
				outPoint = ray.getPoint(param);
				outHeightfieldGeom = heightfieldGeom;
				return true;
			}
		}
	}
	return false;
}


// ----------------------------------------------------------------------------
HeightfieldGeom* HeightfieldGeomManager::create(HeightfieldBuffer* heightfieldBuffer, Ogre::SceneNode* rootNode)
{
	Ogre::VertexElementType uvVertexElementType = Ogre::VET_SHORT2;
	Ogre::VertexElementType heightVertexElementType = Ogre::VET_SHORT4;

	HeightfieldGeom* heightfieldGeom = new HeightfieldGeom(
		heightfieldBuffer, 
		rootNode, 
		uvVertexElementType,
		heightVertexElementType);

	mHeightfieldGeoms.push_back(heightfieldGeom);

	if (mCurrentHeightfieldGeom == NULL)
	{
		mCurrentHeightfieldGeom = heightfieldGeom;
	}

	return heightfieldGeom;
}


// ----------------------------------------------------------------------------
void HeightfieldGeomManager::destroy(HeightfieldGeom* heightfieldGeom)
{
	HeightfieldGeoms::iterator heightfieldGeomIt = mHeightfieldGeoms.begin(), heightfieldGeomItEnd = mHeightfieldGeoms.end();
	for (; heightfieldGeomIt != heightfieldGeomItEnd; ++heightfieldGeomIt)
	{
		if (*heightfieldGeomIt == heightfieldGeom)
		{
			delete *heightfieldGeomIt;
			mHeightfieldGeoms.erase(heightfieldGeomIt);
			break;
		}
	}

	if (heightfieldGeom == mCurrentHeightfieldGeom)
	{
		mCurrentHeightfieldGeom = NULL;
	}
}


// ----------------------------------------------------------------------------
void HeightfieldGeomManager::onPostFrameTick()
{
	HeightfieldGeoms::iterator heightfieldGeomIt = mHeightfieldGeoms.begin(), heightfieldGeomItEnd = mHeightfieldGeoms.end();
	for (; heightfieldGeomIt != heightfieldGeomItEnd; ++heightfieldGeomIt)
	{
		(*heightfieldGeomIt)->tick();
	}

	HeightfieldGeom* currentGeom = getEngineCore()->getInputManager()->getInputPointer()->getCurrentState().get3DPointGeom();
	if (currentGeom)
	{
		mCurrentHeightfieldGeom = currentGeom;
	}


}


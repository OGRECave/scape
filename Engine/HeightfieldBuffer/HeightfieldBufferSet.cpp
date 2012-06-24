/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "HeightfieldBuffer.h"
#include "HeightfieldBufferSet.h"

using namespace ScapeEngine;


// ----------------------------------------------------------------------------
HeightfieldBufferSet::HeightfieldBufferSet(const string& name, int elementColumnCount, int elementRowCount, Ogre::Real heightRangeMin, Ogre::Real heightRangeMax, int elementColumnCountPerPage, int elementRowCountPerPage)
:	mName(name),
	mElementColumnCount(elementColumnCount),
	mElementRowCount(elementRowCount),
	mHeightRangeMin(heightRangeMin),
	mHeightRangeMax(heightRangeMax),
	mElementColumnCountPerPage(elementColumnCountPerPage),
	mElementRowCountPerPage(elementRowCountPerPage)
{
	mPageColumnCount = (int)ceil((Ogre::Real)elementColumnCount / mElementColumnCountPerPage);
	mPageRowCount = (int)ceil((Ogre::Real)elementRowCount / mElementRowCountPerPage);

	mHandle = Utils::createGUID();
}


// ----------------------------------------------------------------------------
HeightfieldBufferSet::~HeightfieldBufferSet()
{
	HeightfieldBufferMap::iterator it = mHeightfieldBufferMap.begin(), itEnd = mHeightfieldBufferMap.end();
	for (; it != itEnd; ++it)
	{
		delete it->second;
	}
}


// ----------------------------------------------------------------------------
HeightfieldBufferHandle HeightfieldBufferSet::create(const string& name, Ogre::PixelFormat format)
{
	HeightfieldBuffer* heightfieldBuffer = new HeightfieldBuffer(
		this,
		name, 
		format, 
		mElementColumnCountPerPage, 
		mElementRowCountPerPage, 
		mElementColumnCount,
		mElementRowCount,
		mHeightRangeMin,
		mHeightRangeMax);

	HeightfieldBufferHandle handle = heightfieldBuffer->getHandle();
	mHeightfieldBufferMap[handle] = heightfieldBuffer;

	Ogre::LogManager::getSingleton().logMessage(_T("Creating HeightfieldBuffer '") + name + _T("' in set '") + getName() + _T("'"));

	return handle;
}


// ----------------------------------------------------------------------------
HeightfieldBufferHandle HeightfieldBufferSet::createFrom(const string& name, HeightfieldBufferHandle originalBufferHandle, bool sharePages)
{
	HeightfieldBuffer* originalBuffer = get(originalBufferHandle);
	if (originalBuffer)
	{
		HeightfieldBufferHandle newHandle = create(name, originalBuffer->getHeightElementFormat());
		HeightfieldBuffer* newBuffer = get(newHandle);
		if (newBuffer)
		{
			Ogre::LogManager::getSingleton().logMessage(_T("Creating HeightfieldBuffer '") + name + _T("' from '") + originalBuffer->getName() + _T("' in set '") + getName() + _T("'"));
			newBuffer->cloneFrom(originalBuffer, sharePages);
			return newHandle;
		}
	}
	return 0;
}


// ----------------------------------------------------------------------------
HeightfieldBufferHandle HeightfieldBufferSet::findHandle(const string& name)
{
	HeightfieldBufferMap::iterator it = mHeightfieldBufferMap.begin(), itEnd = mHeightfieldBufferMap.end();
	for (; it != itEnd; ++it)
	{
		if (it->second->getName() == name)
		{
			return it->first;
		}
	}
	return 0;
}


// ----------------------------------------------------------------------------
bool HeightfieldBufferSet::destroy(HeightfieldBufferHandle handle)
{
	HeightfieldBufferMap::iterator it = mHeightfieldBufferMap.find(handle);

	if (it != mHeightfieldBufferMap.end())
	{
		Ogre::LogManager::getSingleton().logMessage(_T("Destroying HeightfieldBuffer '") + it->second->getName() + _T("' in set '") + getName() + _T("'"));

		delete it->second;
		mHeightfieldBufferMap.erase(it);
		return true;
	}

	return false;
}



// ----------------------------------------------------------------------------
HeightfieldBuffer* HeightfieldBufferSet::get(HeightfieldBufferHandle handle)
{
	HeightfieldBufferMap::iterator it = mHeightfieldBufferMap.find(handle);

	return it != mHeightfieldBufferMap.end() ? it->second : NULL;
}


// ----------------------------------------------------------------------------
void HeightfieldBufferSet::unlockAll()
{
	HeightfieldBufferMap::iterator it = mHeightfieldBufferMap.begin(), itEnd = mHeightfieldBufferMap.end();
	for (; it != itEnd; ++it)
	{
		it->second->unlockAll();
	}
}

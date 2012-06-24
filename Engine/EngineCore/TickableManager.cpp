/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "TickableManager.h"
#include "Tickable.h"
#include "Utils/Utils.h"

using namespace ScapeEngine;

template<> TickableManager* Ogre::Singleton<TickableManager>::msSingleton = 0;

// ----------------------------------------------------------------------------
void TickableManager::disposeTickable(Tickable* tickable)
{
	DWORD predisposeBitmask = 1 << ((DWORD)TICKABLEDISPOSEID_PREDISPOSE);

	if ((tickable->mTickableFlags & predisposeBitmask) == 0)
	{
		mDisposeEventList.push_back(tickable); 
		tickable->mTickableFlags = predisposeBitmask; // clear all flags other than predispose
	}
}


// ----------------------------------------------------------------------------
void TickableManager::disposeAllActiveTickables(bool forceInstant)
{
	for (int listIndex = 0; listIndex < TICKEVENTID_COUNT; ++listIndex)
	{
		TickableEventList& list = mTickEventLists[listIndex];

		TickableEventList::iterator tickableIt;
		for (tickableIt = list.begin(); tickableIt != list.end(); ++tickableIt)
		{
			disposeTickable(*tickableIt);
		}
	}

	if (forceInstant)
	{
		postFrameCleanup();
		postFrameCleanup();
	}

}


// ----------------------------------------------------------------------------
void TickableManager::updateTickableEvent(Tickable* tickable, ETickableEventId eventId, bool enable)
{
	DWORD tickEventBitmask = 1 << ((DWORD)eventId);
	if (enable != ((tickable->mTickableFlags & tickEventBitmask) > 0))
	{
		if (enable)
		{
			mTickEventLists[(int)eventId].push_back(tickable);
			tickable->mTickableFlags |= tickEventBitmask;
		}
		else
		{
			// the tickable will be removed from the respective tickEventList on this list's next tick traversal
			tickable->mTickableFlags &= ~tickEventBitmask;
		}
	}
}


// ----------------------------------------------------------------------------
void TickableManager::fireTickableEvents(ETickableEventId eventId, void (Tickable::*tickEvent)())
{
	DWORD tickEventBitmask = 1 << ((DWORD)eventId);
	TickableEventList& list = mTickEventLists[(int)eventId];

	for (TickableEventList::size_type tickableIndex = 0; tickableIndex < list.size(); )
	{
		Tickable* &tickable = list[tickableIndex];

		if (tickable->mTickableFlags & tickEventBitmask)
		{
			(tickable->*tickEvent)();
			++tickableIndex;
		}
		else
		{
			// remove from list
			tickable = list.back();
			list.pop_back();
		}
	}
}


// ----------------------------------------------------------------------------
void TickableManager::postFrameCleanup()
{
	DWORD predisposeBitmask = 1 << ((DWORD)TICKABLEDISPOSEID_PREDISPOSE);

	for (TickableEventList::size_type tickableIndex = 0; tickableIndex < mDisposeEventList.size(); )
	{
		Tickable* &tickable = mDisposeEventList[tickableIndex];
		if (tickable->mTickableFlags & predisposeBitmask)
		{
			tickable->mTickableFlags = 0; // clear predispose flag
			++tickableIndex;
		}
		else
		{
			delete tickable;
			// remove from list
			tickable = mDisposeEventList.back();
			mDisposeEventList.pop_back();
		}
	}
}


// ----------------------------------------------------------------------------
void TickableManager::onPreSubTick()
{
	fireTickableEvents(TICKEVENTID_PRESUBTICK, &Tickable::onPreSubTick);
}


// ----------------------------------------------------------------------------
void TickableManager::onSubTick()
{
	fireTickableEvents(TICKEVENTID_SUBTICK, &Tickable::onSubTick);
}


// ----------------------------------------------------------------------------
void TickableManager::onPostSubTick()
{
	fireTickableEvents(TICKEVENTID_POSTSUBTICK, &Tickable::onPostSubTick);
}


// ----------------------------------------------------------------------------
void TickableManager::onPreFrameTick()
{
	fireTickableEvents(TICKEVENTID_PREFRAMETICK, &Tickable::onPreFrameTick);
}


// ----------------------------------------------------------------------------
void TickableManager::onFrameTick()
{
	fireTickableEvents(TICKEVENTID_FRAMETICK, &Tickable::onFrameTick);
}


// ----------------------------------------------------------------------------
void TickableManager::onPostFrameTick()
{
	fireTickableEvents(TICKEVENTID_POSTFRAMETICK, &Tickable::onPostFrameTick);
}
/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "Tickable.h"
#include "TickableManager.h"

using namespace ScapeEngine;

void Tickable::dispose() { getEngineCore()->getTickableManager()->disposeTickable(this); }

void Tickable::wantsPreSubTick(bool enable)
{
    getEngineCore()->getTickableManager()->updateTickableEvent(
        this, TickableManager::TICKEVENTID_PRESUBTICK, enable);
}

void Tickable::wantsSubTick(bool enable)
{
    getEngineCore()->getTickableManager()->updateTickableEvent(this, TickableManager::TICKEVENTID_SUBTICK,
                                                               enable);
}

void Tickable::wantsPostSubTick(bool enable)
{
    getEngineCore()->getTickableManager()->updateTickableEvent(
        this, TickableManager::TICKEVENTID_POSTSUBTICK, enable);
}

void Tickable::wantsPreFrameTick(bool enable)
{
    getEngineCore()->getTickableManager()->updateTickableEvent(
        this, TickableManager::TICKEVENTID_PREFRAMETICK, enable);
}

void Tickable::wantsFrameTick(bool enable)
{
    getEngineCore()->getTickableManager()->updateTickableEvent(this, TickableManager::TICKEVENTID_FRAMETICK,
                                                               enable);
}

void Tickable::wantsPostFrameTick(bool enable)
{
    getEngineCore()->getTickableManager()->updateTickableEvent(
        this, TickableManager::TICKEVENTID_POSTFRAMETICK, enable);
}

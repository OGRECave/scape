/**
 * ScapeEngine::TickableManager class
 *
 * Manages all Tickable object registration, deconstruction and tick event calls.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __TICKABLEMANAGER_H__
#define __TICKABLEMANAGER_H__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{


class TickableManager
{

public:
    // Enumeration of all known tick events
    enum ETickableEventId
    {
        TICKEVENTID_PRESUBTICK,
        TICKEVENTID_SUBTICK,
        TICKEVENTID_POSTSUBTICK,
        TICKEVENTID_PREFRAMETICK,
        TICKEVENTID_FRAMETICK,
        TICKEVENTID_POSTFRAMETICK,
        TICKEVENTID_COUNT
    };

    // Continuation of ETickableEventId, containing a predispose state
    enum ETickableDisposeId
    {
        TICKABLEDISPOSEID_PREDISPOSE = TICKEVENTID_COUNT,
    };

    // Update state of Tickable, enabling or disabling one type of tick call
    void updateTickableEvent(Tickable* tickable, ETickableEventId eventId, bool enable);

    // Dispose of one Tickable object
    void disposeTickable(Tickable* tickable);

    // Dispose of all Tickable objects registered with this TickableManager
    void disposeAllActiveTickables(bool forceInstant);

    // Fixed-high-frequency pre-tick call handler. Calls all interested registered Tickables.
    virtual void onPreSubTick();

    // Fixed-high-frequency tick call handler. Calls all interested registered Tickables.
    virtual void onSubTick();

    // Fixed-high-frequency post-tick call handler. Calls all interested registered Tickables.
    virtual void onPostSubTick();

    // Once-per-frame pre-tick call handler. Calls all interested registered Tickables.
    virtual void onPreFrameTick();

    // Once-per-frame tick call handler. Calls all interested registered Tickables.
    virtual void onFrameTick();

    // Once-per-frame post-tick call handler. Calls all interested registered Tickables.
    virtual void onPostFrameTick();

    // Perform any pending two-phased Tickable disposals. Should be called after all
    // on....Tick() calls have been made in the renderloop cycle.
    void postFrameCleanup();

private:
    // Container type of pointers to Tickables.
    typedef std::deque<Tickable*> TickableEventList;

    // Container of the Tickables interested per ETickableEventId
    TickableEventList mTickEventLists[TICKEVENTID_COUNT];

    // Container of the Tickables that are ready to be disposed
    TickableEventList mDisposeEventList;

    // Helper function to call all interested Tickables for the speficied ETickableEventId
    void fireTickableEvents(ETickableEventId eventId, void (Tickable::*tickEvent)());
};
}
#endif // __TICKABLEMANAGER_H__
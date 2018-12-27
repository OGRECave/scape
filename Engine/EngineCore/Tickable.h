/**
 * ScapeEngine::Tickable class
 *
 * Base class for all objects that want to receive a call at one or more specific moments during a
 * renderloop cycle
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __TICKABLE_H__
#define __TICKABLE_H__

#include "ScapeEnginePrerequisites.h"

#include "OgrePlatform.h"

namespace ScapeEngine
{

class Tickable
{

    // Allow TickableManager to access mTickableFlags
    friend class TickableManager;

public:
    // Fixed-high-frequency pre-tick call handler
    virtual void onPreSubTick() {}

    // Fixed-high-frequency tick call handler
    virtual void onSubTick() {}

    // Fixed-high-frequency post-tick call handler
    virtual void onPostSubTick() {}

    // Once-per-frame pre-tick call handler
    virtual void onPreFrameTick() {}

    // Once-per-frame tick call handler
    virtual void onFrameTick() {}

    // Once-per-frame post-tick call handler
    virtual void onPostFrameTick() {}

    // Dispose of this object a.s.a.p. Guarantees that no other
    // tick call handler will be called. The object will
    // be destroyed within two frame cycles. Iff now is true,
    // it will be destroyed right away.
    virtual void dispose();

protected:
    // Non-public constructor. A derived class should create it's own
    // (public) constructor.
    Tickable() : mTickableFlags(0) {}

    // Non-public deconstructor. Prevents dangerous direct calls,
    // as deconstruction should be done by a TickableManager.
    virtual ~Tickable() {}

    // Activate or deactive fixed-high-frequency pre-tick call for this object
    void wantsPreSubTick(bool enable = true);

    // Activate or deactive fixed-high-frequency tick call for this object
    void wantsSubTick(bool enable = true);

    // Activate or deactive fixed-high-frequency post-tick call for this object
    void wantsPostSubTick(bool enable = true);

    // Activate or deactive once-per-frame pre-tick call for this object
    void wantsPreFrameTick(bool enable = true);

    // Activate or deactive once-per-frame tick call for this object
    void wantsFrameTick(bool enable = true);

    // Activate or deactive once-per-frame post-tick call for this object
    void wantsPostFrameTick(bool enable = true);

private:
    // Internal state of which tick calls this object would
    // like to receive and which not
    DWORD mTickableFlags;
};
}

#endif // __TICKABLE_H__

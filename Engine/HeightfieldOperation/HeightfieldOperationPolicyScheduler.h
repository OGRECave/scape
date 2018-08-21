/**
 * ScapeEngine::HeightfieldOperationBrushInstancr class
 *
 * An abstract super class for brush instances policies for HeightfieldOperations
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONPOLICYSCHEDULER_H__
#define __HEIGHTFIELDOPERATIONPOLICYSCHEDULER_H__

#include "HeightfieldOperationBrush.h"

namespace ScapeEngine
{
class HeightfieldBrush;

class HeightfieldOperationPolicyScheduler
{
public:
    virtual ~HeightfieldOperationPolicyScheduler() {}

protected:
    virtual void schedulerTick() = 0;
    virtual bool isSchedulerDone() = 0;
};
}

#endif // __HEIGHTFIELDOPERATIONPOLICYSCHEDULER_H__

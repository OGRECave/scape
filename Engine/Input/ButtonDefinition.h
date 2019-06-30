/**
 * ScapeEngine::ButtonDefinition class
 *
 * Definition of a button identifier through its list of defining hardware buttons
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __BUTTONDEFINITION_H__
#define __BUTTONDEFINITION_H__

#include "ScapeEnginePrerequisites.h"

#include <unordered_set>
#include <vector>

#include "ButtonId.h"
#include "DeviceButtonId.h"

namespace ScapeEngine
{

class _ScapeEngineExport ButtonDefinition
{
public:
    // Device button ordered container type
    typedef std::unordered_set<DeviceButtonId::EDeviceButtonId> DeviceButtonSet;
    typedef std::vector<DeviceButtonSet> DeviceButtonSetsVector;

    // Construct a new definition of the given button identifier
    ButtonDefinition(ButtonId::EButtonId buttonId, int priority);

    ButtonId::EButtonId getButtonId() const;

    int getPriority() const;
    void setPriority(int priority);

    const DeviceButtonSetsVector& getDeviceButtonSets() const;
    DeviceButtonSetsVector& getDeviceButtonSets();

protected:
    // Identifier of defined button
    ButtonId::EButtonId mButtonId;

    int mPriority;

    // Device button container
    DeviceButtonSetsVector mDeviceButtonSets;
};
}

#endif // __BUTTONDEFINITION_H__

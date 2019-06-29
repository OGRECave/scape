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

#include "ButtonId.h"
#include "DeviceButtonId.h"

namespace ScapeEngine
{

class _ScapeEngineExport ButtonDefinition
{
public:
    // Device button ordered container type
    typedef std::unordered_set<DeviceButtonId::EDeviceButtonId> DeviceButtonSet;

    // Construct a new definition of the given button identifier
    ButtonDefinition(ButtonId::EButtonId buttonId, int priority);

    ButtonId::EButtonId getButtonId() const;

    int getPriority() const;

    const DeviceButtonSet& getDeviceButtons() const;
    DeviceButtonSet& getDeviceButtons();

    // Add a device button button to this definition
    void addDeviceButton(DeviceButtonId::EDeviceButtonId buttonId);

protected:
    // Identifier of defined button
    ButtonId::EButtonId mButtonId;

    int mPriority;

    // Device button container
    DeviceButtonSet mDeviceButtons;
};
}

#endif // __BUTTONDEFINITION_H__

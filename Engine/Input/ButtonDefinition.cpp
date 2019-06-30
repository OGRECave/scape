/**
 * G.J.P. de Carpentier, 2007
 */

#include "ScapeEngineStableHeaders.h"
#include "ButtonDefinition.h"

using namespace ScapeEngine;

ButtonDefinition::ButtonDefinition(ButtonId::EButtonId buttonId, int priority)
    : mButtonId(buttonId), mPriority(priority)
{
}

ButtonId::EButtonId ButtonDefinition::getButtonId() const { return mButtonId; }

int ButtonDefinition::getPriority() const { return mPriority; }

void ButtonDefinition::setPriority(int priority) { mPriority = priority; }

const ButtonDefinition::DeviceButtonSetsVector& ButtonDefinition::getDeviceButtonSets() const
{
    return mDeviceButtonSets;
}

ButtonDefinition::DeviceButtonSetsVector& ButtonDefinition::getDeviceButtonSets()
{
    return mDeviceButtonSets;
}

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

const ButtonDefinition::DeviceButtonSet& ButtonDefinition::getDeviceButtons() const
{
    return mDeviceButtons;
}

void ButtonDefinition::addDeviceButton(DeviceButtonId::EDeviceButtonId buttonId)
{
    mDeviceButtons.insert(buttonId);
}

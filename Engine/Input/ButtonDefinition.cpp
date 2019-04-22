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

void ButtonDefinition::addDeviceButton(DeviceButtonId::EDeviceButtonId buttonId)
{
    mDeviceButtons.insert(buttonId);
}

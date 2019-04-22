/**
 * G.J.P. de Carpentier, 2007
 */

#include "ScapeEngineStableHeaders.h"
//#include "DeviceButton.h"
#include "ButtonDefinition.h"

using namespace ScapeEngine;

ButtonDefinition::ButtonDefinition(ButtonId::EButtonId buttonId, int priority)
    : mButtonId(buttonId), mPriority(priority)
{
}

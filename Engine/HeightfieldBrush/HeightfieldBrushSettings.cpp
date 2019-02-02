#include "ScapeEngineStableHeaders.h"

#include "HeightfieldBrushSettings.h"

namespace ScapeEngine
{

HeightfieldBrushSettings::HeightfieldBrushSettings()
    : mInnerRadius(0.0f), mOuterRadius(1.0f), mRampPower(1.0f)
{
}

HeightfieldBrushSettings::~HeightfieldBrushSettings() {}

Ogre::Real HeightfieldBrushSettings::getInnerRadius() const { return mInnerRadius; }

void HeightfieldBrushSettings::setInnerRadius(Ogre::Real innerRadius)
{
    mInnerRadius = innerRadius;
    notifyObservers();
}

Ogre::Real HeightfieldBrushSettings::getOuterRadius() const { return mOuterRadius; }

void HeightfieldBrushSettings::setOuterRadius(Ogre::Real outerRadius)
{
    mOuterRadius = outerRadius;
    notifyObservers();
}

Ogre::Real HeightfieldBrushSettings::getRampPower() const { return mRampPower; }

void HeightfieldBrushSettings::setRampPower(Ogre::Real rampPower)
{
    mRampPower = rampPower;
    notifyObservers();
}
}

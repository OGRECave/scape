#include "HeightfieldBrushState.h"

namespace ScapeEngine
{

HeightfieldBrushState::HeightfieldBrushState() : mPosition(0.0f, 0.0f, 0.0f), mPressure(1.0) {}

HeightfieldBrushState::~HeightfieldBrushState() {}

Ogre::Vector3 HeightfieldBrushState::getPosition() const { return mPosition; }

void HeightfieldBrushState::setPosition(const Ogre::Vector3& position) { mPosition = position; }

Ogre::Real HeightfieldBrushState::getPressure() const { return mPressure; }

void HeightfieldBrushState::setPressure(Ogre::Real pressure) { mPressure = pressure; }
}

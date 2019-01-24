#include "ScapeEngineStableHeaders.h"

#include "HeightfieldOperationBrush.h"

namespace ScapeEngine
{
HeightfieldOperationBrush::HeightfieldOperationBrush() : mBrush(NULL), mStrength(1.0f) {}

HeightfieldOperationBrush::~HeightfieldOperationBrush() {}

HeightfieldBrush* HeightfieldOperationBrush::getBrush() { return mBrush; }
Ogre::Real HeightfieldOperationBrush::getStrength() { return mStrength; }
}

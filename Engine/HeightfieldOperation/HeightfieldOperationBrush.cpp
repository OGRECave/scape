#include "ScapeEngineStableHeaders.h"

#include "HeightfieldOperationBrush.h"
#include "HeightfieldBrush/HeightfieldBrushManager.h"
#include "HeightfieldBrush/HeightfieldBrushSettings.h"

namespace ScapeEngine
{
HeightfieldOperationBrush::HeightfieldOperationBrush()
    : mBrush(NULL), mStrength(1.0f),
      mHeightfieldBrushSettings(
          getEngineCore()->getHeightfieldBrushManager()->getHeightfieldBrushSettings()),
      mHeightfieldBrushState(getEngineCore()->getHeightfieldBrushManager()->getHeightfieldBrushState())
{
}

HeightfieldOperationBrush::~HeightfieldOperationBrush() {}

HeightfieldBrush* HeightfieldOperationBrush::getBrush() { return mBrush; }
Ogre::Real HeightfieldOperationBrush::getStrength() { return mStrength; }
}

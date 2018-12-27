/**
 * ScapeEngine::HeightfieldOperationCPUNoise class
 *
 * A base class for all CPU-based operations operated like a brush tool
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONCPUNOISE_H__
#define __HEIGHTFIELDOPERATIONCPUNOISE_H__

#include "ScapeEnginePrerequisites.h"

#include "HeightfieldOperationCPU.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine
{
class HeightfieldGeom;

class HeightfieldOperationCPUNoise : public HeightfieldOperationCPU
{
    DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationCPUNoise)
public:
    DEFINE_FACTORYCLASS(HeightfieldOperationCPUNoise, HeightfieldOperation)

    HeightfieldOperationCPUNoise();

protected:
    static Ogre::Vector3 mNoiseOrigin;

    bool mHasPreviousPosition;
    Ogre::Vector3 mPositionPrevious;

    Ogre::Real mPropMinRange;
    Ogre::Real mPropMaxRange;
    Ogre::Real mPropHiAmp;
    Ogre::Real mPropMidAmp;

    virtual void applyPrimary(const Ogre::Vector3& position, Ogre::Real strength);
    virtual void applySecondary(const Ogre::Vector3& position, Ogre::Real strength) {}

    virtual bool initPersistentElementValueMap(StringStringMap& map);
    virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
    virtual string setUIElementPropertyValue(const string& elementName, const string& value);
};
}

#endif // __HEIGHTFIELDOPERATIONCPUNOISE_H__

/**
 * ScapeEngine::HeightfieldOperationCPUSmooth class
 *
 * A base class for all CPU-based operations operated like a brush tool
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONCPUSMOOTH_H__
#define __HEIGHTFIELDOPERATIONCPUSMOOTH_H__

#include "ScapeEnginePrerequisites.h"

#include "HeightfieldOperationCPU.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine
{
class HeightfieldGeom;

class HeightfieldOperationCPUSmooth : public HeightfieldOperationCPU
{
    DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationCPUSmooth)
public:
    DEFINE_FACTORYCLASS(HeightfieldOperationCPUSmooth, HeightfieldOperation)

    HeightfieldOperationCPUSmooth();

protected:
    virtual void applyPrimary(const Ogre::Vector3& position, Ogre::Real strength);
    virtual void applySecondary(const Ogre::Vector3& position, Ogre::Real strength);

    virtual bool initPersistentElementValueMap(StringStringMap& map);
    virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
    virtual string setUIElementPropertyValue(const string& elementName, const string& value);
};
}

#endif // __HEIGHTFIELDOPERATIONCPUSMOOTH_H__

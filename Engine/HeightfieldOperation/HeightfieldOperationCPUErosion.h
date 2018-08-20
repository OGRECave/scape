/**
 * ScapeEngine::HeightfieldOperationCPUErosion class
 *
 * A base class for all CPU-based operations operated like a brush tool
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDOPERATIONCPUEROSION_H__
#define __HEIGHTFIELDOPERATIONCPUEROSION_H__

#include "HeightfieldOperationCPU.h"
#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine {
    class HeightfieldOperationCPUErosion : public HeightfieldOperationCPU
    {
        DEFINE_UIELEMENTCONTAINERSIMPLE_CLASS(HeightfieldOperationCPUErosion)
    public:
        DEFINE_FACTORYCLASS(HeightfieldOperationCPUErosion, HeightfieldOperation)

        Ogre::Real mKR;
        Ogre::Real mKD;
        Ogre::Real mKC;
        Ogre::Real mKS;
        Ogre::Real mKA;

        HeightfieldOperationCPUErosion();

    protected:
        void applyPrimary();

        virtual void applyPrimary(const Ogre::Vector3& position, Ogre::Real strength);
        virtual void applySecondary(const Ogre::Vector3& position, Ogre::Real strength) {}
        virtual bool initPersistentElementValueMap(StringStringMap& map);
        virtual bool initPersistentElementStringEnumMap(StringEnumMap& map);
        virtual string setUIElementPropertyValue(const string& elementName, const string& value);
    };
}

#endif // __HEIGHTFIELDOPERATIONCPUEROSION_H__

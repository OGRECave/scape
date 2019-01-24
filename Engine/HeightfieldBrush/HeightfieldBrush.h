/**
 * ScapeEngine::Brush class
 *
 * HeightfieldBrush base tool
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDBRUSH_H__
#define __HEIGHTFIELDBRUSH_H__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{
class HeightfieldBrush
{
public:
    HeightfieldBrush();

    void setPosition(const Ogre::Vector3& position);

    void setPressure(const Ogre::Real pressure);

    inline Ogre::Vector3 getPosition() const { return mPosition; }

    inline Ogre::Real getPressure() const { return mPressure; }

    virtual void tick();

    bool isPrimaryActive() { return mPrimaryActive; }

    bool isSecondaryActive() { return mSecondaryActive; }

    bool isActive() { return mPrimaryActive || mSecondaryActive; }

    class HeightfieldGeom* getHeightfieldGeom() { return mHeightfieldGeom; }

    void setHeightfieldGeom(HeightfieldGeom* heightfieldGeom) { mHeightfieldGeom = heightfieldGeom; }

private:
    Ogre::Vector3 mPosition;

    Ogre::Real mPressure;

    bool mPrimaryActive;
    bool mSecondaryActive;

    HeightfieldGeom* mHeightfieldGeom;
};
}

#endif // __HEIGHTFIELDBRUSH_H__
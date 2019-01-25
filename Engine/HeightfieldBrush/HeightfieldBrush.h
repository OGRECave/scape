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

    virtual void tick();

    bool isPrimaryActive() { return mPrimaryActive; }

    bool isSecondaryActive() { return mSecondaryActive; }

    bool isActive() { return mPrimaryActive || mSecondaryActive; }

    class HeightfieldGeom* getHeightfieldGeom() { return mHeightfieldGeom; }

    void setHeightfieldGeom(HeightfieldGeom* heightfieldGeom) { mHeightfieldGeom = heightfieldGeom; }

private:
    bool mPrimaryActive;
    bool mSecondaryActive;

    HeightfieldGeom* mHeightfieldGeom;
};
}

#endif // __HEIGHTFIELDBRUSH_H__
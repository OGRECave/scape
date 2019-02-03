#ifndef __HEIGHTFIELDBRUSHSETTINGS_H__
#define __HEIGHTFIELDBRUSHSETTINGS_H__

#include "ScapeEnginePrerequisites.h"

#include "Utils/Observer.h"

namespace ScapeEngine
{

class _ScapeEngineExport HeightfieldBrushSettings : public Subject
{
public:
    HeightfieldBrushSettings();
    ~HeightfieldBrushSettings();

    Ogre::Real getRadius() const;
    void setRadius(Ogre::Real radius);

    Ogre::Real getInnerRadius() const;
    void setInnerRadius(Ogre::Real innerRadius);

    Ogre::Real getOuterRadius() const;
    void setOuterRadius(Ogre::Real outerRadius);

    Ogre::Real getRampPower() const;
    void setRampPower(Ogre::Real rampPower);

protected:
    Ogre::Real mInnerRadius;
    Ogre::Real mOuterRadius;
    Ogre::Real mRampPower;
};
}

#endif /* __HEIGHTFIELDBRUSHSETTINGS_H__ */

#ifndef __HEIGHTFIELDBRUSHSTATE_H__
#define __HEIGHTFIELDBRUSHSTATE_H__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{

class HeightfieldBrushState
{
public:
    HeightfieldBrushState();
    ~HeightfieldBrushState();

    Ogre::Vector3 getPosition() const;
    void setPosition(const Ogre::Vector3& position);

    Ogre::Real getPressure() const;
    void setPressure(const Ogre::Real pressure);

protected:
    Ogre::Vector3 mPosition;
    Ogre::Real mPressure;
};
}

#endif /* __HEIGHTFIELDBRUSHSTATE_H__ */

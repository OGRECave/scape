#ifndef __STARTUPSETTINGS_H__
#define __STARTUPSETTINGS_H__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{

class StartupSettings
{
public:
    StartupSettings();
    ~StartupSettings();

    int getHeightfieldRows() const;
    void setHeightfieldRows(int heightfieldRows);

    int getHeightfieldColumns() const;
    void setHeightfieldColumns(int heightfieldColumns);

    Ogre::Real getHeightfieldHeight() const;
    void setHeightfieldHeight(Ogre::Real heightfieldHeight);

protected:
    int mHeightfieldRows;
    int mHeightfieldColumns;
    Ogre::Real mHeightfieldHeight;
};
}

#endif /* __STARTUPSETTINGS_H__ */

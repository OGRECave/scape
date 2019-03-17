#ifndef __STARTUPSETTINGSDATAACCESSOBJECT_H__
#define __STARTUPSETTINGSDATAACCESSOBJECT_H__

#include "ScapeEnginePrerequisites.h"

#include "StartupSettings.h"

namespace ScapeEngine
{

class StartupSettingsDataAccessObject
{
public:
    virtual ~StartupSettingsDataAccessObject();

    virtual const StartupSettings getStartupSettings() const = 0;

protected:
    StartupSettingsDataAccessObject();
};
}

#endif /* __STARTUPSETTINGSDATAACCESSOBJECT_H__ */

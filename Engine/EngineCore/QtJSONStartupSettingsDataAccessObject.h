#ifndef __QTJSONSTARTUPSETTINGSDATAACCESSOBJECT_H__
#define __QTJSONSTARTUPSETTINGSDATAACCESSOBJECT_H__

#include "ScapeEnginePrerequisites.h"

#include "StartupSettingsDataAccessObject.h"

#include <string>

namespace ScapeEngine
{

class QtJSONStartupSettingsDataAccessObject : public StartupSettingsDataAccessObject
{
public:
    QtJSONStartupSettingsDataAccessObject(std::string fileName);
    virtual ~QtJSONStartupSettingsDataAccessObject();

    virtual const StartupSettings getStartupSettings() const;
    virtual void updateStartupSettings(const StartupSettings& startupSettings);

protected:
    std::string mFileName;
};
}

#endif /* __QTJSONSTARTUPSETTINGSDATAACCESSOBJECT_H__ */

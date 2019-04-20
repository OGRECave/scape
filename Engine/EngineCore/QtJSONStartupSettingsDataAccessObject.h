#ifndef __QTJSONSTARTUPSETTINGSDATAACCESSOBJECT_H__
#define __QTJSONSTARTUPSETTINGSDATAACCESSOBJECT_H__

#include "ScapeEnginePrerequisites.h"

#include "StartupSettingsDataAccessObject.h"

#include <string>

#include "Utils/QtJSONFileHelper.h"

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
    QtJSONFileHelper mFileHelper;
};
}

#endif /* __QTJSONSTARTUPSETTINGSDATAACCESSOBJECT_H__ */

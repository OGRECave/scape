#ifndef __SETTINGSDATASETDATAACCESSOBJECT_H__
#define __SETTINGSDATASETDATAACCESSOBJECT_H__

#include "ScapeEnginePrerequisites.h"

#include "SettingsDataset.h"

#include <string>

namespace ScapeEngine
{

class SettingsDatasetDataAccessObject
{
public:
    virtual ~SettingsDatasetDataAccessObject();

    virtual const SettingsDataset::SectionMapStruct getSettingsDataset(const std::string& datasetName) = 0;
    virtual void updateSettingsDataset(const SettingsDataset::SectionMapStruct& settingsDataset,
                                       const std::string& datasetName) = 0;

protected:
    SettingsDatasetDataAccessObject();
};
}

#endif /* __SETTINGSDATASETDATAACCESSOBJECT_H__ */

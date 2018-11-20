#ifndef __LEGACYSETTINGSDATASETDATAACCESSOBJECT_H__
#define __LEGACYSETTINGSDATASETDATAACCESSOBJECT_H__

#include "SettingsDatasetDataAccessObject.h"

#include <string>

namespace ScapeEngine
{

class LegacySettingsDatasetDataAccessObject : public SettingsDatasetDataAccessObject
{
public:
    LegacySettingsDatasetDataAccessObject(std::string fileName, bool saveAppendToFile = false);
    virtual ~LegacySettingsDatasetDataAccessObject();

    virtual const SettingsDataset::SectionMapStruct getSettingsDataset(const std::string& datasetName);
    virtual void updateSettingsDataset(const SettingsDataset::SectionMapStruct& settingsDataset,
                                       const std::string& datasetName);

protected:
    std::string mFileName;
    bool mSaveAppendToFile;
};
}

#endif /* __LEGACYSETTINGSDATASETDATAACCESSOBJECT_H__ */

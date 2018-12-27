#ifndef __QTJSONSETTINGSDATASETDATAACCESSOBJECT_H__
#define __QTJSONSETTINGSDATASETDATAACCESSOBJECT_H__

#include "ScapeEnginePrerequisites.h"

#include "SettingsDatasetDataAccessObject.h"

#include <string>
#include <vector>
#include <QJsonObject>

namespace ScapeEngine
{

class QtJSONSettingsDatasetDataAccessObject : public SettingsDatasetDataAccessObject
{
public:
    QtJSONSettingsDatasetDataAccessObject(std::string fileName);
    virtual ~QtJSONSettingsDatasetDataAccessObject();

    virtual const SettingsDataset::SectionMapStruct getSettingsDataset(const std::string& datasetName);
    virtual void updateSettingsDataset(const SettingsDataset::SectionMapStruct& settingsDataset,
                                       const std::string& datasetName);

protected:
    static void recurseObjects(QJsonObject& parent, SettingsDataset::SectionMapStruct& root,
                               std::vector<std::string> path);

    std::string mFileName;
};
}

#endif /* __QTJSONSETTINGSDATASETDATAACCESSOBJECT_H__ */

/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
#include "SettingsDataset.h"
#include "SettingsDatasetManager.h"
#include "EngineCore/QtJSONSettingsDatasetDataAccessObject.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
SettingsDatasetManager::~SettingsDatasetManager()
{
}

// ----------------------------------------------------------------------------
void SettingsDatasetManager::setDatasetResourcePath(const string& path)
{
    mDatasetResourcePath = Utils::emptyString;
    if (path.length() > 0)
    {
        mDatasetResourcePath = Ogre::StringUtil::standardisePath(path);
    }
}

// ----------------------------------------------------------------------------
SettingsDataset* SettingsDatasetManager::getDataset(const string& datasetName)
{
    DatasetMap::iterator datasetIt = mDatasetMap.find(datasetName);
    if (datasetIt == mDatasetMap.end())
    {
        datasetIt = mDatasetMap.insert(
            mDatasetMap.begin(), DatasetMap::value_type(datasetName, SettingsDataset(datasetName)));

        datasetIt->second.setSettingsDatasetDataAccessObject(
            getSettingsDatasetDataAccessObjectFromDatasetName(datasetName));
        datasetIt->second.load();
    }
    return &datasetIt->second;
}

// ----------------------------------------------------------------------------
string SettingsDatasetManager::getSetting(const string& datasetName, const string& section,
                                          const string& subsection, const string& key)
{
    return getDataset(datasetName)->getSetting(section, subsection, key);
}

// ----------------------------------------------------------------------------
void SettingsDatasetManager::setSetting(const string& datasetName, const string& section,
                                        const string& subsection, const string& key, const string& value)
{
    return getDataset(datasetName)->setSetting(section, subsection, key, value);
}

// ----------------------------------------------------------------------------
std::shared_ptr<SettingsDatasetDataAccessObject>
SettingsDatasetManager::getSettingsDatasetDataAccessObjectFromDatasetName(const std::string& dataset) const
{
    std::string path = dataset;
    if (dataset == "Startup")
    {
        path = "config";
    }
    path = mDatasetResourcePath + path + ".json";

    std::shared_ptr<SettingsDatasetDataAccessObject> dao =
        std::shared_ptr<SettingsDatasetDataAccessObject>(new QtJSONSettingsDatasetDataAccessObject(path));
    return dao;
}

// ----------------------------------------------------------------------------
void SettingsDatasetManager::saveAllDirty()
{
    DatasetMap::iterator datasetIt, datasetItEnd = mDatasetMap.end();
    for (datasetIt = mDatasetMap.begin(); datasetIt != datasetItEnd; ++datasetIt)
    {
        SettingsDataset& dataset = datasetIt->second;
        if (dataset.isDirty())
        {
            dataset.save();
        }
    }
}

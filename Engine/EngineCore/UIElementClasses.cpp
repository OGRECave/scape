/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "UIElementClasses.h"
#include "EngineCore/SettingsDatasetManager.h"
#include "EngineCore/SettingsDataset.h"
#include "EngineCore/QtJSONSettingsDatasetDataAccessObject.h"

#include <memory>
#include <map>

const string ScapeEngine::defaultPresetString = _T("Default");

using namespace ScapeEngine;

#define PRESET_DATASETNAME _T("Presets")

StringEnumMap& UIElementContainerSimple::getPersistentElementStringEnumMap()
{
    std::string containerName = getContainerName();

    static std::map<std::string, StringEnumMap> maps;
    std::map<std::string, StringEnumMap>::iterator it = maps.find(containerName);
    if (it == maps.end())
    {
        it = maps.insert(std::make_pair(containerName, StringEnumMap())).first;
        initPersistentElementStringEnumMap(it->second);
    }
    return it->second;
}

StringStringMap& UIElementContainerSimple::getPersistentElementValueMap()
{
    std::string containerName = getContainerName();

    static std::map<std::string, StringStringMap> maps;
    std::map<std::string, StringStringMap>::iterator it = maps.find(containerName);
    if (it == maps.end())
    {
        it = maps.insert(std::make_pair(containerName, StringStringMap())).first;
        initPersistentElementValueMap(it->second);
    }
    return it->second;
}
// ----------------------------------------------------------------------------
StringList UIElementContainerSimple::getUIElementPropertyFieldNameList()
{
    std::list<string> names;

    StringEnumMap::iterator elementIt, elementItEnd = getPersistentElementStringEnumMap().end();
    for (elementIt = getPersistentElementStringEnumMap().begin(); elementIt != elementItEnd; ++elementIt)
    {
        names.push_back(elementIt->first);
    }

    return names;
}

// ----------------------------------------------------------------------------
StringStringMap UIElementContainerSimple::setUIElementPropertyValueMap(const StringStringMap& valueMap)
{
    StringStringMap correctedValueMap;
    StringStringMap::const_iterator valueIt, valueItEnd = valueMap.end();
    for (valueIt = valueMap.begin(); valueIt != valueItEnd; ++valueIt)
    {
        string correctedValue = setUIElementPropertyValue(valueIt->first, valueIt->second);
        correctedValueMap.insert(StringStringMap::value_type(valueIt->first, correctedValue));
    }
    return correctedValueMap;
}

// ----------------------------------------------------------------------------
string UIElementContainerSimple::getUIElementPropertyFieldValue(const string& elementName)
{
    StringStringMap& valueMap = getPersistentElementValueMap();
    StringStringMap::iterator elementIt = valueMap.find(elementName);
    if (elementIt != valueMap.end())
    {
        return elementIt->second;
    }
    return Utils::emptyString;
}

// ----------------------------------------------------------------------------
string UIElementPresetContainerSimple::getUIElementPresetPropertyValue(const string& presetName,
                                                                       const string& elementName)
{
    return getEngineCore()->getSettingsDatasetManager()->getSetting(PRESET_DATASETNAME, getContainerName(),
                                                                    presetName, elementName);
}

// ----------------------------------------------------------------------------
void UIElementPresetContainerSimple::setUIElementPresetPropertyValue(const string& presetName,
                                                                     const string& elementName,
                                                                     const string& value)
{
    return getEngineCore()->getSettingsDatasetManager()->setSetting(PRESET_DATASETNAME, getContainerName(),
                                                                    presetName, elementName, value);
}

// ----------------------------------------------------------------------------
StringStringMap UIElementPresetContainerSimple::getUIElementPresetPropertyValueMap(const string& presetName)
{
    StringStringMap valueMap;

    StringEnumMap::iterator elementIt, elementItEnd = getPersistentElementStringEnumMap().end();
    for (elementIt = getPersistentElementStringEnumMap().begin(); elementIt != elementItEnd; ++elementIt)
    {
        valueMap[elementIt->first] = getUIElementPresetPropertyValue(presetName, elementIt->first);
    }
    return valueMap;
}

// ----------------------------------------------------------------------------
void UIElementPresetContainerSimple::setUIElementPresetPropertyValueMap(const string& presetName,
                                                                        const StringStringMap& valueMap)
{
    StringStringMap::const_iterator valueIt, valueItEnd = valueMap.end();
    for (valueIt = valueMap.begin(); valueIt != valueItEnd; ++valueIt)
    {
        setUIElementPresetPropertyValue(presetName, valueIt->first, valueIt->second);
    }
}

// ----------------------------------------------------------------------------
void UIElementPresetContainerSimple::deleteUIElementPreset(const string& presetName)
{
    SettingsDataset* dataset = getEngineCore()->getSettingsDatasetManager()->getDataset(PRESET_DATASETNAME);
    assert(dataset);
    return dataset->clear(getContainerName(), presetName);
}

// ----------------------------------------------------------------------------
StringList UIElementPresetContainerSimple::getUIElementPresetPropertyNames()
{
    SettingsDataset* dataset = getEngineCore()->getSettingsDatasetManager()->getDataset(PRESET_DATASETNAME);
    assert(dataset);

    StringList presetNames;

    const SettingsDataset::SectionMapStruct& sections = dataset->getSections();
    SettingsDataset::SectionMapStruct::const_iterator secIt = sections.find(getContainerName());
    if (secIt != sections.end())
    {
        const SettingsDataset::SubsectionMapStruct& subSections = secIt->second;
        for (SettingsDataset::SubsectionMapStruct::const_iterator subSectionIt = subSections.begin();
             subSectionIt != subSections.end(); subSectionIt++)
        {
            presetNames.push_back(subSectionIt->first);
        }
    }

    return presetNames;
}

// ----------------------------------------------------------------------------
string UIElementPresetContainerSimple::importUIElementPreset(const string& fileName)
{
    SettingsDataset* dataset = getEngineCore()->getSettingsDatasetManager()->getDataset(PRESET_DATASETNAME);
    assert(dataset);

    SettingsDataset importedDataset(PRESET_DATASETNAME);
    std::shared_ptr<SettingsDatasetDataAccessObject> dao = std::shared_ptr<SettingsDatasetDataAccessObject>(
        new QtJSONSettingsDatasetDataAccessObject(fileName));
    importedDataset.setSettingsDatasetDataAccessObject(dao);
    importedDataset.load();

    string presetName;

    const SettingsDataset::SectionMapStruct& sections = importedDataset.getSections();
    SettingsDataset::SectionMapStruct::const_iterator secIt = sections.find(getContainerName());
    if (secIt != sections.end())
    {
        const SettingsDataset::SubsectionMapStruct& subSections = secIt->second;
        for (SettingsDataset::SubsectionMapStruct::const_iterator subSectionIt = subSections.begin();
             subSectionIt != subSections.end(); subSectionIt++)
        {
            presetName = subSectionIt->first;
            presetName = makeUniquePresetName(presetName);

            const SettingsDataset::KeyMapStruct& keys = subSectionIt->second;
            for (SettingsDataset::KeyMapStruct::const_iterator keyIt = keys.begin(); keyIt != keys.end();
                 keyIt++)
            {
                dataset->setSetting(getContainerName(), presetName, keyIt->first, keyIt->second);
            }
        }
    }

    return presetName;
}

// ----------------------------------------------------------------------------
void UIElementPresetContainerSimple::exportUIElementPreset(const string& fileName)
{
    StringStringMap valueMap = getUIElementPropertyFieldValueMap();

    SettingsDataset exportedDataset(PRESET_DATASETNAME);

    string baseName, extension, path;
    Ogre::StringUtil::splitFullFilename(fileName, baseName, extension, path);

    string presetName = baseName;

    StringStringMap::const_iterator valueIt, valueItEnd = valueMap.end();
    for (valueIt = valueMap.begin(); valueIt != valueItEnd; ++valueIt)
    {
        exportedDataset.setSetting(getContainerName(), presetName, valueIt->first, valueIt->second);
    }

    std::shared_ptr<SettingsDatasetDataAccessObject> dao = std::shared_ptr<SettingsDatasetDataAccessObject>(
        new QtJSONSettingsDatasetDataAccessObject(fileName));
    exportedDataset.setSettingsDatasetDataAccessObject(dao);
    exportedDataset.save();
}

// ----------------------------------------------------------------------------
string UIElementPresetContainerSimple::makeUniquePresetName(const string& baseName)
{
    return Utils::makeUniqueName(baseName, getUIElementPresetPropertyNames());
}

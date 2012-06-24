/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "UIElementClasses.h"
#include "EngineCore/SettingsDatasetManager.h"
#include "EngineCore/SettingsDataset.h"

const string ScapeEngine::defaultPresetString = _T("Default");

using namespace ScapeEngine;

#define PRESET_DATASETNAME _T("Presets")


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
StringList UIElementPresetContainerSimple::getUIElementPropertyFieldNameList()
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
StringStringMap UIElementPresetContainerSimple::setUIElementPropertyValueMap(const StringStringMap& valueMap)
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
string UIElementPresetContainerSimple::getUIElementPropertyFieldValue(const string& elementName)
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
string UIElementPresetContainerSimple::getUIElementPresetPropertyValue(const string& presetName, const string& elementName)
{
	return getEngineCore()->getSettingsDatasetManager()->getSetting(PRESET_DATASETNAME, getContainerName(), presetName, elementName);
}


// ----------------------------------------------------------------------------
void UIElementPresetContainerSimple::setUIElementPresetPropertyValue(const string& presetName, const string& elementName, const string& value) 
{
	return getEngineCore()->getSettingsDatasetManager()->setSetting(PRESET_DATASETNAME, getContainerName(), presetName, elementName, value);
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
void UIElementPresetContainerSimple::setUIElementPresetPropertyValueMap(const string& presetName, const StringStringMap& valueMap)
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
	SettingsDataset::SubsectionIterator iterator = dataset->getSubsectionIterator(getContainerName());

	StringList presetNames;

	for (; !iterator.isEnd(); iterator.next())
	{
		presetNames.push_back(iterator.getKey());
	}

	return presetNames;
}


// ----------------------------------------------------------------------------
string UIElementPresetContainerSimple::importUIElementPreset(const string& fileName)
{
	SettingsDataset* dataset = getEngineCore()->getSettingsDatasetManager()->getDataset(PRESET_DATASETNAME);
	assert(dataset);

	SettingsDataset importedDataset(PRESET_DATASETNAME);
	importedDataset.load(fileName);
	SettingsDataset::SubsectionIterator subsectionIterator = importedDataset.getSubsectionIterator(getContainerName());

	string presetName;

	for (; !subsectionIterator.isEnd(); subsectionIterator.next())
	{
		presetName = subsectionIterator.getKey();
		presetName = makeUniquePresetName(presetName);

		SettingsDataset::KeyIterator keyIterator = subsectionIterator.getValue();
		
		for (; !keyIterator.isEnd(); keyIterator.next())
		{
			dataset->setSetting(getContainerName(), presetName, keyIterator.getKey(), keyIterator.getValue());
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

	exportedDataset.save(fileName);
}


// ----------------------------------------------------------------------------
string UIElementPresetContainerSimple::makeUniquePresetName(const string& baseName)
{
	return Utils::makeUniqueName(baseName, getUIElementPresetPropertyNames());
}

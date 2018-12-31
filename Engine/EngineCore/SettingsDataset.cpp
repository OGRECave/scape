/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "SettingsDataset.h"
#include "SettingsDatasetDataAccessObject.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
SettingsDataset::SettingsDataset(const std::string& datasetName)
    : mDatasetName(datasetName), mIsDirty(false), mSectionMapStruct()
{
}

// ----------------------------------------------------------------------------
SettingsDataset::~SettingsDataset() {}

const std::string& SettingsDataset::getDatasetName() { return mDatasetName; }

bool SettingsDataset::isDirty() { return mIsDirty; }

std::shared_ptr<SettingsDatasetDataAccessObject> SettingsDataset::getSettingsDatasetDataAccessObject() const
{
    return mSettingsDatasetDataAccessObject;
}

void SettingsDataset::setSettingsDatasetDataAccessObject(
    std::shared_ptr<SettingsDatasetDataAccessObject> settingsDatasetDataAccessObject)
{
    mSettingsDatasetDataAccessObject = settingsDatasetDataAccessObject;
}

const SettingsDataset::SectionMapStruct& SettingsDataset::getSections() { return mSectionMapStruct; }

// ----------------------------------------------------------------------------
std::string SettingsDataset::getSetting(const std::string& section, const std::string& subsection,
                                        const std::string& key) const
{
    SectionMapStruct::const_iterator sectionIt = mSectionMapStruct.find(section);
    if (sectionIt != mSectionMapStruct.end())
    {
        SubsectionMapStruct::const_iterator subsectionIt = sectionIt->second.find(subsection);
        if (subsectionIt != sectionIt->second.end())
        {
            KeyMapStruct::const_iterator keyIt = subsectionIt->second.find(key);
            if (keyIt != subsectionIt->second.end())
            {
                return keyIt->second;
            }
        }
    }
    return Utils::emptyString;
}

// ----------------------------------------------------------------------------
void SettingsDataset::setSetting(const std::string& section, const std::string& subsection,
                                 const std::string& key, const std::string& value)
{
    SectionMapStruct::iterator sectionIt = mSectionMapStruct.find(section);
    if (sectionIt == mSectionMapStruct.end())
    {
        sectionIt = mSectionMapStruct.insert(mSectionMapStruct.begin(),
                                             SectionMapStruct::value_type(section, SubsectionMapStruct()));
    }
    SubsectionMapStruct& subsectionMapStruct = sectionIt->second;

    SubsectionMapStruct::iterator subsectionIt = subsectionMapStruct.find(subsection);
    if (subsectionIt == subsectionMapStruct.end())
    {
        subsectionIt = subsectionMapStruct.insert(
            subsectionMapStruct.begin(), SubsectionMapStruct::value_type(subsection, KeyMapStruct()));
    }
    KeyMapStruct& keyMapStruct = subsectionIt->second;

    KeyMapStruct::iterator keyIt = keyMapStruct.find(key);
    if (keyIt == keyMapStruct.end())
    {
        keyMapStruct.insert(KeyMapStruct::value_type(key, value));
    }
    else
    {
        keyIt->second = value;
    }

    mIsDirty = true;
}

// ----------------------------------------------------------------------------
void SettingsDataset::clear()
{
    mSectionMapStruct.clear();
    mIsDirty = true;
}

// ----------------------------------------------------------------------------
void SettingsDataset::clear(const std::string& section)
{
    SectionMapStruct::iterator sectionIt = mSectionMapStruct.find(section);
    if (sectionIt != mSectionMapStruct.end())
    {
        mSectionMapStruct.erase(sectionIt);
        mIsDirty = true;
    }
}

// ----------------------------------------------------------------------------
void SettingsDataset::clear(const std::string& section, const std::string& subsection)
{
    SectionMapStruct::iterator sectionIt = mSectionMapStruct.find(section);
    if (sectionIt != mSectionMapStruct.end())
    {
        SubsectionMapStruct& subsectionMapStruct = sectionIt->second;
        SubsectionMapStruct::iterator subsectionIt = subsectionMapStruct.find(subsection);
        if (subsectionIt != subsectionMapStruct.end())
        {
            subsectionMapStruct.erase(subsectionIt);
            mIsDirty = true;
        }
    }
}

// ----------------------------------------------------------------------------
void SettingsDataset::clear(const std::string& section, const std::string& subsection,
                            const std::string& key)
{
    SectionMapStruct::iterator sectionIt = mSectionMapStruct.find(section);
    if (sectionIt != mSectionMapStruct.end())
    {
        SubsectionMapStruct& subsectionMapStruct = sectionIt->second;
        SubsectionMapStruct::iterator subsectionIt = subsectionMapStruct.find(subsection);
        if (subsectionIt != subsectionMapStruct.end())
        {
            KeyMapStruct& keyMapStruct = subsectionIt->second;
            KeyMapStruct::iterator keyIt = keyMapStruct.find(key);
            if (keyIt != keyMapStruct.end())
            {
                keyMapStruct.erase(keyIt);
                mIsDirty = true;
            }
        }
    }
}

void SettingsDataset::load(bool appendSettings)
{
    if (mSettingsDatasetDataAccessObject)
    {
        const SettingsDataset::SectionMapStruct data =
            mSettingsDatasetDataAccessObject->getSettingsDataset(mDatasetName);
        if (!appendSettings)
        {
            clear();
        }

        mSectionMapStruct.insert(data.begin(), data.end());

        mIsDirty = appendSettings;
    }
}

void SettingsDataset::save()
{
    if (mSettingsDatasetDataAccessObject)
    {
        mSettingsDatasetDataAccessObject->updateSettingsDataset(mSectionMapStruct, mDatasetName);
        mIsDirty = false;
    }
}

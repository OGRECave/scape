/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
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
void SettingsDataset::clear() {
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

// ----------------------------------------------------------------------------
bool SettingsDataset::load(const std::string& fileName, bool appendSettings)
{
    Ogre::LogManager::getSingleton().logMessage(_T("Loading dataset from '") + fileName + _T("'"));

    std::ifstream fp;
    fp.open(fileName.c_str(), std::ios::in | std::ios::binary);

    if (!fp)
    {
        return false;
    }

    Ogre::DataStreamPtr stream(new Ogre::FileStreamDataStream(fileName, &fp, false));

    if (!appendSettings)
    {
        clear();
    }

    std::string datasetName = Utils::emptyString;
    std::string sectionName = Utils::emptyString;
    string subsectionName = Utils::emptyString;
    bool correctDataset = false;
    SubsectionMapStruct* subsectionMapStruct = NULL;
    KeyMapStruct* keyMapStruct = NULL;

    std::string line, key, value;
    while (!stream->eof())
    {
        line = stream->getLine();
        size_t lineLength = line.length();
        if (lineLength > 0)
        {
            if (line.at(0) == _T(':'))
            {
                if (lineLength > 1 && line.at(1) == _T(':'))
                {
                    if (correctDataset)
                    {
                        if (lineLength > 2 && line.at(2) == _T(':'))
                        {
                            subsectionName = line.substr(3);
                        }
                        else
                        {
                            sectionName = line.substr(2);
                            subsectionMapStruct = NULL;
                        }
                        keyMapStruct = NULL;
                    }
                }
                else
                {
                    datasetName = line.substr(1);
                    correctDataset = (mDatasetName.compare(datasetName) == 0);
                }
            }
            else if (correctDataset)
            {
                std::string::size_type separator_pos = line.find_first_of(_T(':'));
                if (separator_pos != std::string::npos)
                {
                    key = line.substr(0, separator_pos);
                    value = line.substr(separator_pos + 1);

                    if (!subsectionMapStruct)
                    {
                        SectionMapStruct::iterator sectionIt = mSectionMapStruct.find(sectionName);
                        if (sectionIt == mSectionMapStruct.end())
                        {
                            sectionIt = mSectionMapStruct.insert(
                                mSectionMapStruct.begin(),
                                SectionMapStruct::value_type(sectionName, SubsectionMapStruct()));
                        }
                        subsectionMapStruct = &sectionIt->second;
                    }

                    if (!keyMapStruct)
                    {
                        SubsectionMapStruct::iterator subsectionIt =
                            subsectionMapStruct->find(subsectionName);
                        if (subsectionIt == subsectionMapStruct->end())
                        {
                            subsectionIt = subsectionMapStruct->insert(
                                subsectionMapStruct->begin(),
                                SubsectionMapStruct::value_type(subsectionName, KeyMapStruct()));
                        }
                        keyMapStruct = &subsectionIt->second;
                    }

                    KeyMapStruct::iterator keyIt = keyMapStruct->find(key);
                    if (keyIt == keyMapStruct->end())
                    {
                        keyMapStruct->insert(KeyMapStruct::value_type(key, value));
                    }
                    else
                    {
                        keyIt->second = value;
                    }
                    mIsDirty = true;
                }
            }
        }
    }

    fp.close();

    if (!appendSettings)
    {
        mIsDirty = false;
    }

    return true;
}

// ----------------------------------------------------------------------------
bool SettingsDataset::save(const std::string& fileName, bool appendFile)
{
    Ogre::LogManager::getSingleton().logMessage(_T("Saving dataset to '") + fileName + _T("'"));

    std::ofstream fp;
    fp.open(fileName.c_str(), (appendFile ? std::ios::app : std::ios::out) | std::ios::binary);

    if (!fp)
    {
        return false;
    }

    std::string lineFeed = _T("\n");
    std::string datasetNamePrefix = _T(":");
    std::string sectionNamePrefix = _T("::");
    std::string subsectionNamePrefix = _T(":::");
    std::string assignmentInfix = _T(":");

    bool writeDatasetName = true;
    assert(mDatasetName.length() == 0 || mDatasetName.at(0) != _T(':'));
    SectionMapStruct::iterator sectionIt, sectionItEnd = mSectionMapStruct.end();
    for (sectionIt = mSectionMapStruct.begin(); sectionIt != sectionItEnd; ++sectionIt)
    {
        bool writeSectionName = true;
        assert(mDatasetName.length() == 0 || sectionIt->first.at(0) != _T(':'));
        SubsectionMapStruct& subsectionMapStruct = sectionIt->second;
        SubsectionMapStruct::iterator subsectionIt, subsectionItEnd = subsectionMapStruct.end();
        for (subsectionIt = subsectionMapStruct.begin(); subsectionIt != subsectionItEnd; ++subsectionIt)
        {
            bool writeSubsectionName = true;
            KeyMapStruct& keyMapStruct = subsectionIt->second;
            KeyMapStruct::iterator keyIt, keyItEnd = keyMapStruct.end();
            for (keyIt = keyMapStruct.begin(); keyIt != keyItEnd; ++keyIt)
            {
                if (writeSubsectionName)
                {
                    if (writeSectionName)
                    {
                        if (writeDatasetName)
                        {
                            fp << lineFeed << datasetNamePrefix << mDatasetName << lineFeed;
                            writeDatasetName = false;
                        }
                        fp << lineFeed << sectionNamePrefix << sectionIt->first << lineFeed;
                        writeSectionName = false;
                    }
                    fp << lineFeed << subsectionNamePrefix << subsectionIt->first << lineFeed;
                    writeSubsectionName = false;
                }
                assert(keyIt->first.length() &&
                       keyIt->first.find_first_of(assignmentInfix) == std::string::npos);
                fp << keyIt->first << assignmentInfix << keyIt->second << lineFeed;
            }
        }
    }

    fp.close();

    mIsDirty = false;

    return true;
}

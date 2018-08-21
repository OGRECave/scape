/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "SettingsDataset.h"
#include "PCH/stdafx.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
SettingsDataset::SettingsDataset(const string& datasetName) : mDatasetName(datasetName), mIsDirty(false)
{
    mSectionMapStruct = new SectionMapStruct();
}

// ----------------------------------------------------------------------------
SettingsDataset::~SettingsDataset()
{
    SectionMapStruct::MapType::iterator sectionIt, sectionItEnd = mSectionMapStruct->map.end();
    for (sectionIt = mSectionMapStruct->map.begin(); sectionIt != sectionItEnd; ++sectionIt)
    {
        SubsectionMapStruct* subsectionMapStruct = sectionIt->second;
        SubsectionMapStruct::MapType::iterator subsectionIt,
            subsectionItEnd = subsectionMapStruct->map.end();
        for (subsectionIt = subsectionMapStruct->map.begin(); subsectionIt != subsectionItEnd;
             ++subsectionIt)
        {
            KeyMapStruct* keyMapStruct = subsectionIt->second;
            KeyMapStruct::MapType::iterator keyIt, keyItEnd = keyMapStruct->map.end();
            for (keyIt = keyMapStruct->map.begin(); keyIt != keyItEnd; ++keyIt)
            {
                string* key = keyIt->second;
                delete key;
            }
            delete keyMapStruct;
        }
        delete subsectionMapStruct;
    }
    delete mSectionMapStruct;
}

// ----------------------------------------------------------------------------
SettingsDataset::SectionIterator SettingsDataset::getSectionIterator()
{
    return SectionIterator(*mSectionMapStruct);
}

// ----------------------------------------------------------------------------
SettingsDataset::SubsectionIterator SettingsDataset::getSubsectionIterator(const string& section)
{
    SectionMapStruct::MapType::const_iterator sectionIt = mSectionMapStruct->map.find(section);
    if (sectionIt != mSectionMapStruct->map.end())
    {
        return SubsectionIterator(*sectionIt->second);
    }
    return SubsectionIterator();
}

// ----------------------------------------------------------------------------
SettingsDataset::KeyIterator SettingsDataset::getKeyIterator(const string& section,
                                                             const string& subsection)
{
    SectionMapStruct::MapType::const_iterator sectionIt = mSectionMapStruct->map.find(section);
    if (sectionIt != mSectionMapStruct->map.end())
    {
        SubsectionMapStruct::MapType::const_iterator subsectionIt = sectionIt->second->map.find(subsection);
        if (subsectionIt != sectionIt->second->map.end())
        {
            return KeyIterator(*subsectionIt->second);
        }
    }
    return KeyIterator();
}

// ----------------------------------------------------------------------------
string SettingsDataset::getSetting(const string& section, const string& subsection, const string& key) const
{
    SectionMapStruct::MapType::const_iterator sectionIt = mSectionMapStruct->map.find(section);
    if (sectionIt != mSectionMapStruct->map.end())
    {
        SubsectionMapStruct::MapType::const_iterator subsectionIt = sectionIt->second->map.find(subsection);
        if (subsectionIt != sectionIt->second->map.end())
        {
            KeyMapStruct::MapType::const_iterator keyIt = subsectionIt->second->map.find(key);
            if (keyIt != subsectionIt->second->map.end())
            {
                return *keyIt->second;
            }
        }
    }
    return Utils::emptyString;
}

// ----------------------------------------------------------------------------
void SettingsDataset::setSetting(const string& section, const string& subsection, const string& key,
                                 const string& value)
{
    SectionMapStruct::MapType::iterator sectionIt = mSectionMapStruct->map.find(section);
    if (sectionIt == mSectionMapStruct->map.end())
    {
        sectionIt = mSectionMapStruct->map.insert(
            mSectionMapStruct->map.begin(),
            SectionMapStruct::MapType::value_type(section, new SubsectionMapStruct()));
    }
    SubsectionMapStruct* subsectionMapStruct = sectionIt->second;

    SubsectionMapStruct::MapType::iterator subsectionIt = subsectionMapStruct->map.find(subsection);
    if (subsectionIt == subsectionMapStruct->map.end())
    {
        subsectionIt = subsectionMapStruct->map.insert(
            subsectionMapStruct->map.begin(),
            SubsectionMapStruct::MapType::value_type(subsection, new KeyMapStruct()));
    }
    KeyMapStruct* keyMapStruct = subsectionIt->second;

    KeyMapStruct::MapType::iterator keyIt = keyMapStruct->map.find(key);
    if (keyIt == keyMapStruct->map.end())
    {
        keyMapStruct->map.insert(KeyMapStruct::MapType::value_type(key, new string(value)));
    }
    else
    {
        delete keyIt->second;
        keyIt->second = new string(value);
    }

    mIsDirty = true;
}

// ----------------------------------------------------------------------------
void SettingsDataset::clear()
{
    SectionMapStruct::MapType::iterator sectionIt, sectionItEnd = mSectionMapStruct->map.end();
    for (sectionIt = mSectionMapStruct->map.begin(); sectionIt != sectionItEnd; ++sectionIt)
    {
        SubsectionMapStruct* subsectionMapStruct = sectionIt->second;
        SubsectionMapStruct::MapType::iterator subsectionIt,
            subsectionItEnd = subsectionMapStruct->map.end();
        for (subsectionIt = subsectionMapStruct->map.begin(); subsectionIt != subsectionItEnd;
             ++subsectionIt)
        {
            KeyMapStruct* keyMapStruct = subsectionIt->second;
            KeyMapStruct::MapType::iterator keyIt, keyItEnd = keyMapStruct->map.end();
            for (keyIt = keyMapStruct->map.begin(); keyIt != keyItEnd; ++keyIt)
            {
                string* key = keyIt->second;
                delete key;
                mIsDirty = true;
            }
            delete keyMapStruct;
        }
        delete subsectionMapStruct;
    }
    mSectionMapStruct->map.clear();
}

// ----------------------------------------------------------------------------
void SettingsDataset::clear(const string& section)
{
    SectionMapStruct::MapType::iterator sectionIt = mSectionMapStruct->map.find(section);
    if (sectionIt != mSectionMapStruct->map.end())
    {
        SubsectionMapStruct* subsectionMapStruct = sectionIt->second;
        SubsectionMapStruct::MapType::iterator subsectionIt,
            subsectionItEnd = subsectionMapStruct->map.end();
        for (subsectionIt = subsectionMapStruct->map.begin(); subsectionIt != subsectionItEnd;
             ++subsectionIt)
        {
            KeyMapStruct* keyMapStruct = subsectionIt->second;
            KeyMapStruct::MapType::iterator keyIt, keyItEnd = keyMapStruct->map.end();
            for (keyIt = keyMapStruct->map.begin(); keyIt != keyItEnd; ++keyIt)
            {
                string* key = keyIt->second;
                delete key;
                mIsDirty = true;
            }
            delete keyMapStruct;
        }
        delete subsectionMapStruct;
        mSectionMapStruct->map.erase(sectionIt);
    }
}

// ----------------------------------------------------------------------------
void SettingsDataset::clear(const string& section, const string& subsection)
{
    SectionMapStruct::MapType::iterator sectionIt = mSectionMapStruct->map.find(section);
    if (sectionIt != mSectionMapStruct->map.end())
    {
        SubsectionMapStruct* subsectionMapStruct = sectionIt->second;
        SubsectionMapStruct::MapType::iterator subsectionIt = subsectionMapStruct->map.find(subsection);
        if (subsectionIt != subsectionMapStruct->map.end())
        {
            KeyMapStruct* keyMapStruct = subsectionIt->second;
            KeyMapStruct::MapType::iterator keyIt, keyItEnd = keyMapStruct->map.end();
            for (keyIt = keyMapStruct->map.begin(); keyIt != keyItEnd; ++keyIt)
            {
                string* key = keyIt->second;
                delete key;
                mIsDirty = true;
            }
            delete keyMapStruct;
            subsectionMapStruct->map.erase(subsectionIt);
        }
    }
}

// ----------------------------------------------------------------------------
void SettingsDataset::clear(const string& section, const string& subsection, const string& key)
{
    SectionMapStruct::MapType::iterator sectionIt = mSectionMapStruct->map.find(section);
    if (sectionIt != mSectionMapStruct->map.end())
    {
        SubsectionMapStruct* subsectionMapStruct = sectionIt->second;
        SubsectionMapStruct::MapType::iterator subsectionIt = subsectionMapStruct->map.find(subsection);
        if (subsectionIt != subsectionMapStruct->map.end())
        {
            KeyMapStruct* keyMapStruct = subsectionIt->second;
            KeyMapStruct::MapType::iterator keyIt = keyMapStruct->map.find(key);
            if (keyIt != keyMapStruct->map.end())
            {
                string* key = keyIt->second;
                delete key;
                keyMapStruct->map.erase(keyIt);
                mIsDirty = true;
            }
        }
    }
}

// ----------------------------------------------------------------------------
bool SettingsDataset::load(const string& fileName, bool appendSettings)
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

    string datasetName = Utils::emptyString;
    string sectionName = Utils::emptyString;
    string subsectionName = Utils::emptyString;
    bool correctDataset = false;
    SubsectionMapStruct* subsectionMapStruct = NULL;
    KeyMapStruct* keyMapStruct = NULL;

    string line, key, value;
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
                        SectionMapStruct::MapType::iterator sectionIt =
                            mSectionMapStruct->map.find(sectionName);
                        if (sectionIt == mSectionMapStruct->map.end())
                        {
                            sectionIt =
                                mSectionMapStruct->map.insert(mSectionMapStruct->map.begin(),
                                                              SectionMapStruct::MapType::value_type(
                                                                  sectionName, new SubsectionMapStruct()));
                        }
                        subsectionMapStruct = sectionIt->second;
                    }

                    if (!keyMapStruct)
                    {
                        SubsectionMapStruct::MapType::iterator subsectionIt =
                            subsectionMapStruct->map.find(subsectionName);
                        if (subsectionIt == subsectionMapStruct->map.end())
                        {
                            subsectionIt = subsectionMapStruct->map.insert(
                                subsectionMapStruct->map.begin(), SubsectionMapStruct::MapType::value_type(
                                                                      subsectionName, new KeyMapStruct()));
                        }
                        keyMapStruct = subsectionIt->second;
                    }

                    KeyMapStruct::MapType::iterator keyIt = keyMapStruct->map.find(key);
                    if (keyIt == keyMapStruct->map.end())
                    {
                        keyMapStruct->map.insert(KeyMapStruct::MapType::value_type(key, new string(value)));
                    }
                    else
                    {
                        delete keyIt->second;
                        keyIt->second = new string(value);
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
bool SettingsDataset::save(const string& fileName, bool appendFile)
{
    Ogre::LogManager::getSingleton().logMessage(_T("Saving dataset to '") + fileName + _T("'"));

    std::ofstream fp;
    fp.open(fileName.c_str(), (appendFile ? std::ios::app : std::ios::out) | std::ios::binary);

    if (!fp)
    {
        return false;
    }

    string lineFeed = _T("\n");
    string datasetNamePrefix = _T(":");
    string sectionNamePrefix = _T("::");
    string subsectionNamePrefix = _T(":::");
    string assignmentInfix = _T(":");

    bool writeDatasetName = true;
    assert(mDatasetName.length() == 0 || mDatasetName.at(0) != _T(':'));
    SectionMapStruct::MapType::iterator sectionIt, sectionItEnd = mSectionMapStruct->map.end();
    for (sectionIt = mSectionMapStruct->map.begin(); sectionIt != sectionItEnd; ++sectionIt)
    {
        bool writeSectionName = true;
        assert(mDatasetName.length() == 0 || sectionIt->first.at(0) != _T(':'));
        SubsectionMapStruct* subsectionMapStruct = sectionIt->second;
        SubsectionMapStruct::MapType::iterator subsectionIt,
            subsectionItEnd = subsectionMapStruct->map.end();
        for (subsectionIt = subsectionMapStruct->map.begin(); subsectionIt != subsectionItEnd;
             ++subsectionIt)
        {
            bool writeSubsectionName = true;
            KeyMapStruct* keyMapStruct = subsectionIt->second;
            KeyMapStruct::MapType::iterator keyIt, keyItEnd = keyMapStruct->map.end();
            for (keyIt = keyMapStruct->map.begin(); keyIt != keyItEnd; ++keyIt)
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
                fp << keyIt->first << assignmentInfix << *keyIt->second << lineFeed;
            }
        }
    }

    fp.close();

    mIsDirty = false;

    return true;
}

/*
/*
// ----------------------------------------------------------------------------
void SettingsDataset::setDatasetPath(const string& datasetPath)
{
        mDatasetPath = Utils::emptyString;
        if (datasetPath.length() > 0)
        {
                mDatasetPath = Ogre::StringUtil::standardisePath(datasetPath);
        }
}

// ----------------------------------------------------------------------------
void SettingsDataset::save(const string& dataset)
{
        SettingsFile* settingsFile = getSettingsFile(dataset);
        settingsFile->save(getSettingsFileName(dataset), false);
}
*/

/*

// ----------------------------------------------------------------------------
SettingsFile* SettingsDataset::getSettingsFile(const string& dataset)
{
        SettingsFileMap::iterator settingsFileMapIt = mSettingsFileMap.find(dataset);
        SettingsFile* settingsFile = NULL;

        if (settingsFileMapIt == mSettingsFileMap.end())
        {
                settingsFile = new SettingsFile();
                mSettingsFileMap[dataset] = settingsFile;
                settingsFile->load(getSettingsFileName(dataset), false);
        }
        else
        {
                settingsFile = settingsFileMapIt->second;
        }
        return settingsFile;
}
*/

/*
// ----------------------------------------------------------------------------
string SettingsDataset::getSettingsFileName(const string& dataset)
{
        return mResourcePath + dataset + _T(".settings");
}
*/
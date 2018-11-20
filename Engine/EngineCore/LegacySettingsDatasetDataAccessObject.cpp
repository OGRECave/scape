#include "LegacySettingsDatasetDataAccessObject.h"

#include "PCH/stdafx.h"

namespace ScapeEngine
{

LegacySettingsDatasetDataAccessObject::LegacySettingsDatasetDataAccessObject(std::string fileName,
                                                                             bool saveAppendToFile)
    : mFileName(fileName), mSaveAppendToFile(saveAppendToFile)
{
}

LegacySettingsDatasetDataAccessObject::~LegacySettingsDatasetDataAccessObject() {}

const SettingsDataset::SectionMapStruct
LegacySettingsDatasetDataAccessObject::getSettingsDataset(const std::string& datasetNameIn)
{
    SettingsDataset::SectionMapStruct ret;

    Ogre::LogManager::getSingleton().logMessage(_T("Loading dataset from '") + mFileName + _T("'"));

    std::ifstream fp;
    fp.open(mFileName.c_str(), std::ios::in | std::ios::binary);

    if (!fp)
    {
        return ret;
    }

    Ogre::DataStreamPtr stream(new Ogre::FileStreamDataStream(mFileName, &fp, false));

    std::string datasetName = Utils::emptyString;
    std::string sectionName = Utils::emptyString;
    string subsectionName = Utils::emptyString;
    bool correctDataset = false;
    SettingsDataset::SubsectionMapStruct* subsectionMapStruct = NULL;
    SettingsDataset::KeyMapStruct* keyMapStruct = NULL;

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
                    correctDataset = (datasetNameIn.compare(datasetName) == 0);
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
                        SettingsDataset::SectionMapStruct::iterator sectionIt = ret.find(sectionName);
                        if (sectionIt == ret.end())
                        {
                            sectionIt = ret.insert(
                                ret.begin(), SettingsDataset::SectionMapStruct::value_type(
                                                 sectionName, SettingsDataset::SubsectionMapStruct()));
                        }
                        subsectionMapStruct = &sectionIt->second;
                    }

                    if (!keyMapStruct)
                    {
                        SettingsDataset::SubsectionMapStruct::iterator subsectionIt =
                            subsectionMapStruct->find(subsectionName);
                        if (subsectionIt == subsectionMapStruct->end())
                        {
                            subsectionIt = subsectionMapStruct->insert(
                                subsectionMapStruct->begin(),
                                SettingsDataset::SubsectionMapStruct::value_type(
                                    subsectionName, SettingsDataset::KeyMapStruct()));
                        }
                        keyMapStruct = &subsectionIt->second;
                    }

                    SettingsDataset::KeyMapStruct::iterator keyIt = keyMapStruct->find(key);
                    if (keyIt == keyMapStruct->end())
                    {
                        keyMapStruct->insert(SettingsDataset::KeyMapStruct::value_type(key, value));
                    }
                    else
                    {
                        keyIt->second = value;
                    }
                }
            }
        }
    }

    fp.close();

    return ret;
}

void LegacySettingsDatasetDataAccessObject::updateSettingsDataset(
    const SettingsDataset::SectionMapStruct& settingsDataset, const std::string& datasetNameIn)
{
    Ogre::LogManager::getSingleton().logMessage(_T("Saving dataset to '") + mFileName + _T("'"));

    std::ofstream fp;
    fp.open(mFileName.c_str(), (mSaveAppendToFile ? std::ios::app : std::ios::out) | std::ios::binary);

    if (!fp)
    {
        return;
    }

    std::string lineFeed = _T("\n");
    std::string datasetNamePrefix = _T(":");
    std::string sectionNamePrefix = _T("::");
    std::string subsectionNamePrefix = _T(":::");
    std::string assignmentInfix = _T(":");

    bool writeDatasetName = true;
    assert(datasetNameIn.length() == 0 || datasetNameIn.at(0) != _T(':'));
    SettingsDataset::SectionMapStruct::const_iterator sectionIt, sectionItEnd = settingsDataset.end();
    for (sectionIt = settingsDataset.begin(); sectionIt != sectionItEnd; ++sectionIt)
    {
        bool writeSectionName = true;
        assert(datasetNameIn.length() == 0 || sectionIt->first.at(0) != _T(':'));
        const SettingsDataset::SubsectionMapStruct& subsectionMapStruct = sectionIt->second;
        SettingsDataset::SubsectionMapStruct::const_iterator subsectionIt,
            subsectionItEnd = subsectionMapStruct.end();
        for (subsectionIt = subsectionMapStruct.begin(); subsectionIt != subsectionItEnd; ++subsectionIt)
        {
            bool writeSubsectionName = true;
            const SettingsDataset::KeyMapStruct& keyMapStruct = subsectionIt->second;
            SettingsDataset::KeyMapStruct::const_iterator keyIt, keyItEnd = keyMapStruct.end();
            for (keyIt = keyMapStruct.begin(); keyIt != keyItEnd; ++keyIt)
            {
                if (writeSubsectionName)
                {
                    if (writeSectionName)
                    {
                        if (writeDatasetName)
                        {
                            fp << lineFeed << datasetNamePrefix << datasetNameIn << lineFeed;
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

    return;
}
}

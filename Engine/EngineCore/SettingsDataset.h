/**
 * ScapeEngine::SettingsDataset class
 *
 * Manages loading, retrieving and saving of settings in a section-section-subsection structure
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __SETTINGSDATASET_H__
#define __SETTINGSDATASET_H__

#include <string>
#include <map>

namespace ScapeEngine
{
class SettingsDataset
{
public:
    typedef std::map<std::string, std::string> KeyMapStruct;
    typedef std::map<std::string, KeyMapStruct> SubsectionMapStruct;
    typedef std::map<std::string, SubsectionMapStruct> SectionMapStruct;

    SettingsDataset(const std::string& datasetName);
    ~SettingsDataset();

    const std::string& getDatasetName();

    bool isDirty();

    const SectionMapStruct& getSections();

    std::string getSetting(const std::string& section, const std::string& subsection,
                           const std::string& key) const;
    void setSetting(const std::string& section, const std::string& subsection, const std::string& key,
                    const std::string& value);

    void clear();
    void clear(const std::string& section);
    void clear(const std::string& section, const std::string& subsection);
    void clear(const std::string& section, const std::string& subsection, const std::string& key);

    bool load(const std::string& fileName, bool appendSettings = false);
    bool save(const std::string& fileName, bool appendFile = false);

protected:
    std::string mDatasetName;
    bool mIsDirty;
    SectionMapStruct mSectionMapStruct;
};
}

#endif // __SETTINGSDATASET_H__

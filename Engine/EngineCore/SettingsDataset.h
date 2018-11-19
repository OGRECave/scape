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

namespace ScapeEngine
{
class SettingsDataset
{
public:
    typedef Utils::MapStruct<string, string*> KeyMapStruct;
    typedef Utils::MapStruct<string, KeyMapStruct*> SubsectionMapStruct;
    typedef Utils::MapStruct<string, SubsectionMapStruct*> SectionMapStruct;

    SettingsDataset(const string& datasetName);
    ~SettingsDataset();

    const string& getDatasetName();

    bool isDirty();

    const SectionMapStruct& getSections();

    string getSetting(const string& section, const string& subsection, const string& key) const;
    void setSetting(const string& section, const string& subsection, const string& key,
                    const string& value);

    void clear();
    void clear(const string& section);
    void clear(const string& section, const string& subsection);
    void clear(const string& section, const string& subsection, const string& key);

    bool load(const string& fileName, bool appendSettings = false);
    bool save(const string& fileName, bool appendFile = false);

protected:
    string mDatasetName;
    bool mIsDirty;
    SectionMapStruct* mSectionMapStruct;
};
}

#endif // __SETTINGSDATASET_H__

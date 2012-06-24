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
	class SettingsFile;

	class SettingsDataset
	{
	protected:
		typedef Utils::MapStruct<string, string*> KeyMapStruct;
		typedef Utils::MapStruct<string, KeyMapStruct*> SubsectionMapStruct;
		typedef Utils::MapStruct<string, SubsectionMapStruct*> SectionMapStruct;

	public:
		typedef Utils::MapStructReadIterator<KeyMapStruct, const string&> KeyIterator;
		typedef Utils::MapStructReadIterator<SubsectionMapStruct, KeyIterator> SubsectionIterator;
		typedef Utils::MapStructReadIterator<SectionMapStruct, SubsectionIterator> SectionIterator;

		SettingsDataset(const string& datasetName);
		~SettingsDataset();

		const string& getDatasetName() {return mDatasetName;}

		void clear();
		void clear(const string& section);
		void clear(const string& section, const string& subsection);
		void clear(const string& section, const string& subsection, const string& key);

		bool isDirty() {return mIsDirty;}
		void setDirty(bool dirty) {mIsDirty = dirty;}

		// None of the dataset, section and subsection names should begin with a colon.
		// Also, keys should not be empty or contain a colon.
		bool load(const string& fileName, bool appendSettings = false);
		bool save(const string& fileName, bool appendFile = false);

		SectionIterator getSectionIterator();
		SubsectionIterator getSubsectionIterator(const string& section);
		KeyIterator getKeyIterator(const string& section, const string& subsection);

		string getSetting(const string& section, const string& subsection, const string& key) const;
		void setSetting(const string& section, const string& subsection, const string& key, const string& value);

		/*
		void insert(const SectionIterator& sectionIterator);
		void insert(const string& section, const SubsectionIterator& subsectionIterator);
		void insert(const string& section, const string& subsection, const KeyIterator& keyIterator);
		*/



	protected:
		SectionMapStruct* mSectionMapStruct;
		string mDatasetName;
		bool mIsDirty;
	};

}

#endif // __SETTINGSDATASET_H__
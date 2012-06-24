/**
 * ScapeEngine::SettingsDataset class
 *
 * Manages loading, retrieving and storing different SettingsDataset datasets from a standard resource path
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __SETTINGSDATASETMANAGER_H__
#define __SETTINGSDATASETMANAGER_H__

namespace ScapeEngine
{
	class SettingsDataset;

	class SettingsDatasetManager
	{
	public:
		~SettingsDatasetManager();

		void setDatasetResourcePath(const string& path);

		SettingsDataset* getDataset(const string& datasetName) const;

		string getSetting(const string& datasetName, const string& section, const string& subsection, const string& key) const;

		void setSetting(const string& datasetName, const string& section, const string& subsection, const string& key, const string& value);

		void saveAllDirty();

	protected:
		typedef std::map<string, SettingsDataset*> DatasetMap;
		mutable DatasetMap mDatasetMap;
		string mDatasetResourcePath;

		string getPathFromDatasetName(const string& dataset) const;

	};
}

#endif // __SETTINGSDATASETMANAGER_H__
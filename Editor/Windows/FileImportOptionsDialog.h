/**
 * ScapeEditor::FileImportOptionsDialog class
 *
 * Shows a singlechoicedialog of decoders and a flipx and flipy option
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. F
 */

#ifndef __FILEIMPORTOPTIONSDIALOG_H__
#define __FILEIMPORTOPTIONSDIALOG_H__

#include "EngineInterface.h"
#include "PCH/stdafx.h"

namespace ScapeEditor
{
	// ----------------------------------------------------------------------------
	struct FileImportOptionsDialogItem
	{
		string type;
		string label;
		string description;
		string options;
	};

	typedef std::map<string, FileImportOptionsDialogItem> FileImportOptionsDialogItemMap;

	// ----------------------------------------------------------------------------
	class FileImportOptionsDialog : public wxDialog
	{
		DECLARE_CLASS(FileImportOptionsDialog)

	public:
		FileImportOptionsDialog(wxWindow* parentWindow, wxWindowID windowId, const wxString& caption, const wxString& fileName, size_t fileSize, const FileImportOptionsDialogItemMap& itemMap, const StringStringMap& valueMap);

        const ScapeEngine::StringStringMap& getValueMap();

	protected:
		wxCheckBox* mFlipXCheckBox;
		wxCheckBox* mFlipYCheckBox;
		wxTextCtrl* mWidthTextCtrl;
		wxTextCtrl* mHeightTextCtrl;
		wxRadioButton* mBigEndianButton;
		std::list<wxRadioButton*> mBppButtons;
		bool mDirtyValueMap;

		FileImportOptionsDialogItemMap mItemMap;
        ScapeEngine::StringStringMap mValueMap;

		DECLARE_EVENT_TABLE()
	};
}

#endif // __FILEIMPORTOPTIONSDIALOG_H__

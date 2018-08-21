/**
 * ScapeEditor::FileExportDialog class
 *
 * Shows a singlechoicedialog of encoders and a flipx and flipy option
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. F
 */

#ifndef __FILEEXPORTDIALOG_H__
#define __FILEEXPORTDIALOG_H__

#include "EngineInterface.h"

namespace ScapeEditor
{
// ----------------------------------------------------------------------------
struct FileExportItem
{
    string name;
    string label;
    string description;
};

typedef std::list<FileExportItem> FileExportItemList;

// ----------------------------------------------------------------------------
class FileExportDialog : public wxDialog
{
    DECLARE_CLASS(FileExportDialog)

public:
    FileExportDialog(wxWindow* parentWindow, wxWindowID windowId, const FileExportItemList& encoders,
                     const string& selectedEncoder, bool flipX, bool flipY);

    string getSelectionName() { return mSelectedItemName; }
    bool isFlipXChecked() { return mFlipXCheckBox->GetValue(); }
    bool isFlipYChecked() { return mFlipYCheckBox->GetValue(); }

protected:
    wxListBox* mListBox;
    wxTextCtrl* mDescriptionTextCtrl;
    wxCheckBox* mFlipXCheckBox;
    wxCheckBox* mFlipYCheckBox;

    FileExportItemList mEncoders;
    string mSelectedItemName;

    DECLARE_EVENT_TABLE()

    void onListBoxSelect(wxCommandEvent& event);
    void onListBoxDClick(wxCommandEvent& event);
};
}

#endif // __FILEEXPORTDIALOG_H__
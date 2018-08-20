/**
 * ScapeEditor::FileImportTypeDialog class
 *
 * Shows a singlechoicedialog of decoders and a flipx and flipy option
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. F
 */

#ifndef __FILEIMPORTTYPEDIALOG_H__
#define __FILEIMPORTTYPEDIALOG_H__

#include "EngineInterface.h"

namespace ScapeEditor {

    class FileImportTypeDialog : public wxDialog
    {
        DECLARE_CLASS(FileImportTypeDialog)

    public:
        FileImportTypeDialog(
            wxWindow* parentWindow, wxWindowID windowId, const StringList& decoders, const string& selectedEncoder);

        string getSelectionName() { return mSelectedItemName; }
    protected:
        wxListBox* mListBox;
        wxTextCtrl* mDescriptionTextCtrl;
        wxCheckBox* mFlipXCheckBox;
        wxCheckBox* mFlipYCheckBox;

        StringList mEncoders;
        string mSelectedItemName;

        DECLARE_EVENT_TABLE()

        void onListBoxSelect(wxCommandEvent& event);
        void onListBoxDClick(wxCommandEvent& event);
    };
}

#endif // __FILEIMPORTTYPEDIALOG_H__
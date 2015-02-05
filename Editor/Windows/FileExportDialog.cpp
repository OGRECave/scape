/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "FileExportDialog.h"

using namespace ScapeEditor;

// Required for WX
IMPLEMENT_CLASS(FileExportDialog, wxDialog)

// Required for WX
BEGIN_EVENT_TABLE(FileExportDialog, wxDialog)
	EVT_LISTBOX(wxID_ANY, FileExportDialog::onListBoxSelect)  
    EVT_LISTBOX_DCLICK(wxID_ANY, FileExportDialog::onListBoxDClick)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
FileExportDialog::FileExportDialog(wxWindow* parentWindow, wxWindowID windowId, const FileExportItemList& encoders, const string& selectedEncoder, bool flipX, bool flipY)
: wxDialog(parentWindow, 
		   windowId,
		   _T("File export settings"),
		   wxDefaultPosition,
		   wxDefaultSize,
		   wxDEFAULT_DIALOG_STYLE),
   mEncoders(encoders)
{
	wxString message(_T("Please select the file format to export to:"));

	wxArrayString encoderArray;	
	FileExportItemList::const_iterator encoderIt, encoderItEnd = mEncoders.end();
	for (encoderIt = mEncoders.begin(); encoderIt != encoderItEnd; ++encoderIt)
	{
		encoderArray.Add(encoderIt->label);
	}
 
	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

	// list description
    topSizer->Add(CreateTextSizer(message), wxSizerFlags().Expand().TripleBorder());

	// list
    mListBox = new wxListBox(
		this,
		wxID_ANY,                         
		wxDefaultPosition, 
		wxDefaultSize,
		encoderArray,
		wxLB_SINGLE | 
		wxLB_ALWAYS_SB);

    topSizer->Add(mListBox, wxSizerFlags().Expand().Proportion(1).TripleBorder(wxLEFT | wxRIGHT));

	// list item description
	mDescriptionTextCtrl = new wxTextCtrl(
		this, 
		wxID_ANY, 
        ScapeEngine::Utils::emptyString,
		wxDefaultPosition, 
		wxDefaultSize, 
		//wxSIMPLE_BORDER  | 
		wxTE_MULTILINE |
		wxTE_READONLY |
		wxTE_BESTWRAP);

	mDescriptionTextCtrl->SetBackgroundColour(GetBackgroundColour());

    topSizer->Add(mDescriptionTextCtrl, wxSizerFlags().Expand().Proportion(0).TripleBorder(wxLEFT | wxRIGHT));

	// flip X checkboxes
	mFlipXCheckBox = new wxCheckBox(this, wxID_ANY, _T("Horizontal flip"));
	mFlipXCheckBox->SetValue(flipX);
    topSizer->Add(mFlipXCheckBox, wxSizerFlags().Expand().Proportion(0).TripleBorder(wxLEFT | wxRIGHT));

	// flip Y checkboxes
	mFlipYCheckBox = new wxCheckBox(this, wxID_ANY, _T("Vertical flip"));
	mFlipYCheckBox->SetValue(flipY);
    topSizer->Add(mFlipYCheckBox, wxSizerFlags().Expand().Proportion(0).TripleBorder(wxLEFT | wxRIGHT));


	// OK/Cancel buttons
    wxSizer* buttonSizer = CreateSeparatedButtonSizer(wxOK | wxCANCEL);
    if (buttonSizer)
    {
        topSizer->Add(buttonSizer, wxSizerFlags().Expand().DoubleBorder());
    }

    SetSizer(topSizer);

    topSizer->SetSizeHints(this);
    topSizer->Fit(this);


	int index = 0;
	for (encoderIt = mEncoders.begin(); encoderIt != encoderItEnd; ++encoderIt, ++index)
	{
		if (encoderIt->label.compare(selectedEncoder) == 0)
		{
	        mListBox->SetSelection(index);
			mDescriptionTextCtrl->SetLabel(encoderIt->description); 
			break;
		}
	}
	mSelectedItemName = selectedEncoder;

    Centre(wxBOTH);

	mListBox->SetFocus();
}


// ----------------------------------------------------------------------------
void FileExportDialog::onListBoxSelect(wxCommandEvent& event)
{
	if (mDescriptionTextCtrl)
	{
		string selectedLabel(event.GetString());

		FileExportItemList::const_iterator encoderIt, encoderItEnd = mEncoders.end();
		for (encoderIt = mEncoders.begin(); encoderIt != encoderItEnd; ++encoderIt)
		{
			if (encoderIt->label.compare(selectedLabel) == 0) 
			{
				mDescriptionTextCtrl->SetLabel(encoderIt->description); 
				mSelectedItemName = encoderIt->name;
				break;
			}
		}
	}
}


// ----------------------------------------------------------------------------
void FileExportDialog::onListBoxDClick(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}



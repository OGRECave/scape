/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "FileImportTypeDialog.h"

using namespace ScapeEditor;

// Required for WX
IMPLEMENT_CLASS(FileImportTypeDialog, wxDialog)

// Required for WX
BEGIN_EVENT_TABLE(FileImportTypeDialog, wxDialog)
	EVT_LISTBOX(wxID_ANY, FileImportTypeDialog::onListBoxSelect)  
    EVT_LISTBOX_DCLICK(wxID_ANY, FileImportTypeDialog::onListBoxDClick)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
FileImportTypeDialog::FileImportTypeDialog(wxWindow* parentWindow, wxWindowID windowId, const StringList& decoders, const string& selectedEncoder)
: wxDialog(parentWindow, 
		   windowId,
		   _T("File export settings"),
		   wxDefaultPosition,
		   wxDefaultSize,
		   wxDEFAULT_DIALOG_STYLE),
   mEncoders(decoders)
{
	wxString message(_T("File extension was not recognized. Please select the decoder to use:"));

	wxArrayString decoderArray;	
	StringList::const_iterator decoderIt, decoderItEnd = mEncoders.end();
	for (decoderIt = mEncoders.begin(); decoderIt != decoderItEnd; ++decoderIt)
	{
		decoderArray.Add(*decoderIt);
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
		decoderArray,
		wxLB_SINGLE | 
		wxLB_ALWAYS_SB);

    topSizer->Add(mListBox, wxSizerFlags().Expand().Proportion(1).TripleBorder(wxLEFT | wxRIGHT));

	// list item description
	mDescriptionTextCtrl = new wxTextCtrl(
		this, 
		wxID_ANY, 
		Utils::emptyString, 
		wxDefaultPosition, 
		wxDefaultSize, 
		//wxSIMPLE_BORDER  | 
		wxTE_MULTILINE |
		wxTE_READONLY |
		wxTE_BESTWRAP);

	mDescriptionTextCtrl->SetBackgroundColour(GetBackgroundColour());

    topSizer->Add(mDescriptionTextCtrl, wxSizerFlags().Expand().Proportion(0).TripleBorder(wxLEFT | wxRIGHT));

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
	for (decoderIt = mEncoders.begin(); decoderIt != decoderItEnd; ++decoderIt, ++index)
	{
		if (decoderIt->compare(selectedEncoder) == 0)
		{
	        mListBox->SetSelection(index);
			mDescriptionTextCtrl->SetLabel(*decoderIt);
			break;
		}
	}
	mSelectedItemName = selectedEncoder;

    Centre(wxBOTH);

	mListBox->SetFocus();
}


// ----------------------------------------------------------------------------
void FileImportTypeDialog::onListBoxSelect(wxCommandEvent& event)
{
	if (mDescriptionTextCtrl)
	{
		string selectedLabel(event.GetString());

		StringList::const_iterator decoderIt, decoderItEnd = mEncoders.end();
		for (decoderIt = mEncoders.begin(); decoderIt != decoderItEnd; ++decoderIt)
		{
			if (decoderIt->compare(selectedLabel) == 0) 
			{
				mDescriptionTextCtrl->SetLabel(*decoderIt); 
				mSelectedItemName = *decoderIt;
				break;
			}
		}
	}
}


// ----------------------------------------------------------------------------
void FileImportTypeDialog::onListBoxDClick(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}



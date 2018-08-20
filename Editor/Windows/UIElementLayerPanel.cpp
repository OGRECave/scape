/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "UIElementLayerPanel.h"

using namespace ScapeEditor;

// Required for WX
IMPLEMENT_CLASS(UIElementLayerPanel, wxPanel)

enum EControlId

{
	ID_LIST,
	ID_ADD,
	ID_SAVE,
	ID_DELETE,
	ID_IMPORT,
	ID_EXPORT
};

// Required for WX
BEGIN_EVENT_TABLE(UIElementLayerPanel, wxPanel)
	EVT_LIST_ITEM_ACTIVATED(wxID_ANY, UIElementLayerPanel::onItemActivated)
	EVT_LIST_ITEM_RIGHT_CLICK(wxID_ANY, UIElementLayerPanel::onItemActivated)
	EVT_LIST_ITEM_SELECTED(wxID_ANY, UIElementLayerPanel::onItemSelected)
	EVT_LIST_BEGIN_LABEL_EDIT(wxID_ANY, UIElementLayerPanel::onBeginLabelEditCommand)
	EVT_LIST_END_LABEL_EDIT(wxID_ANY, UIElementLayerPanel::onEndLabelEditCommand)
	EVT_BUTTON(ID_ADD, UIElementLayerPanel::onAddCommand)
	EVT_BUTTON(ID_SAVE, UIElementLayerPanel::onSaveCommand)
	EVT_BUTTON(ID_DELETE, UIElementLayerPanel::onDeleteCommand)
	EVT_BUTTON(ID_IMPORT, UIElementLayerPanel::onImportCommand)
	EVT_BUTTON(ID_EXPORT, UIElementLayerPanel::onExportCommand)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
UIElementLayerPanel::UIElementLayerPanel(wxWindow* parentWindow, const wxString& name, const wxString& caption, UIElementLayerPanelListener* listener, wxAuiManager* auiManager)
:	wxPanel(parentWindow),
	mParentWindow(parentWindow),
	mLayerPanelName(name),
	mAuiManager(auiManager),
	mListCtrl(NULL),
	mListener(listener)
{
	mListCtrl = new wxListCtrl(
		this, 
		wxID_ANY,
        wxDefaultPosition, 
		wxDefaultSize,
        wxLC_SINGLE_SEL | 
		wxLC_LIST |
        wxSUNKEN_BORDER | 
		wxLC_EDIT_LABELS
		);

	wxBoxSizer *sizerV = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *sizerH = new wxBoxSizer( wxHORIZONTAL );

	mAddButton = new wxBitmapButton(this, ID_ADD, Utils::getBitmap("ICON_PLUS16"));
	mAddButton->SetToolTip(_T("Create layer from current properties"));
	sizerH->Add(mAddButton, 0, 0);

	mSaveButton = new wxBitmapButton(this, ID_SAVE, Utils::getBitmap("ICON_SAVE16"));
	mSaveButton->SetToolTip(_T("Save current properties to layer"));
	sizerH->Add(mSaveButton, 0, 0);

	mDeleteButton = new wxBitmapButton(this, ID_DELETE, Utils::getBitmap("ICON_TRASHCAN16"));
	mDeleteButton->SetToolTip(_T("Delete layer"));
	sizerH->Add(mDeleteButton, 0, 0);

	sizerH->AddSpacer(10);
	sizerH->AddStretchSpacer();

	mExportButton = new wxBitmapButton(this, ID_EXPORT, Utils::getBitmap("ICON_EXPORT16"));
	mExportButton->SetToolTip(_T("Export current properties to a layer file"));
	sizerH->Add(mExportButton, 0, 0);

	mImportButton = new wxBitmapButton(this, ID_IMPORT, Utils::getBitmap("ICON_IMPORT16"));
	mImportButton->SetToolTip(_T("Import layer from a layer file"));
	sizerH->Add(mImportButton, 0, 0);

	sizerV->Add(sizerH, 0, wxEXPAND);
	sizerV->Add(mListCtrl, 1, wxEXPAND);
	sizerV->SetSizeHints(this);
	SetSizer(sizerV);

	if (mAuiManager)
	{
		mAuiManager->AddPane(this, 
			wxAuiPaneInfo().
			Name(mLayerPanelName).
			Caption(caption).
			Right().
			Layer(1).
			CloseButton(true).
			MaximizeButton(false).
			PinButton(true).
			BestSize(wxSize(200, 480)).
			MinSize(wxSize(32, 32)));
	}
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::populate(const StringList& layerNames)
{
	mListCtrl->ClearAll();

	mDeleteButton->Enable(false);
	mSaveButton->Enable(false);

	long itemId = 0;

	StringList::const_iterator nameIt, nameItEnd = layerNames.end();
	for (nameIt = layerNames.begin(); nameIt != nameItEnd; ++nameIt)
	{
		mListCtrl->InsertItem(itemId, *nameIt, itemId);
		++itemId;
	}

	selectLayer(std::min(mSelectedLayerIndex, (int)layerNames.size() - 1));
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::onItemActivated(wxListEvent &event)
{
	if (mListener)
	{
		mSelectedLayerIndex = event.GetIndex();
		mListener->onLayerPanelLoadLayer(mLayerPanelName, mListCtrl->GetItemText(mSelectedLayerIndex));
	}
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::onItemSelected(wxListEvent &event)
{
	mSelectedLayerIndex = event.GetIndex();

	if (mListener)
	{
		mListener->onLayerPanelSelectLayer(mLayerPanelName, mListCtrl->GetItemText(mSelectedLayerIndex));
	}

	bool enable = true;//(mListCtrl->GetItemText(mSelectedLayerIndex).compare(wxString(Utils::defaultNameString)) != 0);
	mDeleteButton->Enable(enable);
	mSaveButton->Enable(enable);
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::onAddCommand(wxCommandEvent& event)
{
	wxString layerName = _T("Layer");

	if (isValidIndex(mSelectedLayerIndex))
	{
		layerName = mListCtrl->GetItemText(mSelectedLayerIndex);
	}

	if (mListener)
	{
		mListener->onLayerPanelAddLayer(mLayerPanelName, layerName);
	}
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::onDeleteCommand(wxCommandEvent& event)
{
	if (isValidIndex(mSelectedLayerIndex))
	{
		wxString layerName = mListCtrl->GetItemText(mSelectedLayerIndex);
		if (mListener)
		{
			mListener->onLayerPanelDeleteLayer(mLayerPanelName, layerName);
		}
	}
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::onSaveCommand(wxCommandEvent& event)
{
	if (isValidIndex(mSelectedLayerIndex))
	{
		wxString layerName = mListCtrl->GetItemText(mSelectedLayerIndex);
		if (mListener)
		{
			mListener->onLayerPanelSaveLayer(mLayerPanelName, layerName);
		}
	}
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::onBeginLabelEditCommand(wxListEvent& event)
{
	mSelectedLayerIndex = event.GetIndex();
	if (mListCtrl->GetItemText(mSelectedLayerIndex).compare(wxString(Utils::defaultNameString)) == 0)
	{
		event.Veto();
	}
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::onEndLabelEditCommand(wxListEvent& event)
{
	if (!event.IsEditCancelled())
	{
		mSelectedLayerIndex = event.GetIndex();
		wxString oldName = mListCtrl->GetItemText(mSelectedLayerIndex);
		wxString newName = event.GetText();
		if (newName.compare(oldName) != 0)
		{
			int itemCount = mListCtrl->GetItemCount();
			bool unique = true;
			for (int index = 0; index < itemCount; ++index)
			{
				if (event.GetIndex() != index)
				{
					unique = unique && (mListCtrl->GetItemText(index).compare(newName) != 0);
				}
			}

			if (unique && newName.length())
			{
				if (mListener)
				{
					mListener->onLayerPanelChangeLayerName(mLayerPanelName, oldName, newName);
				}
			}
			else
			{
				event.Veto();
			}
		}
	}
}

// ----------------------------------------------------------------------------
void UIElementLayerPanel::selectLayer(int layerIndex, bool edit)
{
	if (isValidIndex(layerIndex))
	{
		mListCtrl->SetItemState(
			layerIndex, 
			wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED, 
			wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);

		if (edit)
		{
			mListCtrl->EditLabel(layerIndex);
		}
	}
}


// ----------------------------------------------------------------------------
int UIElementLayerPanel::findLayer(const wxString& layerName)
{
	int foundIndex = -1;
	do
	{
		foundIndex = mListCtrl->FindItem(foundIndex, layerName);
		if (isValidIndex(foundIndex) && mListCtrl->GetItemText(foundIndex).compare(layerName) == 0)
		{
			return foundIndex;
		}
	} 
	while (isValidIndex(foundIndex));
	return -1;
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::selectLayer(const wxString layerName, bool edit)
{
	selectLayer(findLayer(layerName), edit);
}


// ----------------------------------------------------------------------------
bool UIElementLayerPanel::isValidIndex(int layerIndex)
{
	return layerIndex >= 0 && layerIndex < mListCtrl->GetItemCount();
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::onImportCommand(wxCommandEvent& event)
{
	static string lastPath(_T("."));

    wxString path = wxFileSelector(
		_T("Choose layer file to import"),
		lastPath.c_str(), 
		wxEmptyString,
		wxEmptyString,
		wxString::Format
		(
			_T("Layer files (*.spe)|*.spe|All files (%s)|%s"),
			wxFileSelectorDefaultWildcardStr,
			wxFileSelectorDefaultWildcardStr
		),
		wxFD_OPEN|wxFD_FILE_MUST_EXIST,
		this
		);

	if (path.length())
	{
		lastPath = path;
		if (mListener)
		{
			mListener->onLayerPanelImportLayer(mLayerPanelName, path);
		}
	}
}


// ----------------------------------------------------------------------------
void UIElementLayerPanel::onExportCommand(wxCommandEvent& event)
{
	static string lastPath(_T("."));

    wxString path = wxFileSelector(
		_T("Choose filename to export layer"),
		lastPath.c_str(), 
		wxEmptyString,
		wxEmptyString,
		wxString::Format
		(
			_T("Layer files (*.spe)|*.spe|All files (%s)|%s"),
			wxFileSelectorDefaultWildcardStr,
			wxFileSelectorDefaultWildcardStr
		),
		wxFD_SAVE|wxFD_OVERWRITE_PROMPT,
		this
		);

	if (path.length())
	{
		lastPath = path;
		if (mListener)
		{
			mListener->onLayerPanelExportLayer(mLayerPanelName, path);
		}
	}
}

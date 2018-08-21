/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "UIElementPresetPanel.h"
#include "PCH/stdafx.h"

using namespace ScapeEditor;

// Required for WX
IMPLEMENT_CLASS(UIElementPresetPanel, wxPanel)

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
BEGIN_EVENT_TABLE(UIElementPresetPanel, wxPanel)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY, UIElementPresetPanel::onItemActivated)
EVT_LIST_ITEM_RIGHT_CLICK(wxID_ANY, UIElementPresetPanel::onItemActivated)
EVT_LIST_ITEM_SELECTED(wxID_ANY, UIElementPresetPanel::onItemSelected)
EVT_LIST_BEGIN_LABEL_EDIT(wxID_ANY, UIElementPresetPanel::onBeginLabelEditCommand)
EVT_LIST_END_LABEL_EDIT(wxID_ANY, UIElementPresetPanel::onEndLabelEditCommand)
EVT_BUTTON(ID_ADD, UIElementPresetPanel::onAddCommand)
EVT_BUTTON(ID_SAVE, UIElementPresetPanel::onSaveCommand)
EVT_BUTTON(ID_DELETE, UIElementPresetPanel::onDeleteCommand)
EVT_BUTTON(ID_IMPORT, UIElementPresetPanel::onImportCommand)
EVT_BUTTON(ID_EXPORT, UIElementPresetPanel::onExportCommand)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
UIElementPresetPanel::UIElementPresetPanel(wxWindow* parentWindow, const wxString& name,
                                           const wxString& caption, UIElementPresetPanelListener* listener,
                                           wxAuiManager* auiManager)
    : wxPanel(parentWindow), mParentWindow(parentWindow), mPresetPanelName(name), mAuiManager(auiManager),
      mListCtrl(NULL), mListener(listener)
{
    mListCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                               wxLC_SINGLE_SEL | wxLC_LIST | wxSUNKEN_BORDER | wxLC_EDIT_LABELS);

    wxBoxSizer* sizerV = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizerH = new wxBoxSizer(wxHORIZONTAL);

    mAddButton = new wxBitmapButton(this, ID_ADD, Utils::getBitmap("ICON_PLUS16"));
    mAddButton->SetToolTip(_T("Create preset from current properties"));
    sizerH->Add(mAddButton, 0, 0);

    mSaveButton = new wxBitmapButton(this, ID_SAVE, Utils::getBitmap("ICON_SAVE16"));
    mSaveButton->SetToolTip(_T("Save current properties to preset"));
    sizerH->Add(mSaveButton, 0, 0);

    mDeleteButton = new wxBitmapButton(this, ID_DELETE, Utils::getBitmap("ICON_TRASHCAN16"));
    mDeleteButton->SetToolTip(_T("Delete preset"));
    sizerH->Add(mDeleteButton, 0, 0);

    sizerH->AddSpacer(10);
    sizerH->AddStretchSpacer();

    mExportButton = new wxBitmapButton(this, ID_EXPORT, Utils::getBitmap("ICON_EXPORT16"));
    mExportButton->SetToolTip(_T("Export current properties to a preset file"));
    sizerH->Add(mExportButton, 0, 0);

    mImportButton = new wxBitmapButton(this, ID_IMPORT, Utils::getBitmap("ICON_IMPORT16"));
    mImportButton->SetToolTip(_T("Import preset from a preset file"));
    sizerH->Add(mImportButton, 0, 0);

    sizerV->Add(sizerH, 0, wxEXPAND);
    sizerV->Add(mListCtrl, 1, wxEXPAND);
    sizerV->SetSizeHints(this);
    SetSizer(sizerV);

    if (mAuiManager)
    {
        mAuiManager->AddPane(this, wxAuiPaneInfo()
                                       .Name(mPresetPanelName)
                                       .Caption(caption)
                                       .Right()
                                       .Layer(1)
                                       .CloseButton(true)
                                       .MaximizeButton(false)
                                       .PinButton(true)
                                       .BestSize(wxSize(200, 480))
                                       .MinSize(wxSize(32, 32)));
    }
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::populate(const StringList& presetNames)
{
    mListCtrl->ClearAll();

    mDeleteButton->Enable(false);
    mSaveButton->Enable(false);

    long itemId = 0;

    StringList::const_iterator nameIt, nameItEnd = presetNames.end();
    for (nameIt = presetNames.begin(); nameIt != nameItEnd; ++nameIt)
    {
        mListCtrl->InsertItem(itemId, *nameIt, itemId);
        ++itemId;
    }

    selectPreset(std::min(mSelectedPresetIndex, (int)presetNames.size() - 1));
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::onItemActivated(wxListEvent& event)
{
    if (mListener)
    {
        mSelectedPresetIndex = event.GetIndex();
        mListener->onPresetPanelLoadPreset(mPresetPanelName, mListCtrl->GetItemText(mSelectedPresetIndex));
    }
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::onItemSelected(wxListEvent& event)
{
    mSelectedPresetIndex = event.GetIndex();

    if (mListener)
    {
        mListener->onPresetPanelSelectPreset(mPresetPanelName,
                                             mListCtrl->GetItemText(mSelectedPresetIndex));
    }

    bool enable =
        true; //(mListCtrl->GetItemText(mSelectedPresetIndex).compare(wxString(Utils::defaultNameString)) !=
              //0);
    mDeleteButton->Enable(enable);
    mSaveButton->Enable(enable);
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::onAddCommand(wxCommandEvent& event)
{
    wxString presetName = _T("Preset");

    if (isValidIndex(mSelectedPresetIndex))
    {
        presetName = mListCtrl->GetItemText(mSelectedPresetIndex);
    }

    if (mListener)
    {
        mListener->onPresetPanelAddPreset(mPresetPanelName, presetName);
    }
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::onDeleteCommand(wxCommandEvent& event)
{
    if (isValidIndex(mSelectedPresetIndex))
    {
        wxString presetName = mListCtrl->GetItemText(mSelectedPresetIndex);
        if (mListener)
        {
            mListener->onPresetPanelDeletePreset(mPresetPanelName, presetName);
        }
    }
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::onSaveCommand(wxCommandEvent& event)
{
    if (isValidIndex(mSelectedPresetIndex))
    {
        wxString presetName = mListCtrl->GetItemText(mSelectedPresetIndex);
        if (mListener)
        {
            mListener->onPresetPanelSavePreset(mPresetPanelName, presetName);
        }
    }
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::onBeginLabelEditCommand(wxListEvent& event)
{
    mSelectedPresetIndex = event.GetIndex();
    if (mListCtrl->GetItemText(mSelectedPresetIndex).compare(wxString(Utils::defaultNameString)) == 0)
    {
        event.Veto();
    }
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::onEndLabelEditCommand(wxListEvent& event)
{
    if (!event.IsEditCancelled())
    {
        mSelectedPresetIndex = event.GetIndex();
        wxString oldName = mListCtrl->GetItemText(mSelectedPresetIndex);
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
                    mListener->onPresetPanelChangePresetName(mPresetPanelName, oldName, newName);
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
void UIElementPresetPanel::selectPreset(int presetIndex, bool edit)
{
    if (isValidIndex(presetIndex))
    {
        mListCtrl->SetItemState(presetIndex, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED,
                                wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);

        if (edit)
        {
            mListCtrl->EditLabel(presetIndex);
        }
    }
}

// ----------------------------------------------------------------------------
int UIElementPresetPanel::findPreset(const wxString& presetName)
{
    int foundIndex = -1;
    do
    {
        foundIndex = mListCtrl->FindItem(foundIndex, presetName);
        if (isValidIndex(foundIndex) && mListCtrl->GetItemText(foundIndex).compare(presetName) == 0)
        {
            return foundIndex;
        }
    } while (isValidIndex(foundIndex));
    return -1;
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::selectPreset(const wxString presetName, bool edit)
{
    selectPreset(findPreset(presetName), edit);
}

// ----------------------------------------------------------------------------
bool UIElementPresetPanel::isValidIndex(int presetIndex)
{
    return presetIndex >= 0 && presetIndex < mListCtrl->GetItemCount();
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::onImportCommand(wxCommandEvent& event)
{
    static string lastPath(_T("."));

    wxString path =
        wxFileSelector(_T("Choose preset file to import"), lastPath.c_str(), wxEmptyString, wxEmptyString,
                       wxString::Format(_T("Preset files (*.spe)|*.spe|All files (%s)|%s"),
                                        wxFileSelectorDefaultWildcardStr, wxFileSelectorDefaultWildcardStr),
                       wxFD_OPEN | wxFD_FILE_MUST_EXIST, this);

    if (path.length())
    {
        lastPath = path;
        if (mListener)
        {
            mListener->onPresetPanelImportPreset(mPresetPanelName, path);
        }
    }
}

// ----------------------------------------------------------------------------
void UIElementPresetPanel::onExportCommand(wxCommandEvent& event)
{
    static string lastPath(_T("."));

    wxString path = wxFileSelector(
        _T("Choose filename to export preset"), lastPath.c_str(), wxEmptyString, wxEmptyString,
        wxString::Format(_T("Preset files (*.spe)|*.spe|All files (%s)|%s"),
                         wxFileSelectorDefaultWildcardStr, wxFileSelectorDefaultWildcardStr),
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT, this);

    if (path.length())
    {
        lastPath = path;
        if (mListener)
        {
            mListener->onPresetPanelExportPreset(mPresetPanelName, path);
        }
    }
}

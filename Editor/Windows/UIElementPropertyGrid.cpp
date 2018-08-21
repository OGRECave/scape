/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "UIElementPropertyGrid.h"
#include "Controls/PGDragCtrlEditor.h"
#include "PCH/stdafx.h"
#include <wx/propgrid/advprops.h> // Extra property classes.
#include <wx/propgrid/manager.h>  // This defines wxPropertyGridManager.
#include <wx/propgrid/propdev.h>  // Needed for implementing custom properties.
#include <wx/propgrid/propgrid.h> // Main propertygrid header.

using namespace ScapeEditor;

// ----------------------------------------------------------------------------
enum
{
    EVENTID_PG = 1,
};

// ----------------------------------------------------------------------------
IMPLEMENT_CLASS(UIElementPropertyGrid, wxPanel)

BEGIN_EVENT_TABLE(UIElementPropertyGrid, wxPanel)
EVT_SIZE(UIElementPropertyGrid::onSize)
EVT_PG_CHANGED(EVENTID_PG, UIElementPropertyGrid::onPropertyGridChange)
EVT_PG_RIGHT_CLICK(EVENTID_PG, UIElementPropertyGrid::onPropertyGridItemRightClick)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
UIElementPropertyGrid::UIElementPropertyGrid(wxWindow* parent, const wxString& name,
                                             const wxString& caption,
                                             UIElementPropertyGridListener* listener,
                                             wxAuiManager* auiManager)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxPanelNameStr), mName(name),
      mGridManager(NULL), mListener(listener), mParentWindow(parent), mAuiManager(auiManager)
{
    wxPGRegisterEditorClass(DragCtrl);

    mGridManager = new wxPropertyGridManager(this, EVENTID_PG, wxDefaultPosition, wxDefaultSize,
                                             // wxPG_BOLD_MODIFIED |
                                             wxPG_SPLITTER_AUTO_CENTER |
                                                 // wxPG_AUTO_SORT |
                                                 // wxPG_HIDE_MARGIN|
                                                 // wxPG_STATIC_SPLITTER |
                                                 wxPG_TOOLTIPS |
                                                 // wxPG_HIDE_CATEGORIES |
                                                 // wxPG_LIMITED_EDITING |
                                                 wxTAB_TRAVERSAL
                                             // wxPG_TOOLBAR
                                             // wxPG_DESCRIPTION
                                             // wxPG_COMPACTOR
                                             );

    mGridManager->SetExtraStyle(
        // wxPG_EX_MODE_BUTTONS |
        // wxPG_EX_AUTO_UNSPECIFIED_VALUES |
        // wxPG_EX_GREY_LABEL_WHEN_DISABLED |
        // wxPG_EX_NATIVE_DOUBLE_BUFFERING |
        wxPG_EX_HELP_AS_TOOLTIPS);

    // mGridManager->SetDescBoxHeight(50);

    mGridManager->SetSplitterPosition(100);

    if (mAuiManager)
    {
        mAuiManager->AddPane(this, wxAuiPaneInfo()
                                       .Name(mName)
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
UIElementPropertyGrid::~UIElementPropertyGrid()
{
    if (mGridManager->GetPageCount())
    {
        mGridManager->ClearPage(0);
    }
}

// ----------------------------------------------------------------------------
void UIElementPropertyGrid::onSize(wxSizeEvent& event) { mGridManager->SetSize(event.GetSize()); }

// ----------------------------------------------------------------------------
void UIElementPropertyGrid::onPropertyGridChange(wxPropertyGridEvent& event)
{
    wxPGId id = event.GetProperty();

    const wxString& name = event.GetPropertyName();

    if (mListener)
    {
        wxString value(event.GetPropertyValueAsString());
        mGridManager->SetPropertyValue(id,
                                       mListener->onUpdatePropertyGridPropertyValue(mName, name, value));
    }
}

// ----------------------------------------------------------------------------
void UIElementPropertyGrid::onPropertyGridItemRightClick(wxPropertyGridEvent& event)
{
    wxPGId id = event.GetProperty();

    const wxString& name = event.GetPropertyName();

    if (mListener)
    {
        mGridManager->SetPropertyValue(id, mListener->onResetPropertyGridPropertyValue(mName, name));
    }
}

// ----------------------------------------------------------------------------
void UIElementPropertyGrid::populate(const UIElementPropertyGridItemList& itemList)
{
    if (!mGridManager->GetPageCount())
    {
        mGridManager->AddPage(_T("Page"));
    }

    mGridManager->ClearPage(0);

    UIElementPropertyGridItemList::const_iterator itemIt, itemItEnd = itemList.end();
    for (itemIt = itemList.begin(); itemIt != itemItEnd; ++itemIt)
    {
        const string& name = itemIt->name;
        wxPGId pgId = 0;

        wxString categoryName = itemIt->category;
        if (categoryName.Length() == 0)
            categoryName = _T("General");
        mGridManager->Append(wxPropertyCategory(categoryName, wxPG_LABEL));

        wxString type = itemIt->type;
        if (type.compare("INT") == 0)
        {
            pgId = mGridManager->Append(wxIntProperty(itemIt->label, name));
            if (pgId.IsOk())
            {
                mGridManager->SetPropertyEditor(pgId, wxPG_EDITOR(DragCtrl));
            }
        }
        else if (type.compare("REAL") == 0)
        {
            pgId = mGridManager->Append(wxFloatProperty(itemIt->label, name));
            if (pgId.IsOk())
            {
                mGridManager->SetPropertyEditor(pgId, wxPG_EDITOR(DragCtrl));
            }
        }
        else if (type.compare("BOOL") == 0)
        {
            pgId = mGridManager->Append(wxBoolProperty(itemIt->label, name));
        }
        else if (type.compare("COLOR") == 0)
        {
            pgId = mGridManager->Append(wxColourProperty(itemIt->label, name));
        }
        else if (type.compare("FILE") == 0)
        {
            pgId = mGridManager->Append(wxFileProperty(itemIt->label, name));
            if (pgId.IsOk())
            {
                mGridManager->SetPropertyAttribute(pgId, wxPG_FILE_SHOW_FULL_PATH, 0);
            }
        }
        else
        {
            pgId = mGridManager->Append(wxStringProperty(itemIt->label, name));
        }

        if (pgId.IsOk())
        {
            mGridManager->SetPropertyHelpString(pgId, itemIt->description);
        }
    }
}

// ----------------------------------------------------------------------------
void UIElementPropertyGrid::setValues(const StringStringMap& valueMap)
{
    StringStringMap::const_iterator propertyIt, propertyItEnd = valueMap.end();
    for (propertyIt = valueMap.begin(); propertyIt != propertyItEnd; ++propertyIt)
    {
        wxString value(mListener->onUpdatePropertyGridPropertyValue(mName, propertyIt->first,
                                                                    wxString(propertyIt->second)));
        mGridManager->SetPropertyValue(propertyIt->first, value);
    }
}

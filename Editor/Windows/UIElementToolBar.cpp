/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
#include "UIElementToolBar.h"

using namespace ScapeEditor;

// Required for WX
IMPLEMENT_CLASS(UIElementToolBar, wxToolBar)

// Required for WX
BEGIN_EVENT_TABLE(UIElementToolBar, wxToolBar)
EVT_TOOL(wxID_ANY, UIElementToolBar::onItemClicked)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
UIElementToolBar::UIElementToolBar(
    const wxString& toolBarName, wxWindow* parentWindow, UIElementToolBarListener* listener, wxAuiManager* auiManager)
    : wxToolBar(parentWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT |
              // wxTB_DOCKABLE |
              // wxTB_HORIZONTAL |
              // wxTB_VERTICAL |
              // wxTB_TEXT |
              // wxTB_NOICONS |
              wxTB_NODIVIDER |
              // wxTB_NOALIGN |
              // wxTB_HORZ_LAYOUT |
              // wxTB_HORZ_TEXT |
              // wxTB_NO_TOOLTIPS |
              // wxTB_BOTTOM |
              // wxTB_RIGHT |
              0)
    , mToolBarName(toolBarName)
    , mAuiManager(auiManager)
    , mListener(listener)
{
    SetToolBitmapSize(wxSize(24, 24));

    if (mAuiManager)
    {
        mAuiManager->AddPane(this, wxAuiPaneInfo().Name(mToolBarName).ToolbarPane().Top());
    }
}

// ----------------------------------------------------------------------------
UIElementToolBar::~UIElementToolBar()
{
}

// ----------------------------------------------------------------------------
void UIElementToolBar::populate(const UIElementToolBarItemList& itemList)
{
    ClearTools();

    mItemVector.clear();
    mItemVector.resize(itemList.size());

    // bool addSeperator = false;
    size_t toolId = 0;

    UIElementToolBarItemList::const_iterator itemIt, itemItEnd = itemList.end();
    int lastCategory = -1;
    for (itemIt = itemList.begin(); itemIt != itemItEnd; ++itemIt)
    {
        const UIElementToolBarItem& item = *itemIt;

        if (itemIt != itemList.begin() && lastCategory != item.category)
        {
            // AddSeparator();
        }

        AddRadioTool(toolId, item.label, Utils::getBitmap(item.icon), wxNullBitmap, item.label, item.description);

        lastCategory = item.category;

        mItemVector[toolId] = item;

        ++toolId;
    }

    Realize();

    // A call to aui manager's Update() doesn't do a resize of the toolbar, so force it like this:
    if (mAuiManager)
    {
        Refresh();
        wxAuiPaneInfo& info = mAuiManager->GetPane(this);
        info.BestSize(GetBestSize());
    }

    if (mListener && mItemVector.size())
    {
        mListener->onToolBarItemClicked(mToolBarName, mItemVector[0].name, mItemVector[0].category);
    }
}

// ----------------------------------------------------------------------------
void UIElementToolBar::onItemClicked(wxCommandEvent& event)
{
    size_t eventId = (size_t)event.GetId();
    if (eventId >= 0 && eventId < mItemVector.size())
    {
        UIElementToolBarItem& item = mItemVector[eventId];

        if (mListener)
        {
            mListener->onToolBarItemClicked(mToolBarName, item.name, item.category);
        }
    }
}

/**
 * ScapeEditor::MainFrame class
 *
 * Manages the main tool toolbar
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#ifndef __UIELEMENTTOOLBAR_H__
#define __UIELEMENTTOOLBAR_H__

//#include "UIElementClasses.h"

// ----------------------------------------------------------------------------
namespace ScapeEngine
{
	// ----------------------------------------------------------------------------
	struct string;
}

// ----------------------------------------------------------------------------
namespace ScapeEditor
{
	// ----------------------------------------------------------------------------
	class UIElementToolBarListener
	{
	public:
		virtual void onToolBarItemClicked(const wxString& toolBarName, const wxString& itemName, int itemCategory) = 0;
	};


	// ----------------------------------------------------------------------------
	struct UIElementToolBarItem
	{
		string name;
		string label;
		string description;
		string icon;
		int category;
	};

	typedef std::list<UIElementToolBarItem> UIElementToolBarItemList;



	// ----------------------------------------------------------------------------
	class UIElementToolBar : public wxToolBar
	{
		DECLARE_CLASS(UIElementToolBar);

	public:
		UIElementToolBar(
			const wxString& toolBarName,
			wxWindow* parentWindow, 
			UIElementToolBarListener* listener,
			wxAuiManager* auiManager);

		~UIElementToolBar();

		void populate(const UIElementToolBarItemList& descs);

		wxString getToolBarName() {return mToolBarName;}

	protected:
		DECLARE_EVENT_TABLE()

		typedef std::vector<UIElementToolBarItem> UIElementToolBarItemVector;
		UIElementToolBarItemVector mItemVector;

		wxString mToolBarName;
		wxAuiManager* mAuiManager;
		UIElementToolBarListener* mListener;

		void onItemClicked(wxCommandEvent& event);
	};
}

#endif __UIELEMENTTOOLBAR_H__
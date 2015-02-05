/**
 * ScapeEditor::UIElementPropertyGrid class
 *
 * Property grid container, supporting importing and exporting as wxString maps
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#ifndef __UIELEMENTPROPERTYGRID_H__
#define __UIELEMENTPROPERTYGRID_H__

#include "EngineInterface.h"
#include <wx/propgrid/propgrid.h>
#include <wx/wx.h>
#include <wx/aui/aui.h>
// ----------------------------------------------------------------------------
namespace ScapeEditor
{
	// ----------------------------------------------------------------------------
	class UIElementPropertyGridListener
	{
	public:
		// should react to the change and return a corrected (e.g. clamped) version of 'value'
		virtual wxString onUpdatePropertyGridPropertyValue(const wxString& name, const wxString& propertyName, const wxString& value) = 0;

		// shoud reset to the requested property, returning the new value
		virtual wxString onResetPropertyGridPropertyValue(const wxString& name, const wxString& propertyName) = 0;
	};

	// ----------------------------------------------------------------------------
	struct UIElementPropertyGridItem
	{
		string type;
		string name;
		string label;
		string description;
		string category;
	};

	typedef std::list<UIElementPropertyGridItem> UIElementPropertyGridItemList;

	// ----------------------------------------------------------------------------
	class UIElementPropertyGrid : public wxPanel
	{
		DECLARE_CLASS(UIElementPropertyGrid)

	public:
	    UIElementPropertyGrid(
			wxWindow *parent,
			const wxString& name,
			const wxString& caption,
			UIElementPropertyGridListener* listener,
			wxAuiManager *auiManager);

		~UIElementPropertyGrid();

		void populate(const UIElementPropertyGridItemList& itemList);
        void setValues(const ScapeEngine::StringStringMap& valueMap);

	protected:
		DECLARE_EVENT_TABLE()

		wxPropertyGridManager* mGridManager;
		UIElementPropertyGridListener* mListener;
		wxWindow* mParentWindow;
		wxAuiManager* mAuiManager;
		wxString mName;

		void onSize(wxSizeEvent &event);
	    void onPropertyGridChange(wxPropertyGridEvent& event);
		void onPropertyGridItemRightClick(wxPropertyGridEvent& event);
	};

}

#endif // __UIELEMENTPROPERTYGRID_H__

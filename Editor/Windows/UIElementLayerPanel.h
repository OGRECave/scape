/**
 * ScapeEditor::RenderPanel class
 *
 * Show the Renders a 3D rendered view inside the control
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#ifndef __UIELEMENTLAYERPANEL_H__
#define __UIELEMENTLAYERPANEL_H__

#include "EngineInterface.h"
#include "wx/listctrl.h"

// ----------------------------------------------------------------------------
namespace ScapeEditor
{
	// ----------------------------------------------------------------------------
	class UIElementLayerPanelListener
	{
	public:
		virtual void onLayerPanelSelectLayer(const wxString& layerPanelName, const wxString& layerName) = 0;
		virtual void onLayerPanelLoadLayer(const wxString& layerPanelName, const wxString& layerName) = 0;
		virtual void onLayerPanelSaveLayer(const wxString& layerPanelName, const wxString& layerName) = 0;
		virtual void onLayerPanelAddLayer(const wxString& layerPanelName, const wxString& layerName) = 0;
		virtual void onLayerPanelDeleteLayer(const wxString& layerPanelName, const wxString& layerName) = 0;
		virtual void onLayerPanelChangeLayerName(const wxString& layerPanelName, const wxString& oldLayerName, const wxString& newLayerName) = 0;
		virtual void onLayerPanelImportLayer(const wxString& layerPanelName, const wxString& fileName) = 0;
		virtual void onLayerPanelExportLayer(const wxString& layerPanelName, const wxString& fileName) = 0;
	};


	// ----------------------------------------------------------------------------
	class UIElementLayerPanel : public wxPanel
	{
		DECLARE_CLASS(UIElementLayerPanel)

		UIElementLayerPanel(wxWindow* parentWindow, const wxString& name, const wxString& caption, UIElementLayerPanelListener* listener, wxAuiManager* auiManager);

	public:
		UIElementLayerPanel();

		void populate(const StringList& descsMap);

		void selectLayer(const wxString layerName, bool edit = false);

		void onItemActivated(wxListEvent &event);
		void onItemSelected(wxListEvent &event);
		void onAddCommand(wxCommandEvent& event);
		void onDeleteCommand(wxCommandEvent& event);
		void onImportCommand(wxCommandEvent& event);
		void onExportCommand(wxCommandEvent& event);
		void onSaveCommand(wxCommandEvent& event);
		void onBeginLabelEditCommand(wxListEvent &event);
		void onEndLabelEditCommand(wxListEvent &event);


	protected:
		DECLARE_EVENT_TABLE()

		wxAuiManager* mAuiManager;
		wxWindow* mParentWindow;
		wxListCtrl* mListCtrl;
		UIElementLayerPanelListener* mListener;
		wxString mLayerPanelName;
		int mSelectedLayerIndex;
		wxBitmapButton* mAddButton;
		wxBitmapButton* mSaveButton;
		wxBitmapButton* mDeleteButton;
		wxBitmapButton* mImportButton;
		wxBitmapButton* mExportButton;

		void selectLayer(int layerIndex, bool edit = false);
		int findLayer(const wxString& layerName);
		bool isValidIndex(int layerIndex);
	};

}

#endif // __UIELEMENTLAYERPANEL_H__

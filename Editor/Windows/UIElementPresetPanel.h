/**
 * ScapeEditor::RenderPanel class
 *
 * Show the Renders a 3D rendered view inside the control
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __UIELEMENTPRESETPANEL_H__
#define __UIELEMENTPRESETPANEL_H__

#include "EngineInterface.h"
#include "wx/listctrl.h"

// ----------------------------------------------------------------------------
namespace ScapeEditor {
    // ----------------------------------------------------------------------------
    class UIElementPresetPanelListener
    {
    public:
        virtual void onPresetPanelSelectPreset(const wxString& presetPanelName, const wxString& presetName) = 0;
        virtual void onPresetPanelLoadPreset(const wxString& presetPanelName, const wxString& presetName) = 0;
        virtual void onPresetPanelSavePreset(const wxString& presetPanelName, const wxString& presetName) = 0;
        virtual void onPresetPanelAddPreset(const wxString& presetPanelName, const wxString& presetName) = 0;
        virtual void onPresetPanelDeletePreset(const wxString& presetPanelName, const wxString& presetName) = 0;
        virtual void onPresetPanelChangePresetName(
            const wxString& presetPanelName, const wxString& oldPresetName, const wxString& newPresetName)
            = 0;
        virtual void onPresetPanelImportPreset(const wxString& presetPanelName, const wxString& fileName) = 0;
        virtual void onPresetPanelExportPreset(const wxString& presetPanelName, const wxString& fileName) = 0;
    };

    // ----------------------------------------------------------------------------
    class UIElementPresetPanel : public wxPanel
    {
        DECLARE_CLASS(UIElementPresetPanel)

        UIElementPresetPanel(wxWindow* parentWindow, const wxString& name, const wxString& caption,
            UIElementPresetPanelListener* listener, wxAuiManager* auiManager);

    public:
        UIElementPresetPanel();

        void populate(const StringList& descsMap);

        void selectPreset(const wxString presetName, bool edit = false);

        void onItemActivated(wxListEvent& event);
        void onItemSelected(wxListEvent& event);
        void onAddCommand(wxCommandEvent& event);
        void onDeleteCommand(wxCommandEvent& event);
        void onImportCommand(wxCommandEvent& event);
        void onExportCommand(wxCommandEvent& event);
        void onSaveCommand(wxCommandEvent& event);
        void onBeginLabelEditCommand(wxListEvent& event);
        void onEndLabelEditCommand(wxListEvent& event);

    protected:
        DECLARE_EVENT_TABLE()

        wxAuiManager* mAuiManager;
        wxWindow* mParentWindow;
        wxListCtrl* mListCtrl;
        UIElementPresetPanelListener* mListener;
        wxString mPresetPanelName;
        int mSelectedPresetIndex;
        wxBitmapButton* mAddButton;
        wxBitmapButton* mSaveButton;
        wxBitmapButton* mDeleteButton;
        wxBitmapButton* mImportButton;
        wxBitmapButton* mExportButton;

        void selectPreset(int presetIndex, bool edit = false);
        int findPreset(const wxString& presetName);
        bool isValidIndex(int presetIndex);
    };
}

#endif // __UIELEMENTPRESETPANEL_H__

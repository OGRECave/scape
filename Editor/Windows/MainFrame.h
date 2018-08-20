/**
 * ScapeEditor::MainFrame class
 *
 * Implements the main window of the editor application, initializing all other subcomponents
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "UIElementPropertyGrid.h"
#include "UIElementToolBar.h"
#include "UIElementPresetPanel.h"

// ----------------------------------------------------------------------------
namespace ScapeEngine {
    class EngineInterface;
    struct string;
}

// ----------------------------------------------------------------------------
namespace ScapeEditor {

    class Application;
    class UIElementPropertyGrid;
    class UIElementToolBar;
    class UIElementPresetPanel;
    class RenderPanel;

    // ----------------------------------------------------------------------------
    class MainFrame : public wxFrame,
                      public UIElementPropertyGridListener,
                      public UIElementToolBarListener,
                      public UIElementPresetPanelListener
    {
    public:
        MainFrame(Application* application);
        virtual ~MainFrame();

        // Menu commands
        void onMenuExit(wxCommandEvent& event);
        void onMenuToggleMaximize3DView(wxCommandEvent& event);
        void onMenuShowHideTools(wxCommandEvent& event);
        void onMenuShowHidePresets(wxCommandEvent& event);
        void onMenuShowHideProperties(wxCommandEvent& event);
        void onMenuLoadPerspective(wxCommandEvent& event);
        void onMenuSavePerspective(wxCommandEvent& event);
        void onMenuResetPerspective(wxCommandEvent& event);
        void onMenuImportImageFile(wxCommandEvent& event);
        void onMenuExportImageFile(wxCommandEvent& event);

        void onSize(wxSizeEvent& event);

        // Menu command update functions

        void onRenderTimer(wxTimerEvent& event);
        void onProcessIdle();

        // UIElementPropertyGridListener listener implementation
        virtual wxString onUpdatePropertyGridPropertyValue(
            const wxString& gridIdName, const wxString& propertyName, const wxString& value);
        virtual wxString onResetPropertyGridPropertyValue(const wxString& gridIdName, const wxString& propertyName);

        // UIElementTooBarListener listener implementation
        virtual void onToolBarItemClicked(const wxString& toolBarName, const wxString& itemName, int itemCategory);

        // UIElementPresetPanel listener implementation
        virtual void onPresetPanelSelectPreset(const wxString& presetPanelName, const wxString& presetName);
        virtual void onPresetPanelLoadPreset(const wxString& presetPanelName, const wxString& presetName);
        virtual void onPresetPanelSavePreset(const wxString& presetPanelName, const wxString& presetName);
        virtual void onPresetPanelAddPreset(const wxString& presetPanelName, const wxString& presetName);
        virtual void onPresetPanelDeletePreset(const wxString& presetPanelName, const wxString& presetName);
        virtual void onPresetPanelChangePresetName(
            const wxString& presetPanelName, const wxString& oldPresetName, const wxString& newPresetName);
        virtual void onPresetPanelImportPreset(const wxString& presetPanelName, const wxString& fileName);
        virtual void onPresetPanelExportPreset(const wxString& presetPanelName, const wxString& fileName);

    private:
        DECLARE_EVENT_TABLE()
        DECLARE_NO_COPY_CLASS(MainFrame)

        class ScapeEngine::EngineInterface* mEngineInterface;
        bool mAttachedInputToEngine;
        Application* mApplication;
        wxAuiManager* mAuiManager;
        UIElementToolBar* mMainToolBar;
        UIElementPresetPanel* mPresetPanel;
        wxString mDefaultPerspective;

        string mSelectedToolElementName;
        ScapeEngine::EScapeUIElementGroupId mSelectedToolElementGroupId;

        wxTimer* mTimer;
        RenderPanel* mRenderPanel;
        UIElementPropertyGrid* mPropertyGrid;

        void selectTool(const wxString& toolName, int category);

        void populateMainToolBar();
        void populatePropertyGrid();
        void populatePresetPanel(const string& selectedPreset = Utils::emptyString, bool edit = false);

        std::map<std::pair<int, wxString>, wxString> mPresetToolSelectedItemMap;
    };
}

#endif //__MAINFRAME_H__

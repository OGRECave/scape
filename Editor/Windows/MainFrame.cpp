/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
#include "EngineInterface.h"
#include "MainFrame.h"
#include "RenderPanel.h"
#include "Application/Application.h"
#include "UIElementPropertyGrid.h"
#include "UIElementToolBar.h"
#include "UIElementPresetPanel.h"
#include "FileExportDialog.h"
#include "FileImportTypeDialog.h"
#include "FileImportOptionsDialog.h"

#define PERSPECTIVE_FILENAME _T("Perspective.cfg")

#ifdef _DEBUG
#define APPLICATION_TITLE _T("Scape Editor 0.1.1 - Debug - ") _T(__DATE__)
#else // _DEBUG
#define APPLICATION_TITLE _T("Scape Editor 0.1.1 - ") _T(__DATE__)
#endif // _DEBUG

#define UIELEMENTPROPERTYNAME_LABEL _T("LABEL")
#define UIELEMENTPROPERTYNAME_SHORT _T("SHORT")
#define UIELEMENTPROPERTYNAME_LONG _T("LONG")
#define UIELEMENTPROPERTYNAME_ICON _T("ICON")
#define UIELEMENTPROPERTYNAME_TYPE _T("TYPE")
#define UIELEMENTPROPERTYNAME_CATEGORY _T("CATEGORY")
#define UIELEMENTPROPERTYNAME_OPTIONS _T("OPTIONS")

using namespace ScapeEditor;

// ----------------------------------------------------------------------------
// ID for the menu commands
enum
{
    EVENTID_PG = 1,
    ID_RENDERTIMER,
    ID_COMMAND_IMPORT_IMAGE,
    ID_COMMAND_EXPORT_IMAGE,
    ID_COMMAND_EXIT,
    ID_COMMAND_TOGGLEMAXIMIZE_3DVIEW,
    ID_COMMAND_SHOWHIDE_TOOLS,
    ID_COMMAND_SHOWHIDE_PRESETS,
    ID_COMMAND_SHOWHIDE_PROPERTIES,
    ID_COMMAND_PERSPECTIVE_SAVE,
    ID_COMMAND_PERSPECTIVE_LOAD,
    ID_COMMAND_PERSPECTIVE_RESET,
};

// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(ID_COMMAND_EXIT, MainFrame::onMenuExit)
EVT_MENU(ID_COMMAND_IMPORT_IMAGE, MainFrame::onMenuImportImageFile)
EVT_MENU(ID_COMMAND_EXPORT_IMAGE, MainFrame::onMenuExportImageFile)
EVT_MENU(ID_COMMAND_TOGGLEMAXIMIZE_3DVIEW, MainFrame::onMenuToggleMaximize3DView)
EVT_MENU(ID_COMMAND_SHOWHIDE_TOOLS, MainFrame::onMenuShowHideTools)
EVT_MENU(ID_COMMAND_SHOWHIDE_PRESETS, MainFrame::onMenuShowHidePresets)
EVT_MENU(ID_COMMAND_SHOWHIDE_PROPERTIES, MainFrame::onMenuShowHideProperties)
EVT_MENU(ID_COMMAND_PERSPECTIVE_SAVE, MainFrame::onMenuSavePerspective)
EVT_MENU(ID_COMMAND_PERSPECTIVE_LOAD, MainFrame::onMenuLoadPerspective)
EVT_MENU(ID_COMMAND_PERSPECTIVE_RESET, MainFrame::onMenuResetPerspective)
EVT_SIZE(MainFrame::onSize)
EVT_TIMER(ID_RENDERTIMER, MainFrame::onRenderTimer)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// CONSTRUCTION/DECONSTRUCTION
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
MainFrame::MainFrame(Application* application)
    : wxFrame(NULL, wxID_ANY, APPLICATION_TITLE, wxDefaultPosition, wxSize(1240, 885),
          wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE | wxMAXIMIZE)
    , mApplication(application)
    , mAuiManager(NULL)
    , mRenderPanel(NULL)
    , mAttachedInputToEngine(false)
    , mEngineInterface(NULL)
    , mPropertyGrid(NULL)
    , mTimer(NULL)
    , mMainToolBar(NULL)
    , mPresetPanel(NULL)
    , mSelectedToolElementName(Utils::emptyString)
    , mSelectedToolElementGroupId((ScapeEngine::EScapeUIElementGroupId)-1)
{
    assert(mApplication);

    mEngineInterface = new ScapeEngine::EngineInterface();
    mEngineInterface->initialize();

    mTimer = new wxTimer(this, ID_RENDERTIMER);

    mAuiManager = new wxAuiManager(this); //, wxAUI_MGR_DEFAULT | wxAUI_MGR_ALLOW_ACTIVE_PANE);

    CreateStatusBar(2);
    wxToolTip::SetDelay(0);
    wxToolTip::Enable(true);

    SetIcon(Utils::getIcon("ICON_APPLICATION"));

    wxMenuBar* mainMenuBar = new wxMenuBar;

    wxMenu* fileMenu = new wxMenu;
    mainMenuBar->Append(fileMenu, _T("&File"));
    fileMenu->Append(ID_COMMAND_IMPORT_IMAGE, _T("&Import image...\tCtrl-Shift-O"));
    fileMenu->Append(ID_COMMAND_EXPORT_IMAGE, _T("&Export image...\tCtrl-Shift-S"));
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_COMMAND_EXIT, _T("E&xit\tAlt-F4"), _T("Exit"));

    wxMenu* windowMenu = new wxMenu;
    mainMenuBar->Append(windowMenu, _T("&Window"));
    windowMenu->Append(ID_COMMAND_TOGGLEMAXIMIZE_3DVIEW, _T("Toggle &maximize 3D View\tCtrl-X"));
    windowMenu->Append(ID_COMMAND_SHOWHIDE_TOOLS, _T("Show/hide &Tools\tCtrl-T"));
    windowMenu->Append(ID_COMMAND_SHOWHIDE_PRESETS, _T("Show/hide P&resets\tCtrl-R"));
    windowMenu->Append(ID_COMMAND_SHOWHIDE_PROPERTIES, _T("Show/hide Propert&y sheet\tCtrl-Y"));
    windowMenu->AppendSeparator();
    windowMenu->Append(ID_COMMAND_PERSPECTIVE_SAVE, _T("&Save perspective\tCtrl-Alt-P"));
    windowMenu->Append(ID_COMMAND_PERSPECTIVE_LOAD, _T("&Load saved perspective\tCtrl-P"));
    windowMenu->Append(ID_COMMAND_PERSPECTIVE_RESET, _T("Load &default perspective\tCtrl-Shift-P"));

    SetMenuBar(mainMenuBar);

    mMainToolBar = new UIElementToolBar(_T("maintoolbar"), this, this, mAuiManager);

    mPropertyGrid = new UIElementPropertyGrid(this, wxT("properties"), wxT("Properties"), this, mAuiManager);

    mPresetPanel = new UIElementPresetPanel(this, wxT("presets"), wxT("Presets"), this, mAuiManager);

    mRenderPanel = new RenderPanel(1, this, mEngineInterface, mAuiManager);

    populateMainToolBar();

    if (mAuiManager)
    {
        mDefaultPerspective = mAuiManager->SavePerspective();

        wxCommandEvent dummyCommand;
        onMenuLoadPerspective(dummyCommand);
    }

    // selectTool(_T("HeightfieldOperationCPUBrush"));
}

// ----------------------------------------------------------------------------
MainFrame::~MainFrame()
{
    // clean by wxwidgets:
    //- mRenderPanel,
    //- mMainToolBar,
    //- mPresetPanel

    SAFE_DELETE(mTimer);
    SAFE_DELETE(mAuiManager);

    mEngineInterface->deinitialize();
    SAFE_DELETE(mEngineInterface);

    Sleep(1000); // only here to show console info a bit longer
}

// ----------------------------------------------------------------------------
// HELPER METHODS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void MainFrame::selectTool(const wxString& toolName, int category)
{
    printf("Selecting tool %s\n", toolName.c_str());
    mSelectedToolElementName = toolName;
    mSelectedToolElementGroupId = (ScapeEngine::EScapeUIElementGroupId)category;

    if (mSelectedToolElementGroupId == ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION)
    {
        mEngineInterface->selectOperation(string(toolName));
    }

    populatePropertyGrid();
    populatePresetPanel();
}

// ----------------------------------------------------------------------------
// GENERAL EVENTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void MainFrame::onSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);

    if (mRenderPanel && !mAttachedInputToEngine)
    {
        mEngineInterface->attachInputToWindow(wxString::Format(_T("%d"), GetHandle()).c_str());
        mAttachedInputToEngine = true;
    }
}

// ----------------------------------------------------------------------------
void MainFrame::onRenderTimer(wxTimerEvent& event)
{
    mEngineInterface->update();
    if (mTimer)
        mTimer->Start(1, true);
}

// ----------------------------------------------------------------------------
void MainFrame::onProcessIdle()
{
    mEngineInterface->update();
    if (mTimer && !mTimer->IsRunning())
    {
        mTimer->Start(1, true);
    }
}

// ----------------------------------------------------------------------------
// MENU EVENTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void MainFrame::onMenuExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

// ----------------------------------------------------------------------------
void MainFrame::onMenuToggleMaximize3DView(wxCommandEvent& event)
{
    if (mAuiManager)
    {
        wxAuiPaneInfo& info = mAuiManager->GetPane(mRenderPanel);
        if (info.IsOk())
        {
            if (info.IsMaximized())
            {
                mAuiManager->RestorePane(info);
            }
            else
            {
                mAuiManager->MaximizePane(info);
            }
            mAuiManager->Update();
        }
    }
}

// ----------------------------------------------------------------------------
void MainFrame::onMenuShowHideTools(wxCommandEvent& event)
{
    if (mAuiManager)
    {
        wxAuiPaneInfo& info = mAuiManager->GetPane(mMainToolBar);
        if (info.IsOk())
        {
            info.Show(!info.IsShown());
            mAuiManager->Update();
        }
    }
}

// ----------------------------------------------------------------------------
void MainFrame::onMenuShowHidePresets(wxCommandEvent& event)
{
    if (mAuiManager)
    {
        wxAuiPaneInfo& info = mAuiManager->GetPane(mPresetPanel);
        if (info.IsOk())
        {
            info.Show(!info.IsShown());
            mAuiManager->Update();
        }
    }
}

// ----------------------------------------------------------------------------
void MainFrame::onMenuShowHideProperties(wxCommandEvent& event)
{
    if (mAuiManager)
    {
        wxAuiPaneInfo& info = mAuiManager->GetPane(mPropertyGrid);
        if (info.IsOk())
        {
            info.Show(!info.IsShown());
            mAuiManager->Update();
        }
    }
}

// ----------------------------------------------------------------------------
void MainFrame::onMenuLoadPerspective(wxCommandEvent& event)
{
    if (mAuiManager && mEngineInterface)
    {
        std::pair<void*, size_t> contents = mEngineInterface->readAuxiliaryFile(PERSPECTIVE_FILENAME);
        if (contents.first)
        {
            mAuiManager->LoadPerspective(string((TCHAR*)contents.first));
            delete contents.first;
        }
    }
}

// ----------------------------------------------------------------------------
void MainFrame::onMenuSavePerspective(wxCommandEvent& event)
{
    if (mAuiManager && mEngineInterface)
    {
        string contents = mAuiManager->SavePerspective();
        mEngineInterface->writeAuxiliaryFile(PERSPECTIVE_FILENAME, contents.c_str(), contents.length());
    }
}

// ----------------------------------------------------------------------------
void MainFrame::onMenuResetPerspective(wxCommandEvent& event)
{
    if (mAuiManager && mEngineInterface)
    {
        mAuiManager->LoadPerspective(mDefaultPerspective);
    }
}

// ----------------------------------------------------------------------------
void MainFrame::onMenuImportImageFile(wxCommandEvent& event)
{
    StringStringStringPairMap fileFilterMap
        = mEngineInterface->getFileFilterMap(ScapeEngine::SCAPEUIELEMENTGROUPID_FILEIMPORT);

    string fileFilters;
    StringStringStringPairMap::iterator fileFilterIt, fileFilterItEnd = fileFilterMap.end();
    for (fileFilterIt = fileFilterMap.begin(); fileFilterIt != fileFilterItEnd; ++fileFilterIt)
    {
        string fileFilterLabel = fileFilterIt->second.first.c_str();
        if (!fileFilterLabel.length())
        {
            fileFilterLabel = _T("Importable files");
        }
        fileFilters += wxString::Format(_T("%s (%s)|%s|"), fileFilterLabel.c_str(), fileFilterIt->second.second.c_str(),
            wxString(fileFilterIt->second.second).Lower().c_str());
    }
    fileFilters += wxString::Format(
        _T("All files (%s)|%s"), wxFileSelectorDefaultWildcardStr, wxFileSelectorDefaultWildcardStr);

    static wxString lastFullPath(_T("./"));
    wxString lastPath, lastName;
    wxFileName::SplitPath(lastFullPath, &lastPath, &lastName, NULL);

    wxString fullPath = wxFileSelector(_T("Choose heightfield image to import"), lastPath.c_str(), wxEmptyString,
        wxEmptyString, fileFilters.c_str(), wxFD_OPEN | wxFD_FILE_MUST_EXIST, this);

    if (fullPath.length())
    {
        lastFullPath = fullPath;
        string decoderName = mEngineInterface->getDecoderNameFromExtension(string(fullPath));
        StringList decoderDescs = mEngineInterface->getUIElementNameList(ScapeEngine::SCAPEUIELEMENTGROUPID_FILEIMPORT);

        // Use FileImportType dialog if extension wasn't recognized by the decoders to let the user choose a decoder
        // manually
        if (decoderName.length() == 0)
        {
            static string selectedDecoder;

            FileImportTypeDialog dialog(this, wxID_ANY, decoderDescs, selectedDecoder);

            if (dialog.ShowModal() == wxID_OK)
            {
                decoderName = dialog.getSelectionName();
            }
        }

        if (decoderName.length())
        {
            wxString dialogCaption = decoderName + _T(" import options");

            StringList nameList = mEngineInterface->getUIElementPropertyNameList(
                ScapeEngine::SCAPEUIELEMENTGROUPID_FILEIMPORT, decoderName);
            FileImportOptionsDialogItemMap itemMap;
            StringList::const_iterator nameIt, nameItEnd = nameList.end();
            for (nameIt = nameList.begin(); nameIt != nameItEnd; ++nameIt)
            {
                FileImportOptionsDialogItem item;
                item.label
                    = mEngineInterface->getUIElementPropertyField(decoderName, *nameIt, UIELEMENTPROPERTYNAME_SHORT);
                item.description
                    = mEngineInterface->getUIElementPropertyField(decoderName, *nameIt, UIELEMENTPROPERTYNAME_LONG);
                item.type
                    = mEngineInterface->getUIElementPropertyField(decoderName, *nameIt, UIELEMENTPROPERTYNAME_TYPE);
                item.options
                    = mEngineInterface->getUIElementPropertyField(decoderName, *nameIt, UIELEMENTPROPERTYNAME_OPTIONS);
                itemMap[*nameIt] = item;
            }

            StringStringMap valueMap = mEngineInterface->getUIElementPropertyValueMap(
                ScapeEngine::SCAPEUIELEMENTGROUPID_FILEIMPORT, decoderName);
            struct stat fileStat;
            size_t fileSize = 0;
            if (stat(fullPath, &fileStat) == 0)
                fileSize = (size_t)fileStat.st_size;
            string fileName = fullPath.AfterLast('/').AfterLast('\\');

            FileImportOptionsDialog dialog(this, wxID_ANY, dialogCaption, fileName, fileSize, itemMap, valueMap);

            if (dialog.ShowModal() == wxID_OK)
            {
                mEngineInterface->setUIElementPropertyValueMap(
                    ScapeEngine::SCAPEUIELEMENTGROUPID_FILEIMPORT, decoderName, dialog.getValueMap());
                string error;
                if (!mEngineInterface->importImageFile(decoderName, string(fullPath), &error))
                {
                    wxMessageBox(string(_T("Import failed: ")) + error);
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------
void MainFrame::onMenuExportImageFile(wxCommandEvent& event)
{
    StringList names = mEngineInterface->getUIElementNameList(ScapeEngine::SCAPEUIELEMENTGROUPID_FILEEXPORT);

    FileExportItemList itemList;
    StringList::const_iterator nameIt, nameItEnd = names.end();
    for (nameIt = names.begin(); nameIt != nameItEnd; ++nameIt)
    {
        FileExportItem item;
        item.name = *nameIt;
        item.label = mEngineInterface->getUIElementPropertyField(
            *nameIt, UIELEMENTPROPERTYNAME_LABEL, UIELEMENTPROPERTYNAME_SHORT);
        item.description = mEngineInterface->getUIElementPropertyField(
            *nameIt, UIELEMENTPROPERTYNAME_LABEL, UIELEMENTPROPERTYNAME_LONG);

        itemList.push_back(item);
    }

    static string selectedEncoder;
    static bool flipX = false;
    static bool flipY = false;

    FileExportDialog dialog(this, wxID_ANY, itemList, selectedEncoder, flipX, flipY);

    if (dialog.ShowModal() == wxID_OK)
    {
        selectedEncoder = dialog.getSelectionName();
        flipX = dialog.isFlipXChecked();
        flipY = dialog.isFlipYChecked();

        if (selectedEncoder.length())
        {
            StringStringStringPairMap fileFilterMap
                = mEngineInterface->getFileFilterMap(ScapeEngine::SCAPEUIELEMENTGROUPID_FILEEXPORT);
            std::pair<string, string> fileFilter = fileFilterMap[selectedEncoder];

            static wxString lastFullPath(_T("./"));
            wxString lastPath, lastName;
            wxFileName::SplitPath(lastFullPath, &lastPath, &lastName, NULL);

            wxString fullPath
                = wxFileSelector(_T("Export File"), lastPath, lastName, wxEmptyString,
                    wxString::Format(_T("%s (%s)|%s|All files (%s)|%s"), fileFilter.first.c_str(),
                                     fileFilter.second.c_str(), wxString(fileFilter.second).Lower().c_str(),
                                     wxFileSelectorDefaultWildcardStr, wxFileSelectorDefaultWildcardStr),
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT, this);

            if (fullPath.length())
            {
                lastFullPath = fullPath;
                mEngineInterface->setUIElementPropertyValue(ScapeEngine::SCAPEUIELEMENTGROUPID_FILEEXPORT,
                    selectedEncoder, _T("FLIPX"), string(wxString::Format(_T("%d"), flipX)));
                mEngineInterface->setUIElementPropertyValue(ScapeEngine::SCAPEUIELEMENTGROUPID_FILEEXPORT,
                    selectedEncoder, _T("FLIPY"), string(wxString::Format(_T("%d"), flipY)));
                string error;
                if (!mEngineInterface->exportImageFile(selectedEncoder, string(fullPath), &error))
                {
                    wxMessageBox(string(_T("Import failed: ")) + error);
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------
// TOOLBAR EVENTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void MainFrame::onToolBarItemClicked(const wxString& toolBarName, const wxString& itemName, int itemCategory)
{
    selectTool(itemName, itemCategory);
}

// ----------------------------------------------------------------------------
// PROPERTYGRID EVENTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
wxString MainFrame::onUpdatePropertyGridPropertyValue(
    const wxString& gridIdName, const wxString& propertyName, const wxString& value)
{
    wxString valueOut = mEngineInterface->setUIElementPropertyValue(
        mSelectedToolElementGroupId, mSelectedToolElementName, string(propertyName), string(value));

    return valueOut;
}

// ----------------------------------------------------------------------------
wxString MainFrame::onResetPropertyGridPropertyValue(const wxString& gridIdName, const wxString& propertyName)
{
    string value = mEngineInterface->getUIElementPresetPropertyValue(
        mSelectedToolElementGroupId, mSelectedToolElementName, Utils::defaultNameString, string(propertyName));
    return onUpdatePropertyGridPropertyValue(gridIdName, propertyName, value);
}

// ----------------------------------------------------------------------------
// PRESETPANEL EVENTS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void MainFrame::onPresetPanelSelectPreset(const wxString& presetPanelName, const wxString& presetName)
{
    mPresetToolSelectedItemMap[std::pair<int, string>(mSelectedToolElementGroupId, mSelectedToolElementName)]
        = presetName;
}

// ----------------------------------------------------------------------------
void MainFrame::onPresetPanelLoadPreset(const wxString& presetPanelName, const wxString& presetName)
{
    StringStringMap valueMap = mEngineInterface->getUIElementPresetPropertyValueMap(
        mSelectedToolElementGroupId, mSelectedToolElementName, string(presetName));
    StringStringMap correctValueMap = mEngineInterface->setUIElementPropertyValueMap(
        mSelectedToolElementGroupId, mSelectedToolElementName, valueMap);
    mPropertyGrid->setValues(correctValueMap);
}

// ----------------------------------------------------------------------------
void MainFrame::onPresetPanelSavePreset(const wxString& presetPanelName, const wxString& presetName)
{
    StringStringMap valueMap
        = mEngineInterface->getUIElementPropertyValueMap(mSelectedToolElementGroupId, mSelectedToolElementName);
    mEngineInterface->setUIElementPresetPropertyValueMap(
        mSelectedToolElementGroupId, mSelectedToolElementName, string(presetName), valueMap);
}

// ----------------------------------------------------------------------------
void MainFrame::onPresetPanelAddPreset(const wxString& presetPanelName, const wxString& presetName)
{
    string uniquePresetName = mEngineInterface->makeUniquePresetName(
        mSelectedToolElementGroupId, mSelectedToolElementName, string(presetName));
    StringStringMap valueMap
        = mEngineInterface->getUIElementPropertyValueMap(mSelectedToolElementGroupId, mSelectedToolElementName);
    mEngineInterface->setUIElementPresetPropertyValueMap(
        mSelectedToolElementGroupId, mSelectedToolElementName, string(uniquePresetName), valueMap);
    populatePresetPanel(uniquePresetName, true);
}

// ----------------------------------------------------------------------------
void MainFrame::onPresetPanelDeletePreset(const wxString& presetPanelName, const wxString& presetName)
{
    mEngineInterface->deleteUIElementPreset(mSelectedToolElementGroupId, mSelectedToolElementName, string(presetName));
    populatePresetPanel();
}

// ----------------------------------------------------------------------------
void MainFrame::onPresetPanelChangePresetName(
    const wxString& presetPanelName, const wxString& oldPresetName, const wxString& newPresetName)
{
    StringStringMap valueMap = mEngineInterface->getUIElementPresetPropertyValueMap(
        mSelectedToolElementGroupId, mSelectedToolElementName, string(oldPresetName));
    mEngineInterface->setUIElementPresetPropertyValueMap(
        mSelectedToolElementGroupId, mSelectedToolElementName, string(newPresetName), valueMap);
    mEngineInterface->deleteUIElementPreset(
        mSelectedToolElementGroupId, mSelectedToolElementName, string(oldPresetName));
}

// ----------------------------------------------------------------------------
void MainFrame::onPresetPanelImportPreset(const wxString& presetPanelName, const wxString& fileName)
{
    string presetName = mEngineInterface->importUIElementPreset(
        mSelectedToolElementGroupId, mSelectedToolElementName, string(fileName));
    populatePresetPanel(presetName);
}

// ----------------------------------------------------------------------------
void MainFrame::onPresetPanelExportPreset(const wxString& presetPanelName, const wxString& fileName)
{
    mEngineInterface->exportUIElementPreset(mSelectedToolElementGroupId, mSelectedToolElementName, string(fileName));
}

// ----------------------------------------------------------------------------
void MainFrame::populatePropertyGrid()
{
    if (mPropertyGrid)
    {
        StringList nameList
            = mEngineInterface->getUIElementPropertyNameList(mSelectedToolElementGroupId, mSelectedToolElementName);

        UIElementPropertyGridItemList itemList;

        StringList::iterator nameIt, nameItEnd = nameList.end();

        string propertySetName = mSelectedToolElementName;

        for (nameIt = nameList.begin(); nameIt != nameItEnd; ++nameIt)
        {
            UIElementPropertyGridItem item;
            item.name = *nameIt;
            item.label
                = mEngineInterface->getUIElementPropertyField(propertySetName, *nameIt, UIELEMENTPROPERTYNAME_SHORT);
            item.description
                = mEngineInterface->getUIElementPropertyField(propertySetName, *nameIt, UIELEMENTPROPERTYNAME_LONG);
            item.category
                = mEngineInterface->getUIElementPropertyField(propertySetName, *nameIt, UIELEMENTPROPERTYNAME_CATEGORY);
            item.type
                = mEngineInterface->getUIElementPropertyField(propertySetName, *nameIt, UIELEMENTPROPERTYNAME_TYPE);

            itemList.push_back(item);
        }

        mPropertyGrid->populate(itemList);
        mPropertyGrid->setValues(
            mEngineInterface->getUIElementPropertyValueMap(mSelectedToolElementGroupId, mSelectedToolElementName));
    }
}

// ----------------------------------------------------------------------------
void MainFrame::populatePresetPanel(const string& selectedPreset, bool edit)
{
    if (mPresetPanel)
    {
        std::pair<int, string> selectedItemMapKey(mSelectedToolElementGroupId, mSelectedToolElementName);
        if (selectedPreset.length())
        {
            mPresetToolSelectedItemMap[selectedItemMapKey] = selectedPreset;
        }

        // presetpanel::populate() sets this indirectly, so get presetname before populating
        wxString combinedSelectedPreset = mPresetToolSelectedItemMap[selectedItemMapKey];

        mPresetPanel->populate(
            mEngineInterface->getUIElementPresetPropertyNames(mSelectedToolElementGroupId, mSelectedToolElementName));
        mPresetPanel->selectPreset(combinedSelectedPreset, edit);
    }
}

// ----------------------------------------------------------------------------
void MainFrame::populateMainToolBar()
{
    if (mMainToolBar)
    {
        StringList nameList = mEngineInterface->getUIElementNameList(ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
        UIElementToolBarItemList itemList;
        StringList::iterator nameIt, nameItEnd = nameList.end();
        for (nameIt = nameList.begin(); nameIt != nameItEnd; ++nameIt)
        {
            UIElementToolBarItem item;
            item.name = *nameIt;
            item.label = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_SHORT);
            item.description
                = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_LONG);
            item.icon = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_ICON);
            item.category = (int)ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION;

            itemList.push_back(item);
        }

        nameList = mEngineInterface->getUIElementNameList(ScapeEngine::SCAPEUIELEMENTGROUPID_MATERIAL);
        nameItEnd = nameList.end();
        for (nameIt = nameList.begin(); nameIt != nameItEnd; ++nameIt)
        {
            UIElementToolBarItem item;
            item.name = *nameIt;
            item.label = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_SHORT);
            item.description
                = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_LONG);
            item.icon = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_ICON);
            item.category = (int)ScapeEngine::SCAPEUIELEMENTGROUPID_MATERIAL;

            itemList.push_back(item);
        }

        nameList = mEngineInterface->getUIElementNameList(ScapeEngine::SCAPEUIELEMENTGROUPID_SKY);
        nameItEnd = nameList.end();
        for (nameIt = nameList.begin(); nameIt != nameItEnd; ++nameIt)
        {
            UIElementToolBarItem item;
            item.name = *nameIt;
            item.label = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_SHORT);
            item.description
                = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_LONG);
            item.icon = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_ICON);
            item.category = (int)ScapeEngine::SCAPEUIELEMENTGROUPID_SKY;

            itemList.push_back(item);
        }

        nameList = mEngineInterface->getUIElementNameList(ScapeEngine::SCAPEUIELEMENTGROUPID_RENDERWINDOW);
        nameItEnd = nameList.end();
        for (nameIt = nameList.begin(); nameIt != nameItEnd; ++nameIt)
        {
            UIElementToolBarItem item;
            item.name = *nameIt;
            item.label = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_SHORT);
            item.description
                = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_LONG);
            item.icon = mEngineInterface->getUIElementPropertyField(*nameIt, _T("LABEL"), UIELEMENTPROPERTYNAME_ICON);
            item.category = (int)ScapeEngine::SCAPEUIELEMENTGROUPID_RENDERWINDOW;

            itemList.push_back(item);
        }

        mMainToolBar->populate(itemList);
    }
}
/*/////////////////////////////////////////////////////////////////////////////////
/// This file is part of
///    ___                   _ _ _
///   /___\__ _ _ __ ___  __| (_) |_ ___  _ __
///  //  // _` | '__/ _ \/ _` | | __/ _ \| '__|
/// / \_// (_| | | |  __/ (_| | | || (_) | |
/// \___/ \__, |_|  \___|\__,_|_|\__\___/|_|
///       |___/
///             Copyright (c) 2010 Jacob 'jacmoe' Moen
/// The MIT License
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////*/
#include "MainFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>

#include "renderpanel.h"
#include "wxOgreditor.h"

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title), mRenderPanel(0)

{
    // set the frame icon
    SetIcon(wxICON(sample));

    // create a menu bar
    wxMenu* fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About...\tF1", "Show about dialog");

    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");

    mRenderPanel
        = new RenderPanel(this, RenderPanel::ID_RENDERPANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    mRenderPanel->SetBackgroundColour(wxColour(0, 0, 0));

    readConfig();
}

MainFrame::~MainFrame()
{
    storeConfig();
}

void MainFrame::readConfig()
{
    // restore frame position and size
    int x = 0, y = 0, w = 0, h = 0;
    wxGetApp().GetConfig()->Read(wxT("/MainFrame/x"), &x, wxDefaultPosition.x);
    wxGetApp().GetConfig()->Read(wxT("/MainFrame/y"), &y, wxDefaultPosition.y);
    wxGetApp().GetConfig()->Read(wxT("/MainFrame/w"), &w, 1044);
    wxGetApp().GetConfig()->Read(wxT("/MainFrame/h"), &h, 555);

    Move(x, y);
    SetClientSize(w, h);

    //// Load AUI perspective
    // wxString tmp;
    // if(wxGetApp().GetConfig()->Read(wxT("layout"), &tmp))
    // GetAuiManager().LoadPerspective(tmp, true);
}

void MainFrame::storeConfig()
{
    //    wxGetApp().GetConfig()->Write(wxT("layout"), GetAuiManager().SavePerspective());
    // save the frame position
    int x, y, w, h;
    GetClientSize(&w, &h);
    GetPosition(&x, &y);
    wxGetApp().GetConfig()->Write(wxT("/MainFrame/x"), (long)x);
    wxGetApp().GetConfig()->Write(wxT("/MainFrame/y"), (long)y);
    wxGetApp().GetConfig()->Write(wxT("/MainFrame/w"), (long)w);
    wxGetApp().GetConfig()->Write(wxT("/MainFrame/h"), (long)h);
}

// event handlers

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format("Welcome to %s!\n"
                                  "\n"
                                  "This is the minimal wxWidgets sample\n"
                                  "running under %s.",
                     wxVERSION_STRING, wxGetOsDescription()),
        "About wxWidgets minimal sample", wxOK | wxICON_INFORMATION, this);
}

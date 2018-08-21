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
#pragma once

#include "wx/frame.h"

class RenderPanel;

// Define a new frame type: this is going to be our main frame
class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
    virtual ~MainFrame();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void readConfig();
    void storeConfig();

    RenderPanel* GetRenderPanel() const { return mRenderPanel; }

private:
    RenderPanel* mRenderPanel;
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT
};

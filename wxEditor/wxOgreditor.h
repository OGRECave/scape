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
#ifndef __wxOgreditor_h_
#define __wxOgreditor_h_

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "res/resource.h"
#endif

#include <wx/app.h>
#include <wx/config.h>

#include <OgreRoot.h>

// Define a new application type, each program should derive a class from wxApp
class OgreditorApp : public wxApp
{
    DECLARE_CLASS(OgreditorApp)

public:
    // override base class virtuals
    // ----------------------------

    OgreditorApp();
    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
    virtual int OnExit();
    bool SetupOgre();
    Ogre::Root* GetOgreRoot() const { return m_OgreRoot; }
    wxConfig* GetConfig() const { return mConfig; }

private:
    wxConfig* mConfig;
    Ogre::Root* m_OgreRoot;
};

DECLARE_APP(OgreditorApp)

#endif // #ifndef __wxOgreditor_h_

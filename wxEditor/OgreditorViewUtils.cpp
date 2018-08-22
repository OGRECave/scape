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
/*
-----------------------------------------------------------------------------
This source file is part of wxOgreMVC
Copyright (c) 2007 Torus Knot Software Ltd, all rights reserved

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "wx/wxprec.h"

#include "OgreRenderWindow.h"
#include "OgreRoot.h"
#include "OgreStringConverter.h"
#include "OgreditorViewUtils.h"

#if defined(_MSC_VER)
#include <exception>
#elif defined(__GNUC__)
#include <stdexcept>
#endif

//---------------------------------------------------------------------
Ogre::RenderWindow* ViewUtils::createOgreWindow(const Ogre::String& windowName, wxWindow* win,
                                                const wxSize& sz)
{
    // Confirm Ogre::Root created
    Ogre::Root* proot = Ogre::Root::getSingletonPtr();
    if (!proot)
    {
#if defined(_MSC_VER)
        throw std::exception("No instance of Root exists, cannot create Ogre View");
#elif defined(__GNUC__)
        throw std::runtime_error("No instance of Root exists, cannot create Ogre View");
#endif
    }
    if (!proot->isInitialised())
    {
#if defined(_MSC_VER)
        throw std::exception("Root hasn't been initialised!");
#elif defined(__GNUC__)
        throw std::runtime_error("Root hasn't been initialised!");
#endif
    }

    // --------------------
    // Create a new parameters list according to compiled OS
    Ogre::NameValuePairList params;
    Ogre::String handle;

#ifdef __WXMSW__
    handle = Ogre::StringConverter::toString((size_t)((HWND)(win->GetHandle())));
#elif defined(__WXGTK__)
    GtkWidget* widget = win->GetHandle();
    gtk_widget_realize(widget); // Mandatory. Otherwise, a segfault happens.
    std::stringstream handleStream;

    Display* display = GDK_WINDOW_XDISPLAY(widget->window);
    Window wid = GDK_WINDOW_XWINDOW(
        widget->window); // Window is a typedef for XID, which is a typedef for unsigned int

    /* Get the right display (DisplayString() returns ":display.screen") */
    std::string displayStr = DisplayString(display);
    displayStr = displayStr.substr(1, (displayStr.find(".", 0) - 1));
    int attrlist[] = {GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16, GLX_STENCIL_SIZE, 8, None};
    XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), attrlist);

    // -----------
    // Try either:
    // -----------
    // handleStream << (unsigned long)display << ':' << DefaultScreen( display ) << ':' << wid << ":" <<
    // (unsigned long)vi;

    // ----------------
    // or alternatively
    // ----------------
    handleStream << wid;

    handle = handleStream.str();

    // force xserver to process all tasks and events
    XSync(display, False);
#elif defined(__WXOSX_CARBON__)
    handle = Ogre::StringConverter::toString((unsigned long)((HIViewRef)win->GetHandle()));
#else
// Support __WXOSX__ sometime?
#error Not supported on this platform.
#endif

    params["externalWindowHandle"] = handle;

    // Create the render window
    return proot->createRenderWindow(windowName, sz.x, sz.y, false, &params);
}

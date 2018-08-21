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
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include "wxOgreditor.h"
#include "MainFrame.h"
#include "renderpanel.h"
#include "paths.h"
#include <OgrePlugin.h>

#include <wx/splash.h>

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MainFrame::OnQuit)
    EVT_MENU(Minimal_About, MainFrame::OnAbout)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(OgreditorApp)
IMPLEMENT_CLASS( OgreditorApp, wxApp )

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

OgreditorApp::OgreditorApp()
    : m_OgreRoot(0)
{
}
// 'Main program' equivalent: the program execution "starts" here
bool OgreditorApp::OnInit()
{
#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif

//#ifndef _DEBUG
//    wxString bmppath = getDataPath() + "Images/OgitorLogo.png";
//    wxSplashScreen* splash = new wxSplashScreen(wxBitmap(bmppath, wxBITMAP_TYPE_PNG),
//        wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_NO_TIMEOUT,
//        5000, 0, -1, wxDefaultPosition, wxDefaultSize,
//        wxSIMPLE_BORDER | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP);
//    wxYield();
//#endif

	mConfig = new wxConfig("wxOgreditor");

    SetupOgre();

    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

    // create the main application window
    MainFrame *frame = new MainFrame("Minimal wxWidgets App");


    frame->GetRenderPanel()->SetupOgre();

//#ifndef _DEBUG
//	splash->Destroy();
//#endif


    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

int OgreditorApp::OnExit()
{
    delete m_OgreRoot;
	
	return wxApp::OnExit();
}

bool OgreditorApp::SetupOgre()
{
    // construct Ogre::Root : no plugins filename, no config filename, using a custom log filename
    m_OgreRoot = new Ogre::Root("", "", "wxOgreditor.log");

    // A list of required plugins
    Ogre::StringVector required_plugins;
    required_plugins.push_back("GL RenderSystem");
    required_plugins.push_back("Octree Scene Manager");

    // Load the OpenGL RenderSystem and the Octree SceneManager plugins
#ifdef _DEBUG
    Ogre::StringVector rp_debug;
    rp_debug.push_back("RenderSystem_GL_d");
    rp_debug.push_back("Plugin_OctreeSceneManager_d");
    for (Ogre::StringVector::iterator j = rp_debug.begin(); j != rp_debug.end(); j++)
    {
        m_OgreRoot->loadPlugin(*j);
    }
#else
    Ogre::StringVector rp_release;
    rp_release.push_back("RenderSystem_GL");
    rp_release.push_back("Plugin_OctreeSceneManager");
    for (Ogre::StringVector::iterator j = rp_release.begin(); j != rp_release.end(); j++)
    {
        m_OgreRoot->loadPlugin(*j);
    }
#endif;

    // Check if the required plugins are installed and ready for use
    // If not: exit the application
    Ogre::Root::PluginInstanceList ip = m_OgreRoot->getInstalledPlugins();
    for (Ogre::StringVector::iterator j = required_plugins.begin(); j != required_plugins.end(); j++)
    {
        bool found = false;
        // try to find the required plugin in the current installed plugins
        for (Ogre::Root::PluginInstanceList::iterator k = ip.begin(); k != ip.end(); k++)
        {
            if ((*k)->getName() == *j)
            {
                found = true;
                break;
            }
        }
        if (!found)  // return false because a required plugin is not available
        {
            return false;
        }
    }

//-------------------------------------------------------------------------------------
    // setup resources
    // Only add the minimally required resource locations to load up the Ogre head mesh
    //Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../media/materials/programs", "FileSystem", "General");
    //Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../media/materials/scripts", "FileSystem", "General");
    //Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../media/materials/textures", "FileSystem", "General");
    //Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../media/models", "FileSystem", "General");

//-------------------------------------------------------------------------------------
    // configure
    // Grab the OpenGL RenderSystem, or exit
    Ogre::RenderSystem* rs = m_OgreRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
    if(!(rs->getName() == "OpenGL Rendering Subsystem"))
    {
        return false; //No RenderSystem found
    }
    // configure our RenderSystem
    rs->setConfigOption("Full Screen", "No");
    rs->setConfigOption("VSync", "No");
    rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit");

    m_OgreRoot->setRenderSystem(rs);

	m_OgreRoot->initialise(false);
	Ogre::MovableObject::setDefaultQueryFlags(0);

    return true;
}

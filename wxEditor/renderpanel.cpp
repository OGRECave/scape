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

#include "renderpanel.h"
#include "OgreditorViewUtils.h"

#include <iostream>
using std::cout;
using std::endl;

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/datetime.h>

#include "wxOgreditor.h"

#include <OgreEntity.h>

#ifdef __WXGTK__
#include <gtk/gtkwidget.h>
#include <gdk/gdkx.h>
#include <GL/glx.h>
#endif

const long RenderPanel::ID_RENDERPANEL = wxNewId();

IMPLEMENT_DYNAMIC_CLASS(RenderPanel, wxPanel)

bool ViewKeyboard[1024];

RenderPanel::RenderPanel()
    : m_OgreRoot(0)
    , m_Window(0)
    , m_SceneManager(0)
    , m_ObjCounter(0)
    , m_OgreInitialised(false)
    , m_LastKeyEventTime(0)
    , m_RenderStop(false)
    , m_ScreenResize(false)
{
    Init();
}

RenderPanel::RenderPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : m_OgreRoot(0)
    , m_Window(0)
    , m_SceneManager(0)
    , m_ObjCounter(0)
    , m_OgreInitialised(false)
    , m_LastKeyEventTime(0)
    , m_RenderStop(false)
    , m_ScreenResize(false)
{
    Init();
    Create(parent, id, pos, size, style);
}

bool RenderPanel::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();

    Connect(wxEVT_PAINT, wxPaintEventHandler(RenderPanel::onPaint), NULL, this);
    Connect(wxEVT_SIZE, wxSizeEventHandler(RenderPanel::onSize), NULL, this);
    Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(RenderPanel::OnEraseBackground), NULL, this);

    return true;
}

RenderPanel::~RenderPanel()
{
    Disconnect(wxEVT_PAINT, wxPaintEventHandler(RenderPanel::onPaint), NULL, this);
    Disconnect(wxEVT_SIZE, wxSizeEventHandler(RenderPanel::onSize), NULL, this);
    Disconnect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(RenderPanel::OnEraseBackground), NULL, this);
}

void RenderPanel::Init()
{
    m_FrameCounter = 0;
    m_TotalFrameTime = 0;
    m_CurrentCursor = wxCURSOR_ARROW;
    m_CursorHidden = false;
    m_EventsConnected = false;

    for (int i = 0; i < 1024; i++)
        ViewKeyboard[i] = false;
}

void RenderPanel::CreateControls()
{
    this->SetBackgroundColour(wxColour(0, 0, 0));
}

void RenderPanel::ConnectEvents()
{
    if (m_EventsConnected)
        return;
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(RenderPanel::onKeyDown), NULL, this);
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(RenderPanel::onKeyUp), NULL, this);
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(RenderPanel::onMouseLeftDown), NULL, this);
    Connect(wxEVT_LEFT_UP, wxMouseEventHandler(RenderPanel::onMouseLeftUp), NULL, this);
    Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(RenderPanel::onMouseRightDown), NULL, this);
    Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(RenderPanel::onMouseRightUp), NULL, this);
    Connect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(RenderPanel::onMouseMiddleDown), NULL, this);
    Connect(wxEVT_MIDDLE_UP, wxMouseEventHandler(RenderPanel::onMouseMiddleUp), NULL, this);
    Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(RenderPanel::onMouseWheel), NULL, this);
    Connect(wxEVT_MOTION, wxMouseEventHandler(RenderPanel::onMouseMove), NULL, this);
    Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(RenderPanel::onMouseLeave), NULL, this);
    // Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( RenderPanel::onMouseEvents ), NULL, this );

    // Setup render time
    // long idRenderTimer = wxNewId();
    // m_Timer = new wxTimer(this, idRenderTimer);

    // Connect(idRenderTimer, wxEVT_TIMER, wxTimerEventHandler(RenderPanel::onRenderTimer), NULL, this);

    // m_Timer->Start(10);

    m_EventsConnected = true;
}

void RenderPanel::DisconnectEvents()
{
    if (!m_EventsConnected)
        return;
    Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(RenderPanel::onKeyDown), NULL, this);
    Disconnect(wxEVT_KEY_UP, wxKeyEventHandler(RenderPanel::onKeyUp), NULL, this);
    Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(RenderPanel::onMouseLeftDown), NULL, this);
    Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(RenderPanel::onMouseLeftUp), NULL, this);
    Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(RenderPanel::onMouseRightDown), NULL, this);
    Disconnect(wxEVT_RIGHT_UP, wxMouseEventHandler(RenderPanel::onMouseRightUp), NULL, this);
    Disconnect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(RenderPanel::onMouseMiddleDown), NULL, this);
    Disconnect(wxEVT_MIDDLE_UP, wxMouseEventHandler(RenderPanel::onMouseMiddleUp), NULL, this);
    Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(RenderPanel::onMouseWheel), NULL, this);
    Disconnect(wxEVT_MOTION, wxMouseEventHandler(RenderPanel::onMouseMove), NULL, this);
    Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(RenderPanel::onMouseLeave), NULL, this);
    // Disconnect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( RenderPanel::onMouseEvents ), NULL, this );
    // Disconnect( wxEVT_TIMER, wxTimerEventHandler(RenderPanel::onRenderTimer), NULL, this);
    m_EventsConnected = false;
}

void RenderPanel::SetupOgre(void)
{
    // Get wx control window size
    wxRect rect = GetClientRect();

    m_OgreRoot = wxGetApp().GetOgreRoot();

    m_Window = ViewUtils::createOgreWindow("Ogitor Window", this, wxSize(rect.width, rect.height));
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Load Resources
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    m_OgreRoot->getRenderSystem()->addListener(this);

    m_OgreInitialised = true;

    // Get the SceneManager, in this case the OctreeSceneManager
    Ogre::SceneManager* mSceneMgr = m_OgreRoot->createSceneManager("OctreeSceneManager", "DefaultSceneManager");
    //-------------------------------------------------------------------------------------
    // create camera
    // Create the camera
    Ogre::Camera* mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0, 0, 80));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0, 0, -300));
    mCamera->setNearClipDistance(5);

    //-------------------------------------------------------------------------------------
    // create viewports
    // Create one viewport, entire window
    Ogre::Viewport* vp = m_Window->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    //-------------------------------------------------------------------------------------
    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    //-------------------------------------------------------------------------------------
    // Create any resource listeners (for loading screens)
    // createResourceListener();
    //-------------------------------------------------------------------------------------
    // load resources
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    //-------------------------------------------------------------------------------------
    // Create the scene
    // Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");

    // Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    // headNode->attachObject(ogreHead);

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20, 80, 50);

#if defined(__WXOSX__)
    // Force update to display proper frame
    wxAuiPaneInfo& pane = wxGetApp().GetMainWindow()->GetAuiManager().GetPane(wxT("ID_RENDERPANEL"));

    // Ugly hack follows:
    pane.Float();
    wxGetApp().GetMainWindow()->GetAuiManager().Update();
    pane.Dock();
    wxGetApp().GetMainWindow()->GetAuiManager().Update();
#endif
}

static Ogre::Vector3 oldCamPos = Ogre::Vector3::ZERO;

bool RenderPanel::frameStarted(const Ogre::FrameEvent& evt)
{
    return true;
}

void RenderPanel::onPaint(wxPaintEvent& event)
{
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE
    wxPaintDC dc(this);
#endif

    if ((m_OgreInitialised == true))
    {
        wxRect rect = GetClientRect();
        if (rect.width > 0 && rect.height > 0)
        {
            m_OgreRoot->renderOneFrame();

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            m_Window->update();
            wxGetApp().GetMainWindow()->GetEntityPanel()->GetRenderWindow()->update();
#endif
        }
    }
    else
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        wxPaintDC dc(this);
#endif

        dc.SetBackground(wxBrush(*wxBLACK, wxSOLID));
        dc.Clear();

        wxString msgstr = wxT("Initializing OGRE...");
        if ((m_OgreInitialised == true))
            msgstr = wxT("Please load a Scene File...");
        if (m_RenderStop == true)
            msgstr = wxT("Device Lost...");

        wxSize size = GetClientSize();
        int w, height;
        dc.GetTextExtent(msgstr, &w, &height);
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetTextForeground(*wxWHITE);
        dc.SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial")));
        dc.DrawText(msgstr, (size.x - w) / 2, ((size.y - (height * 5)) / 2));
    }
}

void RenderPanel::onSize(wxSizeEvent& evt)
{
    m_ScreenResize = true;

    wxRect rect = GetClientRect();

    if (m_Window)
    {
// notify "render window" instance
#ifdef __WXGTK__
        m_Window->resize(rect.width, rect.height);
#endif
        m_Window->windowMovedOrResized();

        Refresh();
    }

    m_ScreenResize = false;
}

void RenderPanel::OnEraseBackground(wxEraseEvent& event)
{
}

void RenderPanel::OnTerminateScene()
{
    TerminateScene();
}

Ogre::NameValuePairList testparams;

void RenderPanel::OnAfterLoadScene()
{
}

bool RenderPanel::TerminateScene()
{
    DisconnectEvents();
    m_OgreRoot->getRenderSystem()->removeListener(this);
    Refresh();
    Update();
    return true;
}

void RenderPanel::ShowCursorEx(bool bShow)
{
    if (m_CursorHidden == bShow)
    {
        if (bShow)
            SetCursor(wxCURSOR_ARROW);
        else
            SetCursor(wxCURSOR_BLANK);

        m_CursorHidden = !bShow;
    }
}

void RenderPanel::onKeyDown(wxKeyEvent& evt)
{
    int key = evt.GetKeyCode();
}

void RenderPanel::onKeyUp(wxKeyEvent& evt)
{
}

unsigned int RenderPanel::GetMouseButtons(wxMouseEvent& evt)
{
    unsigned int buttons = 0;
    // if(evt.ButtonIsDown(wxMOUSE_BTN_LEFT))
    //    buttons |= OMB_LEFT;
    // if(evt.ButtonIsDown(wxMOUSE_BTN_RIGHT))
    //    buttons |= OMB_RIGHT;
    // if(evt.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
    //    buttons |= OMB_MIDDLE;
    return buttons;
}

void RenderPanel::onMouseMove(wxMouseEvent& evt)
{
    Ogre::Vector2 pos(evt.GetPosition().x, evt.GetPosition().y);

    if (FindFocus() != this)
        SetFocus();
}

void RenderPanel::onMouseLeave(wxMouseEvent& evt)
{
    Ogre::Vector2 pos(evt.GetPosition().x, evt.GetPosition().y);
}

void RenderPanel::onMouseLeftDown(wxMouseEvent& evt)
{
    SetFocus();
    Ogre::Vector2 pos(evt.GetPosition().x, evt.GetPosition().y);
}

void RenderPanel::onMouseLeftUp(wxMouseEvent& evt)
{
    Ogre::Vector2 pos(evt.GetPosition().x, evt.GetPosition().y);
}

void RenderPanel::onMouseRightDown(wxMouseEvent& evt)
{
    SetFocus();
    Ogre::Vector2 pos(evt.GetPosition().x, evt.GetPosition().y);
}

void RenderPanel::onMouseRightUp(wxMouseEvent& evt)
{
    Ogre::Vector2 pos(evt.GetPosition().x, evt.GetPosition().y);
}

void RenderPanel::onMouseMiddleDown(wxMouseEvent& evt)
{
    SetFocus();
    Ogre::Vector2 pos(evt.GetPosition().x, evt.GetPosition().y);
}

void RenderPanel::onMouseMiddleUp(wxMouseEvent& evt)
{
    Ogre::Vector2 pos(evt.GetPosition().x, evt.GetPosition().y);
}

void RenderPanel::onMouseWheel(wxMouseEvent& evt)
{
    int zDelta = evt.GetWheelRotation();

    Ogre::Vector2 pos(evt.GetPosition().x, evt.GetPosition().y);
}

void RenderPanel::onRenderTimer(wxTimerEvent& evet)
{
    // Ogre::LogManager::getSingleton().getDefaultLog()->logMessage("Timer Event");
    if ((m_OgreInitialised == true) /*&& (wxGetApp().GetOgitorRoot()->IsSceneLoaded() == true)*/
        && (m_RenderStop == false))
    {
        // Ogre::LogManager::getSingleton().getDefaultLog()->logMessage("Render");
        // m_OgreRoot->renderOneFrame();
    }
}

void RenderPanel::DisplayFPS(float time)
{
}

void RenderPanel::SetCameraMode(Ogre::PolygonMode mode)
{
}

void RenderPanel::ProcessKeyActions()
{
}

void RenderPanel::ReloadUserResources()
{
}

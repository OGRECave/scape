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
#ifndef __renderpanel_h__431D421B_ADFE_44b2_B788_1E94A4A1A418
#define __renderpanel_h__431D421B_ADFE_44b2_B788_1E94A4A1A418

extern bool ViewKeyboard[1024];

// control identifiers
#define SYMBOL_SCENEVIEW_STYLE wxSUNKEN_BORDER | wxTAB_TRAVERSAL
#define SYMBOL_SCENEVIEW_IDNAME ID_RENDERPANEL
#define SYMBOL_SCENEVIEW_SIZE wxDefaultSize
#define SYMBOL_SCENEVIEW_POSITION wxDefaultPosition

#include <Ogre.h>
#include <wx/msw/winundef.h>
#include <wx/panel.h>
#include <wx/timer.h>

class RenderPanel : public wxPanel, public Ogre::FrameListener, public Ogre::RenderSystem::Listener
{
    DECLARE_DYNAMIC_CLASS(RenderPanel)

protected:
    virtual void eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters = 0)
    {
        if (m_ScreenResize)
            return;
        if (eventName == "DeviceLost")
            m_RenderStop = true;
        Ogre::LogManager::getSingleton().logMessage("RENDERSYSTEM: " + eventName);
    };

    unsigned int m_ObjCounter;
    unsigned int m_LastKeyEventTime;
    bool m_ScreenResize;
    int m_FrameCounter;
    double m_TotalFrameTime;
    unsigned int m_CurrentCursor;
    bool m_CursorHidden;
    bool m_EventsConnected;

    wxTimer* m_Timer; // Render Timer

    void OnTerminateScene();
    void OnAfterLoadScene();
    void OnUpdate(float time){};

    void DisplayFPS(float time);

public:
    static const long ID_RENDERPANEL;

    Ogre::Root* m_OgreRoot;
    Ogre::RenderWindow* m_Window;
    Ogre::SceneManager* m_SceneManager;
    bool m_OgreInitialised;
    bool m_RenderStop;
    float m_FrameRate;

    void SetupOgre(void);
    void SetCameraMode(Ogre::PolygonMode);
    bool frameStarted(const Ogre::FrameEvent& evt);
    void ProcessKeyActions();
    void ReloadUserResources();
    bool TerminateScene();
    void ShowCursorEx(bool bShow);

    /// Constructors
    RenderPanel();
    RenderPanel(wxWindow* parent, wxWindowID id = ID_RENDERPANEL, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxSUNKEN_BORDER | wxTAB_TRAVERSAL);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = ID_RENDERPANEL, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxSUNKEN_BORDER | wxTAB_TRAVERSAL);

    /// Destructor
    ~RenderPanel();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

protected:
    /** Painting event callback.
         @param evt Data regarding the painting event.
         */

    void onPaint(wxPaintEvent& event);

    void OnEraseBackground(wxEraseEvent& event);

    /** Resizing events callback.
     @param evt Data regarding the resize event.
     */
    void onSize(wxSizeEvent& evt);

    /** Keyboard events callback.
     @remarks This will handle Key Down Event.
     @param evt Data regarding the key event.
     */
    void onKeyDown(wxKeyEvent& evt);

    /** Keyboard events callback.
     @remarks This will handle Key Up Event.
     @param evt Data regarding the key event.
     */
    void onKeyUp(wxKeyEvent& evt);

    /** Mouse events callback.
     @remarks This will handle Mouse Move Event.
     @param evt Data regarding the mouse event.
     */
    void onMouseMove(wxMouseEvent& evt);

    /** Mouse events callback.
     @remarks This will handle Mouse Leave Event.
     @param evt Data regarding the mouse event.
     */
    void onMouseLeave(wxMouseEvent& evt);

    /** Mouse events callback.
     @remarks This will handle Left Mouse Down Event.
     @param evt Data regarding the mouse event.
     */
    void onMouseLeftDown(wxMouseEvent& evt);

    /** Mouse events callback.
     @remarks This will handle Left Mouse Up Event.
     @param evt Data regarding the mouse event.
     */
    void onMouseLeftUp(wxMouseEvent& evt);

    /** Mouse events callback.
     @remarks This will handle Right Mouse Down Event.
     @param evt Data regarding the mouse event.
     */
    void onMouseRightDown(wxMouseEvent& evt);

    /** Mouse events callback.
     @remarks This will handle Middle Mouse Down Event.
     @param evt Data regarding the mouse event.
     */
    void onMouseMiddleDown(wxMouseEvent& evt);

    /** Mouse events callback.
     @remarks This will handle Middle Mouse Up Event.
     @param evt Data regarding the mouse event.
     */
    void onMouseMiddleUp(wxMouseEvent& evt);

    /** Mouse events callback.
     @remarks This will handle Right Mouse Up Event.
     @param evt Data regarding the mouse event.
     */
    void onMouseRightUp(wxMouseEvent& evt);

    /** Mouse events callback.
     @remarks This will handle Mouse Wheel Event.
     @param evt Data regarding the mouse event.
     */
    void onMouseWheel(wxMouseEvent& evt);

    void onRenderTimer(wxTimerEvent& evet);

    void ConnectEvents();

    void DisconnectEvents();

    unsigned int GetMouseButtons(wxMouseEvent& evt);
};

#endif // __renderpanel_h__431D421B_ADFE_44b2_B788_1E94A4A1A418

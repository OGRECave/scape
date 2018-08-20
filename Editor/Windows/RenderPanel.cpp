/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"
#include "RenderPanel.h"

#define MINIMAL_SIZE_RENDERVIEW 32

using namespace ScapeEditor;

// Required for WX
IMPLEMENT_CLASS(RenderPanel, wxPanel)

// Required for WX
BEGIN_EVENT_TABLE(RenderPanel, wxPanel)
EVT_RIGHT_DOWN(RenderPanel::onClick)
EVT_LEFT_DOWN(RenderPanel::onClick)
EVT_SIZE(RenderPanel::onSize)
EVT_MOVE(RenderPanel::onMove)
EVT_SET_FOCUS(RenderPanel::onSetFocus)
EVT_KILL_FOCUS(RenderPanel::onKillFocus)
EVT_IDLE(RenderPanel::onIdle)
EVT_ENTER_WINDOW(RenderPanel::onEnterWindow)
EVT_LEAVE_WINDOW(RenderPanel::onLeaveWindow)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
RenderPanel::RenderPanel(
    int viewId, wxWindow* parent, ScapeEngine::EngineInterface* engineInterface, wxAuiManager* auiManager)
    : wxPanel(parent, wxID_ANY)
    , mViewId(viewId)
    , mHasView(false)
    , mHasFocus(false)
    , mViewRect(0, 0, 0, 0)
    , mPreviousViewRect(0, 0, 0, 0)
    , mEngineInterface(engineInterface)
    , mAuiManager(auiManager)
{
    SetWindowStyle(wxNO_BORDER);
    SetSize(0, 0, -1, -1);

    if (mAuiManager)
    {
        mAuiManager->AddPane(this, wxAuiPaneInfo()
                                       .Name(wxString::Format(_T("RenderPanel%d"), mViewId))
                                       .Dockable(false)
                                       .Movable(false)
                                       .Center()
                                       .Layer(0)
                                       .CloseButton(false)
                                       .MaximizeButton(true)
                                       .BestSize(wxSize(640, 480))
                                       .MinSize(wxSize(32, 32)));
    }
}

// ----------------------------------------------------------------------------
void RenderPanel::onSize(wxSizeEvent& event)
{
    updateRect();
}

// ----------------------------------------------------------------------------
void RenderPanel::onMove(wxMoveEvent& event)
{
    updateRect();
}

// ----------------------------------------------------------------------------
void RenderPanel::onSetFocus(wxFocusEvent& event)
{
    if (mHasView)
    {
        mEngineInterface->onRenderViewSetFocus(mViewId);
    }
    mHasFocus = true;
}

// ----------------------------------------------------------------------------
void RenderPanel::onKillFocus(wxFocusEvent& event)
{
    if (mHasView)
    {
        mEngineInterface->onRenderViewKillFocus(mViewId);
    }
    mHasFocus = false;
}

// ----------------------------------------------------------------------------
void RenderPanel::onIdle(wxIdleEvent& event)
{
    if (mViewRect != mPreviousViewRect)
    {
        if (mViewRect.width >= MINIMAL_SIZE_RENDERVIEW && mViewRect.height >= MINIMAL_SIZE_RENDERVIEW)
        {
            if (!mHasView)
            {
                wxString windowHandle;
                windowHandle << (long)GetHandle();

                mEngineInterface->createRenderView(
                    mViewId, windowHandle.c_str(), mViewRect.x, mViewRect.y, mViewRect.width, mViewRect.height);
                mHasView = true;
                if (mHasFocus)
                {
                    wxFocusEvent event;
                    onSetFocus(event);
                }
            }
            else
            {
                mEngineInterface->onRenderViewMovedOrResized(
                    mViewId, mViewRect.x, mViewRect.y, mViewRect.width, mViewRect.height);
                mPreviousViewRect = mViewRect;
            }
        }
    }
}

// ----------------------------------------------------------------------------
void RenderPanel::onClick(wxMouseEvent& event)
{
    SetFocus();
}

// ----------------------------------------------------------------------------
void RenderPanel::onEnterWindow(wxMouseEvent& event)
{
    // SetFocus();
}

// ----------------------------------------------------------------------------
void RenderPanel::onLeaveWindow(wxMouseEvent& event)
{
}

// ----------------------------------------------------------------------------
void RenderPanel::updateRect()
{
    wxPoint location = wxGetTopLevelParent(this)->GetClientAreaOrigin()
        + ::wxGetTopLevelParent(this)->ScreenToClient(ClientToScreen(wxPoint(0, 0)));
    wxSize size = GetClientSize();

    mViewRect = wxRect(location.x, location.y, size.x, size.y);
}
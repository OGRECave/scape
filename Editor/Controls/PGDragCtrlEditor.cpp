/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"

#include <wx/propgrid/propgrid.h>		// Main propertygrid header.
#include <wx/propgrid/propdev.h>		// Needed for implementing custom properties.
#include <wx/propgrid/advprops.h>		// Extra property classes.
#include <wx/propgrid/manager.h>		// This defines wxPropertyGridManager.
#include "PGDragCtrlEditor.h"


// ----------------------------------------------------------------------------
using namespace ScapeEditor;


// ----------------------------------------------------------------------------
class DragButton : public wxSpinButton
{
	DECLARE_CLASS(DragButton)
public:
	DragButton();

	int GetScrollDistance() {return mScrollDistance;}

protected:
	DECLARE_EVENT_TABLE()

	bool mIsDragging;
	wxPoint mLastMousePosition;
	int mScrollDistance;

	void onLeftUp(wxMouseEvent& event);
	void onLeftDown(wxMouseEvent& event);
	void onRightUp(wxMouseEvent& event);
	void onMouseMotion(wxMouseEvent& event);
	void onMouseCaptureLost(wxMouseCaptureLostEvent& event);
};

IMPLEMENT_CLASS(DragButton, wxWindow)

BEGIN_EVENT_TABLE(DragButton, wxSpinButton)
    EVT_LEFT_UP(DragButton::onLeftUp)
    EVT_LEFT_DOWN(DragButton::onLeftDown)
	EVT_RIGHT_UP(DragButton::onRightUp)
	EVT_MOTION(DragButton::onMouseMotion)
	EVT_MOUSE_CAPTURE_LOST(DragButton::onMouseCaptureLost)
END_EVENT_TABLE()

DragButton::DragButton()
:	mIsDragging(false),
	mScrollDistance(0),
	mLastMousePosition(0, 0)
{
}


// ----------------------------------------------------------------------------
void DragButton::onLeftDown(wxMouseEvent& event) 
{
	mIsDragging = true;
	mLastMousePosition = ::wxGetMousePosition();
	CaptureMouse();
}


// ----------------------------------------------------------------------------
void DragButton::onLeftUp(wxMouseEvent& event) 
{
	if (mIsDragging)
	{
		ReleaseMouse();
	}
	mIsDragging = false;
}


// ----------------------------------------------------------------------------
void DragButton::onRightUp(wxMouseEvent& event) 
{
	mScrollDistance = 0;

	wxNotifyEvent scrollEvent(wxEVT_SCROLL_CHANGED, m_windowId);
	scrollEvent.SetEventObject(this);
	GetEventHandler()->ProcessEvent(scrollEvent);
}


// ----------------------------------------------------------------------------
void DragButton::onMouseMotion(wxMouseEvent& event) 
{
	if (mIsDragging)
	{
		wxPoint mousePosition = ::wxGetMousePosition();

		wxRect displayRect = ::wxGetClientDisplayRect();

		long deltaY = mousePosition.y - mLastMousePosition.y;
		mLastMousePosition = mousePosition;

		if (deltaY == 0)
		{
			if (mousePosition.y <= displayRect.GetTop() + 1) --deltaY;
			else if (mousePosition.y >= displayRect.GetBottom() -+ 1) ++deltaY;
		}

		int deltaValue = (int)(- deltaY);

		if (deltaValue)
		{
			mScrollDistance = deltaValue;

			wxSpinEvent scrollEvent(wxEVT_SCROLL_CHANGED, m_windowId);
			scrollEvent.SetEventObject(this);
			GetEventHandler()->ProcessEvent(scrollEvent);
		}

		mScrollDistance = 0;
	}
}


// ----------------------------------------------------------------------------
void DragButton::onMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
	mIsDragging = false;
}


// ----------------------------------------------------------------------------
WX_PG_IMPLEMENT_EDITOR_CLASS(DragCtrl,PGDragCtrlEditor,wxPGEditor)


// ----------------------------------------------------------------------------
wxWindow* PGDragCtrlEditor::CreateControls( wxPropertyGrid* propgrid, wxPGProperty* property,
                                              const wxPoint& pos, const wxSize& sz, wxWindow** pSecondary ) const
{
    const int margin = 1;
    wxSize butSz(sz.y, sz.y);
    wxSize tcSz(sz.x - butSz.x - margin, sz.y);
    wxPoint butPos(pos.x + tcSz.x + margin, pos.y);

    DragButton* wnd2 = new DragButton();

#ifdef __WXMSW__
    wnd2->Hide();
#endif
    wnd2->Create( propgrid, wxPG_SUBID2, butPos, butSz, wxSP_VERTICAL | wxSP_ARROW_KEYS);
    wnd2->SetRange( INT_MIN, INT_MAX );
    //wnd2->SetRange( 5, 12 );
    wnd2->SetValue( 0 );

	propgrid->Connect( wxPG_SUBID2, wxEVT_SCROLL_CHANGED,
                       (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
                       &wxPropertyGrid::OnCustomEditorEvent, NULL, propgrid );

    // Let's add validator to make sure only numbers can be entered
    wxString temps;
    wxTextValidator validator(wxFILTER_NUMERIC, &temps);

    wxTextCtrl* wnd1 = (wxTextCtrl*) wxPGTextCtrlEditor::CreateControls( propgrid, property, pos, tcSz, NULL );
    wnd1->SetValidator(validator);

    *pSecondary = wnd2;
    return wnd1;
}


// ----------------------------------------------------------------------------
bool PGDragCtrlEditor::OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property,
                                  wxWindow* wnd, wxEvent& event ) const
{
    int evtType = event.GetEventType();

    if (evtType == wxEVT_SCROLL_CHANGED)
    {
        wxString s;
        // Can't use wnd since it might be clipper window
        wxTextCtrl* tc = wxDynamicCast(propgrid->GetEditorControl(), wxTextCtrl);

        if ( tc )
            s = tc->GetValue();
        else
            s = property->GetValueAsString(wxPG_FULL_VALUE);

        DragButton* dragButton = (DragButton*) propgrid->GetEditorControlSecondary();
        int dragMin = dragButton->GetMin();
        int dragMax = dragButton->GetMax();

		if (evtType == wxEVT_SCROLL_CHANGED)
		{
			if ( property->GetValueType() == wxPG_VALUETYPE(double) )
			{
				double v_d;

				// Try double
				if ( s.ToDouble(&v_d) )
				{
					v_d += dragButton->GetScrollDistance();

					// Min/Max
					double dDragMin = (double) dragMin;
					double dDragMax = (double) dragMax;
					if ( v_d > dDragMax ) v_d = dDragMax;
					else if ( v_d < dDragMin ) v_d = dDragMin;
	                
					wxPropertyGrid::DoubleToString(s, v_d, 6, true, NULL);
				}
				else
				{
					return false;
				}
			}
			else
			{
				long v_l;

				// Try long
				if ( s.ToLong(&v_l, 0) )
				{
					v_l += dragButton->GetScrollDistance();

					// Min/Max
					if ( v_l > dragMax ) v_l = dragMax;
					else if ( v_l < dragMin ) v_l = dragMin;

					s = wxString::Format(_T("%i"),(int)v_l);
				}
				else
				{
					return false;
				}
			}
		}

        if ( tc )
            tc->SetValue(s);

        return true;
    }

    return wxPGTextCtrlEditor::OnEvent(propgrid,property,wnd,event);
}


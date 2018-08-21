/**
 * ScapeEditor::RenderPanel class
 *
 * Renders a 3D rendered view inside the control
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#ifndef __RENDERPANEL_H__
#define __RENDERPANEL_H__

#include "EngineInterface.h"

// ----------------------------------------------------------------------------
namespace ScapeEditor
{
	// ----------------------------------------------------------------------------
	class RenderPanel : public wxPanel
	{
		DECLARE_CLASS(RenderPanel);

	public:
		RenderPanel(int viewId, wxWindow* parent, ScapeEngine::EngineInterface* engineInterface, wxAuiManager* auiManager);

	protected:
		DECLARE_EVENT_TABLE()

	private:
		ScapeEngine::EngineInterface* mEngineInterface;
		wxAuiManager* mAuiManager;
		int mViewId;
		bool mHasFocus;
		bool mHasView;
		wxRect mViewRect;
		wxRect mPreviousViewRect;

		void onClick(wxMouseEvent &event);
		void onSize(wxSizeEvent &event);
		void onMove(wxMoveEvent &event);
		void onPaint(wxPaintEvent &event);
		void onSetFocus(wxFocusEvent &event);
		void onKillFocus(wxFocusEvent &event);
		void onIdle(wxIdleEvent &event);
		void onEnterWindow(wxMouseEvent &event);
		void onLeaveWindow(wxMouseEvent &event);

		void updateRect();

	public:
	};

}

#endif // __RENDERPANEL_H__

/**
 * ScapeEditor::UIElementPropertyGrid class
 *
 * Property grid container, supporting importing and exporting as wxString maps
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __PGDDRAGCTRLEDITORD_H__
#define __PGDDRAGCTRLEDITORD_H__

#include "EngineInterface.h"
#include <wx/spinbutt.h>

namespace ScapeEditor
{
class PGDragCtrlEditor : public wxPGTextCtrlEditor
{
    WX_PG_DECLARE_EDITOR_CLASS()
public:
    virtual ~PGDragCtrlEditor() {}

    // See below for short explanations of what these are suppposed to do.
    wxPG_DECLARE_CREATECONTROLS

        virtual bool
        OnEvent(wxPropertyGrid* propgrid, wxPGProperty* property, wxWindow* wnd, wxEvent& event) const;
};
}

WX_PG_DECLARE_EDITOR(DragCtrl)

#endif // __PGDDRAGCTRLEDITORD_H__

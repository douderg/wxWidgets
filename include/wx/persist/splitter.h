///////////////////////////////////////////////////////////////////////////////
// Name:        wx/persist/splitter.h
// Purpose:     Persistence support for wxSplitterWindow.
// Author:      Vadim Zeitlin
// Created:     2011-08-31
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PERSIST_SPLITTER_H_
#define _WX_PERSIST_SPLITTER_H_

#include "wx/persist/window.h"

#include "wx/splitter.h"

// ----------------------------------------------------------------------------
// string constants used by wxPersistentSplitter
// ----------------------------------------------------------------------------

#define wxPERSIST_SPLITTER_KIND wxASCII_STR("Splitter")

// Special position value of -1 means the splitter is not split at all.
#define wxPERSIST_SPLITTER_POSITION wxASCII_STR("Position")

#define wxPERSIST_SPLITTER_DEFAULT_HORIZONTAL wxASCII_STR("LastHorz")
#define wxPERSIST_SPLITTER_DEFAULT_VERTICAL wxASCII_STR("LastVert")

// ----------------------------------------------------------------------------
// wxPersistentSplitter: supports saving/restoring splitter position
// ----------------------------------------------------------------------------

class wxPersistentSplitter : public wxPersistentWindow<wxSplitterWindow>
{
public:
    wxPersistentSplitter(wxSplitterWindow* splitter)
        : wxPersistentWindow<wxSplitterWindow>(splitter)
    {
    }

    virtual void Save() const override
    {
        wxSplitterWindow* const splitter = Get();

        wxPoint lastSplitPos = splitter->GetLastSplitPosition();
        int pos = splitter->IsSplit() ? splitter->GetSashPosition() : -1;
        SaveValue(wxPERSIST_SPLITTER_POSITION, pos);
        SaveValue(wxPERSIST_SPLITTER_DEFAULT_HORIZONTAL, lastSplitPos.y);
        SaveValue(wxPERSIST_SPLITTER_DEFAULT_VERTICAL, lastSplitPos.x);
    }

    virtual bool Restore() override
    {
        int pos;
        if ( !RestoreValue(wxPERSIST_SPLITTER_POSITION, &pos) )
            return false;

        int horizontal, vertical;
        if (!RestoreValue(wxPERSIST_SPLITTER_DEFAULT_HORIZONTAL, &horizontal))
            return false;
        if (!RestoreValue(wxPERSIST_SPLITTER_DEFAULT_VERTICAL, &vertical))
            return false;

        if ( pos == -1 )
        {
            Get()->Unsplit();
            Get()->SetLastSplitPosition(vertical, horizontal);
        }
        else
            Get()->SetSashPosition(pos);

        return true;
    }

    virtual wxString GetKind() const override { return wxPERSIST_SPLITTER_KIND; }
};

inline wxPersistentObject *wxCreatePersistentObject(wxSplitterWindow* splitter)
{
    return new wxPersistentSplitter(splitter);
}

#endif // _WX_PERSIST_SPLITTER_H_

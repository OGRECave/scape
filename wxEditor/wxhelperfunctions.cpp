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
#include "wxhelperfunctions.h"

#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/filesys.h>
#include <wx/dir.h>

bool MakeDirectory(wxString dirname) { return wxMkdir(dirname, 0777); }

void CleanPath(wxString& path)
{
    // will this do?
    path.Replace(wxT("//"), wxT("/"));
    path.Replace(wxT("*"), wxT(""));
    wxFileName p = wxFileName(path);
    p.Normalize(wxPATH_NORM_DOTS);
    path = p.GetPath();
}

wxString ExtractFileName(wxString path)
{
    wxFileName p = wxFileName(path);
    return p.GetFullName();
}

wxString ExtractFilePath(wxString filename)
{
    wxFileName p = wxFileName(filename);
    return p.GetPath();
}

wxString ConvertToWxString(Ogre::String val) { return wxString(val.c_str()); }

Ogre::String ConvertToOgreString(wxString val) { return Ogre::String(val.c_str()); }

wxString GetRelativePath(const wxString pathFrom, const wxString pathTo, bool isfile,
                         wxString PathToModifier)
{
    wxFileName p;
    if (isfile)
    {
        p = wxFileName(pathFrom);
    }
    else
    {
        wxString tmpFilename = p.CreateTempFileName(pathFrom);
        p = wxFileName(pathFrom, tmpFilename);
    }

    p.MakeRelativeTo(pathTo);
    return p.GetPath();
}

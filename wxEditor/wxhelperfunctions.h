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
#ifndef __wxhelperfunctions_h__431D421B_ADFE_44b2_B788_1E94A4A1A418
#define __wxhelperfunctions_h__431D421B_ADFE_44b2_B788_1E94A4A1A418

#include <OgrePrerequisites.h>
#include <OgreString.h>
#include <wx/string.h>

bool MakeDirectory(wxString dirname);
void CleanPath(wxString& path);
wxString ExtractFileName(wxString path);
wxString ExtractFilePath(wxString filename);
wxString ConvertToWxString(Ogre::String val);
Ogre::String ConvertToOgreString(wxString val);
wxString GetRelativePath(const wxString pathFrom, const wxString pathTo, bool isfile,
                         wxString PathToModifier = wxT(""));

#endif // __wxhelperfunctions_h__431D421B_ADFE_44b2_B788_1E94A4A1A418

/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "Utils.h"

using namespace ScapeEditor;

const string Utils::emptyString = _T("");
const string Utils::defaultNameString = _T("Default");

// ----------------------------------------------------------------------------
wxBitmap Utils::getBitmap(const string& resourceName)
{
	wxBitmap bitmap;
#ifdef __WXMSW__
	bitmap.LoadFile(resourceName, wxBITMAP_TYPE_RESOURCE);
#else // __WXMSW__
	#error Resource loading not implemented for this platform
#endif // __WXMSW__
	return bitmap;
}


// ----------------------------------------------------------------------------
wxIcon Utils::getIcon(const string& resourceName)
{
	wxIcon icon;
#ifdef __WXMSW__
	icon.LoadFile(resourceName, wxBITMAP_TYPE_ICO_RESOURCE);
	if (icon.IsOk())
	{
		printf("%s\n", resourceName.c_str());

	}
#else // __WXMSW__
	#error Resource loading not implemented for this platform
#endif // __WXMSW__
	return icon;
}


// ----------------------------------------------------------------------------
bool Utils::isBoolTrue(const wxString& boolString)
{
	const wxChar *chars = boolString.c_str();
	return (*chars && (*chars == _T('1') || *chars == _T('t') || *chars == _T('T') || *chars == _T('y')) || *chars == _T('Y'));
}


// ----------------------------------------------------------------------------
bool Utils::isBoolTrue(const string& boolString)
{
	const wxChar *chars = boolString.c_str();
	return (*chars && (*chars == _T('1') || *chars == _T('t') || *chars == _T('T') || *chars == _T('y')) || *chars == _T('Y'));
}

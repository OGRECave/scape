/**
 * ScapeEditor::Utils class
 *
 * Collection of useful constants, macros, templates and functions
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __SCAPEEDITOR_UTILS_H__
#define __SCAPEEDITOR_UTILS_H__

#include <wx/wx.h>

namespace ScapeEditor
{
	typedef std::list<string> StringList;
	typedef std::map<string, string> StringStringMap;
	typedef std::map<string, StringList> StringStringListMap;
	typedef std::map<string, std::pair<string, string>> StringStringStringPairMap;

	namespace Utils
	{
		wxBitmap getBitmap(const string& resourceName);

		wxIcon getIcon(const string& resourceName);

		extern const string emptyString;

		extern const string defaultNameString;

		bool isBoolTrue(const wxString& boolString);
		bool isBoolTrue(const string& boolString);
	}
}

#endif // __SCAPEEDITOR_UTILS_H__

/**
 * Enumeration of all virtual buttons
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __BUTTONID_H__
#define __BUTTONID_H__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{
namespace ButtonId
{
// EButtonId enum.
#define ENUMID(a) a,
enum EButtonId
{
    BUTTONID_UNKNOWN,
#include "ButtonId.def"
    BUTTONID_ENUM_LENGTH
};
#undef ENUMID

std::map<EButtonId, std::string> _ScapeEngineExport getButtonIdMap();

// Find the ButtonId from the upper-case name. Returns BUTTONID_UNKNOWN otherwise
EButtonId _ScapeEngineExport getButtonIdFromUpperName(const string& buttonName);

// Find the ButtonId from the case-insensitive name. Returns BUTTONID_UNKNOWN otherwise
EButtonId _ScapeEngineExport getButtonIdFromName(const string& buttonName);

// Get the upper-case string of the ButtonId.
string _ScapeEngineExport getButtonIdToUpperName(EButtonId button);
}
}

#endif //__BUTTONID_H__
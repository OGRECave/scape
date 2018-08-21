/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ButtonId.h"
#include "PCH/stdafx.h"

using namespace ScapeEngine;

// ----------------------------------------------------------------------------
ButtonId::EButtonId ButtonId::getButtonIdFromUpperName(const string& buttonName)
{
#define ENUMID(a)                                                                                          \
    {                                                                                                      \
        string name(#a);                                                                                   \
        std::transform(name.begin(), name.end(), name.begin(), toupper);                                   \
        table[name] = a;                                                                                   \
    }
#define ENUM_OIS_KEYBOARD(a) a
#define ENUM_OIS_MOUSE(a) a
    static std::map<string, EButtonId> table;
    if (table.empty())
    {
#include "ButtonId.def"
    }
#undef ENUMID
#undef ENUM_OIS_KEYBOARD
#undef ENUM_OIS_MOUSE

    std::map<string, EButtonId>::iterator it = table.find(buttonName);
    return it != table.end() ? it->second : BUTTONID_UNKNOWN;
}

// ----------------------------------------------------------------------------
ButtonId::EButtonId ButtonId::getButtonIdFromName(const string& buttonName)
{
    string name(buttonName);
    std::transform(name.begin(), name.end(), name.begin(), toupper);
    return getButtonIdFromUpperName(name);
}

// ----------------------------------------------------------------------------
string ButtonId::getButtonIdToUpperName(ButtonId::EButtonId button)
{
#define ENUMID(a)                                                                                          \
    {                                                                                                      \
        string name(#a);                                                                                   \
        std::transform(name.begin(), name.end(), name.begin(), toupper);                                   \
        table[(size_t)a] = name;                                                                           \
    }
#define ENUM_OIS_KEYBOARD(a) a
#define ENUM_OIS_MOUSE(a) a

    static std::vector<string> table;
    if (table.empty())
    {
        table.resize(BUTTONID_ENUM_LENGTH, _T("BUTTONID_UNKNOWN"));
#include "ButtonId.def"
    }
    return table[(int)button];

#undef ENUMID
#undef ENUM_OIS_KEYBOARD
#undef ENUM_OIS_MOUSE
}

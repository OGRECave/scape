/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "DeviceButtonId.h"

using namespace ScapeEngine;

#if defined(ENUMID)
#error "ENUMID shouldn't be defined at this point"
#endif

std::map<DeviceButtonId::EDeviceButtonId, std::string> DeviceButtonId::getDeviceButtonIdMap()
{
    static std::map<EDeviceButtonId, string> table;

    if (table.empty())
    {
#define ENUMID(a)                                                                                          \
    {                                                                                                      \
        string name(#a);                                                                                   \
        std::transform(name.begin(), name.end(), name.begin(), toupper);                                   \
        table[a] = name;                                                                                   \
    }
#include "DeviceButtonIdOISKeyboard.def"
#include "DeviceButtonIdOISMouse.def"
#undef ENUMID
    }

    return table;
}

string DeviceButtonId::getDeviceButtonIdToUpperName(DeviceButtonId::EDeviceButtonId button)
{
    static std::vector<string> table;

    if (table.empty())
    {
        table.resize(DEVICEBUTTONID_ENUM_LENGTH, _T("DEVICEBUTTONID_UNKNOWN"));
#define ENUMID(a)                                                                                          \
    {                                                                                                      \
        string name(#a);                                                                                   \
        std::transform(name.begin(), name.end(), name.begin(), toupper);                                   \
        table[(size_t)a] = name;                                                                           \
    }
#include "DeviceButtonIdOISKeyboard.def"
#include "DeviceButtonIdOISMouse.def"
#undef ENUMID
    }

    return table[(int)button];
}

DeviceButtonId::EDeviceButtonId DeviceButtonId::getDeviceButtonIdFromUpperName(const string& buttonName)
{
    static std::map<string, EDeviceButtonId> table;

    if (table.empty())
    {
#define ENUMID(a)                                                                                          \
    {                                                                                                      \
        string name(#a);                                                                                   \
        std::transform(name.begin(), name.end(), name.begin(), toupper);                                   \
        table[name] = a;                                                                                   \
    }
#include "DeviceButtonIdOISKeyboard.def"
#include "DeviceButtonIdOISMouse.def"
#undef ENUMID
    }

    std::map<string, EDeviceButtonId>::iterator it = table.find(buttonName);
    return it != table.end() ? it->second : DEVICEBUTTONID_UNKNOWN;
}

DeviceButtonId::EDeviceButtonId DeviceButtonId::getDeviceButtonIdFromName(const string& buttonName)
{
    string name(buttonName);
    std::transform(name.begin(), name.end(), name.begin(), toupper);
    return getDeviceButtonIdFromUpperName(name);
}

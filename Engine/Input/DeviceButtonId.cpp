/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "DeviceButtonId.h"


using namespace ScapeEngine;

#if defined(ENUMID)
#error ENUMID shouldn't be defined at this point
#endif


// ----------------------------------------------------------------------------
string DeviceButtonId::getDeviceButtonIdToUpperName(DeviceButtonId::EDeviceButtonId button)
{
	static std::vector<string> table;

	if (table.empty())
	{
		table.resize(DEVICEBUTTONID_ENUM_LENGTH, _T("DEVICEBUTTONID_UNKNOWN"));
		#define ENUMID(a) {string name(#a); std::transform(name.begin(), name.end(), name.begin(), toupper); table[(size_t)a] = name;}
		#include "DeviceButtonIdOISKeyboard.def"
		#include "DeviceButtonIdOISMouse.def"
		#undef ENUMID
	}

	return table[(int)button];
}


// ----------------------------------------------------------------------------
DeviceButtonId::EDeviceButtonId DeviceButtonId::getDeviceButtonIdFromUpperName(const string& buttonName)
{
	static std::map<string, EDeviceButtonId> table;

	if (table.empty())
	{
		#define ENUMID(a) {string name(#a); std::transform(name.begin(), name.end(), name.begin(), toupper); table[name] = a;}
		#include "DeviceButtonIdOISKeyboard.def"
		#include "DeviceButtonIdOISMouse.def"
		#undef ENUMID
	}

	std::map<string, EDeviceButtonId>::iterator it = table.find(buttonName);
	return it != table.end() ? it->second : DEVICEBUTTONID_UNKNOWN;
}


// ----------------------------------------------------------------------------
DeviceButtonId::EDeviceButtonId DeviceButtonId::getDeviceButtonIdFromName(const string& buttonName)
{
	string name(buttonName);
	std::transform(name.begin(), name.end(), name.begin(), toupper);
	return getDeviceButtonIdFromUpperName(name);
}


// ----------------------------------------------------------------------------
DeviceButtonId::DeviceId::EDeviceId DeviceButtonId::getDeviceButtonIdToDeviceId(DeviceButtonId::EDeviceButtonId button)
{
	if ((int)getDeviceButtonIdToOISKeyCode(button) >= 0) return DeviceButtonId::DeviceId::DEVICEID_OIS_KEYBOARD;
	if ((int)getDeviceButtonIdToOISMouseButtonID(button) >= 0) return DeviceButtonId::DeviceId::DEVICEID_OIS_MOUSE;
	return DeviceButtonId::DeviceId::DEVICEID_NONE;
}


// ----------------------------------------------------------------------------
DeviceButtonId::EDeviceButtonId DeviceButtonId::getDeviceButtonIdFromOISKeyCode(OIS::KeyCode keyCode)
{
	static std::vector<EDeviceButtonId> table;

	if (table.empty())
	{
		#define ENUMID(a) {size_t idx = (size_t)OIS::a; if (idx >= table.size()) {table.resize(idx + 1, DEVICEBUTTONID_UNKNOWN);} table[idx] = a;}
		#include "DeviceButtonIdOISKeyboard.def"
		#undef ENUMID
	}

	size_t idx = (size_t)keyCode;
	return idx < table.size() ? table[idx] : DEVICEBUTTONID_UNKNOWN;
}


// ----------------------------------------------------------------------------
OIS::KeyCode DeviceButtonId::getDeviceButtonIdToOISKeyCode(DeviceButtonId::EDeviceButtonId button)
{
	static std::vector<OIS::KeyCode> table;
	if (table.empty())
	{
		#define ENUMID(a) table[(size_t)a] = OIS::a;
		table.resize(DEVICEBUTTONID_ENUM_LENGTH, (OIS::KeyCode)-1);
		#include "DeviceButtonIdOISKeyboard.def"
		#undef ENUMID
	}

	return table[(size_t)button];
}


// ----------------------------------------------------------------------------
DeviceButtonId::EDeviceButtonId DeviceButtonId::getDeviceButtonIdFromOISMouseButtonID(OIS::MouseButtonID mouseButtonID)
{
	static std::vector<EDeviceButtonId> table;
	if (table.empty())
	{
		#define ENUMID(a) {size_t idx = (size_t)OIS::a; if (idx >= table.size()) {table.resize(idx + 1, DEVICEBUTTONID_UNKNOWN);} table[idx] = a;}
		#include "DeviceButtonIdOISMouse.def"
		#undef ENUMID
	}

	size_t idx = (size_t)mouseButtonID;
	return idx < table.size() ? table[idx] : DEVICEBUTTONID_UNKNOWN;
}


// ----------------------------------------------------------------------------
OIS::MouseButtonID DeviceButtonId::getDeviceButtonIdToOISMouseButtonID(DeviceButtonId::EDeviceButtonId button)
{
	static std::vector<OIS::MouseButtonID> table;
	if (table.empty())
	{
		#define ENUMID(a) table[(size_t)a] = OIS::a;
		table.resize(DEVICEBUTTONID_ENUM_LENGTH, (OIS::MouseButtonID)-1);
		#include "DeviceButtonIdOISMouse.def"
		#undef ENUMID
	}

	return table[(size_t)button];
}

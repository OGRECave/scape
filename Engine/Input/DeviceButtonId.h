/**
 * ScapeEngine::DeviceButtonId class
 *
 * Enumeration of all supported low-level device buttons.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __DEVICEBUTTONID_H__
#define __DEVICEBUTTONID_H__

#include "ScapeEnginePrerequisites.h"

#if defined(ENUMID)
#error "ENUMID shouldn't be defined at this point"
#endif

namespace ScapeEngine
{

namespace DeviceButtonId
{
// EDeviceButton enum.
#define ENUMID(a) a,
enum EDeviceButtonId
{
    DEVICEBUTTONID_UNKNOWN,
//#include "DeviceButtonIdOISKeyboard.def"
//#include "DeviceButtonIdOISMouse.def"
    ENUMID( KC_UNASSIGNED )
    ENUMID( KC_ESCAPE )
    ENUMID( KC_1 )
    ENUMID( KC_2 )
    ENUMID( KC_3 )
    ENUMID( KC_4 )
    ENUMID( KC_5 )
    ENUMID( KC_6 )
    ENUMID( KC_7 )
    ENUMID( KC_8 )
    ENUMID( KC_9 )
    ENUMID( KC_0 )
    ENUMID( KC_MINUS )
    ENUMID( KC_EQUALS )
    ENUMID( KC_BACK )
    ENUMID( KC_TAB )
    ENUMID( KC_Q )
    ENUMID( KC_W )
    ENUMID( KC_E )
    ENUMID( KC_R )
    ENUMID( KC_T )
    ENUMID( KC_Y )
    ENUMID( KC_U )
    ENUMID( KC_I )
    ENUMID( KC_O )
    ENUMID( KC_P )
    ENUMID( KC_LBRACKET )
    ENUMID( KC_RBRACKET )
    ENUMID( KC_RETURN )
    ENUMID( KC_LCONTROL )
    ENUMID( KC_A )
    ENUMID( KC_S )
    ENUMID( KC_D )
    ENUMID( KC_F )
    ENUMID( KC_G )
    ENUMID( KC_H )
    ENUMID( KC_J )
    ENUMID( KC_K )
    ENUMID( KC_L )
    ENUMID( KC_SEMICOLON )
    ENUMID( KC_APOSTROPHE )
    ENUMID( KC_GRAVE )
    ENUMID( KC_LSHIFT )
    ENUMID( KC_BACKSLASH )
    ENUMID( KC_Z )
    ENUMID( KC_X )
    ENUMID( KC_C )
    ENUMID( KC_V )
    ENUMID( KC_B )
    ENUMID( KC_N )
    ENUMID( KC_M )
    ENUMID( KC_COMMA )
    ENUMID( KC_PERIOD )
    ENUMID( KC_SLASH )
    ENUMID( KC_RSHIFT )
    ENUMID( KC_MULTIPLY )
    ENUMID( KC_LMENU )
    ENUMID( KC_SPACE )
    ENUMID( KC_CAPITAL )
    ENUMID( KC_F1 )
    ENUMID( KC_F2 )
    ENUMID( KC_F3 )
    ENUMID( KC_F4 )
    ENUMID( KC_F5 )
    ENUMID( KC_F6 )
    ENUMID( KC_F7 )
    ENUMID( KC_F8 )
    ENUMID( KC_F9 )
    ENUMID( KC_F10 )
    ENUMID( KC_NUMLOCK )
    ENUMID( KC_SCROLL )
    ENUMID( KC_NUMPAD7 )
    ENUMID( KC_NUMPAD8 )
    ENUMID( KC_NUMPAD9 )
    ENUMID( KC_SUBTRACT )
    ENUMID( KC_NUMPAD4 )
    ENUMID( KC_NUMPAD5 )
    ENUMID( KC_NUMPAD6 )
    ENUMID( KC_ADD )
    ENUMID( KC_NUMPAD1 )
    ENUMID( KC_NUMPAD2 )
    ENUMID( KC_NUMPAD3 )
    ENUMID( KC_NUMPAD0 )
    ENUMID( KC_DECIMAL )
    ENUMID( KC_OEM_102 )
    ENUMID( KC_F11 )
    ENUMID( KC_F12 )
    ENUMID( KC_F13 )
    ENUMID( KC_F14 )
    ENUMID( KC_F15 )
    ENUMID( KC_KANA )
    ENUMID( KC_ABNT_C1 )
    ENUMID( KC_CONVERT )
    ENUMID( KC_NOCONVERT )
    ENUMID( KC_YEN )
    ENUMID( KC_ABNT_C2 )
    ENUMID( KC_NUMPADEQUALS )
    ENUMID( KC_PREVTRACK )
    ENUMID( KC_AT )
    ENUMID( KC_COLON )
    ENUMID( KC_UNDERLINE )
    ENUMID( KC_KANJI )
    ENUMID( KC_STOP )
    ENUMID( KC_AX )
    ENUMID( KC_UNLABELED )
    ENUMID( KC_NEXTTRACK )
    ENUMID( KC_NUMPADENTER )
    ENUMID( KC_RCONTROL )
    ENUMID( KC_MUTE )
    ENUMID( KC_CALCULATOR )
    ENUMID( KC_PLAYPAUSE )
    ENUMID( KC_MEDIASTOP )
    ENUMID( KC_VOLUMEDOWN )
    ENUMID( KC_VOLUMEUP )
    ENUMID( KC_WEBHOME )
    ENUMID( KC_NUMPADCOMMA )
    ENUMID( KC_DIVIDE )
    ENUMID( KC_SYSRQ )
    ENUMID( KC_RMENU )
    ENUMID( KC_PAUSE )
    ENUMID( KC_HOME )
    ENUMID( KC_UP )
    ENUMID( KC_PGUP )
    ENUMID( KC_LEFT )
    ENUMID( KC_RIGHT )
    ENUMID( KC_END )
    ENUMID( KC_DOWN )
    ENUMID( KC_PGDOWN )
    ENUMID( KC_INSERT )
    ENUMID( KC_DELETE )
    ENUMID( KC_LWIN )
    ENUMID( KC_RWIN )
    ENUMID( KC_APPS )
    ENUMID( KC_POWER )
    ENUMID( KC_SLEEP )
    ENUMID( KC_WAKE )
    ENUMID( KC_WEBSEARCH )
    ENUMID( KC_WEBFAVORITES )
    ENUMID( KC_WEBREFRESH )
    ENUMID( KC_WEBSTOP )
    ENUMID( KC_WEBFORWARD )
    ENUMID( KC_WEBBACK )
    ENUMID( KC_MYCOMPUTER )
    ENUMID( KC_MAIL )
    ENUMID( KC_MEDIASELECT )
    ENUMID( MB_Left )
    ENUMID( MB_Right ) 
    ENUMID( MB_Middle )
    ENUMID( MB_Button3 )
    ENUMID( MB_Button4 )
    ENUMID( MB_Button5 )
    ENUMID( MB_Button6 )
    ENUMID( MB_Button7 )
    DEVICEBUTTONID_ENUM_LENGTH
};
#undef ENUMID

namespace DeviceId
{
// Device type responsible for EDeviceButton item
enum EDeviceId
{
    DEVICEID_NONE,
    DEVICEID_OIS_MOUSE,
    DEVICEID_OIS_KEYBOARD,
    DEVICEID_ENUM_LENGTH
};
}

std::map<EDeviceButtonId, std::string> _ScapeEngineExport getDeviceButtonIdMap();

// Find the DeviceButtonId from the upper-case name. Returns DEVICEBUTTONID_UNKNOWN otherwise
EDeviceButtonId _ScapeEngineExport getDeviceButtonIdFromUpperName(const string& buttonName);

// Find the DeviceButtonId from the case-insensitive name. Returns DEVICEBUTTONID_UNKNOWN otherwise
EDeviceButtonId _ScapeEngineExport getDeviceButtonIdFromName(const string& buttonName);

// Get the upper-case string of the DeviceButtonId.
string _ScapeEngineExport getDeviceButtonIdToUpperName(EDeviceButtonId button);
}
}

#endif // __DEVICEBUTTONID_H__

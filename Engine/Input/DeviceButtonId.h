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

#if defined(ENUMID)
#error "ENUMID shouldn't be defined at this point"
#endif

namespace ScapeEngine
{

	namespace DeviceButtonId
	{
		// EDeviceButton enum.
		#define ENUMID(a) a, 
		enum EDeviceButtonId {
			DEVICEBUTTONID_UNKNOWN,
			#include "DeviceButtonIdOISKeyboard.def"
			#include "DeviceButtonIdOISMouse.def"
			DEVICEBUTTONID_ENUM_LENGTH
		};
		#undef ENUMID


		namespace DeviceId
		{
			// Device type responsible for EDeviceButton item
			enum EDeviceId {
				DEVICEID_NONE,
				DEVICEID_OIS_MOUSE, 
				DEVICEID_OIS_KEYBOARD,
				DEVICEID_ENUM_LENGTH
			};
		}

		// Find the DeviceButtonId from the upper-case name. Returns DEVICEBUTTONID_UNKNOWN otherwise
		EDeviceButtonId getDeviceButtonIdFromUpperName(const string& buttonName);

		// Find the DeviceButtonId from the case-insensitive name. Returns DEVICEBUTTONID_UNKNOWN otherwise
		EDeviceButtonId getDeviceButtonIdFromName(const string& buttonName);

		// Get the upper-case string of the DeviceButtonId.
		string getDeviceButtonIdToUpperName(EDeviceButtonId button);
	}

}


#endif // __DEVICEBUTTONID_H__
